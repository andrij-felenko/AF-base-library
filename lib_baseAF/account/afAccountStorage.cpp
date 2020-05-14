#include "afAccountStorage.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDebug>

#include <AFbase/AfDir>
#include <AFbase/AfStorage>
#include <AFbase/AfEnum>

AFaccount::AccountPtr m_user;
AFaccount::AccountPtr m_guest;
AFaccount::StoragePtr m_accountStorage;
QDir AFaccount::Storage::m_accountStorageDir = QDir::current();

AFaccount::Storage::Storage(QObject* parent) : QObject(parent)
{
    if (m_accountStorageDir == QDir::current())
        m_accountStorageDir = AFlib::afDir()->users();
    reload();

    connect(this, &Storage::userChanged, this, &Storage::isGuestChanged);
}

AFaccount::StoragePtr AFaccount::Storage::instance()
{
    return storage();
}

bool AFaccount::Storage::check(const AFlib::id::Account_bit &id)
{
    for (auto it : m_accountList)
        if (it->afObject()->owner() == id.accountId())
            return true;

    for (auto it : m_groupList)
        if (it->afObject()->owner() == id.accountId())
            return true;

    return false;
}

bool AFaccount::Storage::check(const quint32 &id)
{
    return check(AFlib::id::Account_bit(id));
}

std::optional <QString> AFaccount::Storage::checkLogin(QString login, QString password) const
{
    return checkLogin(login, password, AFlib::id::Account_bit());
}

std::optional<QString> AFaccount::Storage::checkLogin(QString login, QString password, AFlib::id::Account_bit acc_b) const
{
    for (auto it : m_accountList)
        if (it->mail() == login || it->login() == login){
            if (it->check(password)){
                acc_b = it->afObject()->owner();
                return std::nullopt;
            }
            else
                return QString("Password not equal.");
        }

    return QString("Login %1 not found in storage.").arg(login);
}

QList<AFlib::id::Account_bit> AFaccount::Storage::dependsAccount(AFlib::id::Account_bit account) const
{
    if (account.toUInt32() == 0)
        account = AFaccount::storage()->user()->afObject()->owner();

    AFlib::id::AccList_b retList;
    retList.push_back(account);
    auto accPtr = AFaccount::storage()->getAccount(account);
    if (accPtr){
        for (auto it : accPtr.value()->friendList())
            retList.push_back(it);
        for (auto it : accPtr.value()->groupList())
            retList.push_back(it);
    }

    return retList;
}

AFaccount::InfoPtr AFaccount::Storage::getInfo(const AFIdAccount &id)
{
    auto accId = id.accountId();
    for (uint i = 0; i < m_accountList.size(); i++)
        if (m_accountList[i]->afObject()->owner() == accId
            || m_accountList[i]->afObject()->owner() == accId)
            return m_accountList[i];
    return InfoPtr();
}

AFaccount::InfoPtr AFaccount::Storage::getInfo(const QString &nick)
{
    for (uint i = 0; i < m_accountList.size(); i++)
        if (m_accountList[i]->afObject()->name() == nick)
            return m_accountList[i];
    return InfoPtr();
}

AFaccount::InfoPtr AFaccount::Storage::getInfo(const quint32 &id)
{
    return getInfo(AFlib::id::Account_bit(id));
}

bool AFaccount::Storage::checkNickname(const QString &nick)
{
    for (auto it : m_accountList)
        if (it->login() == nick)
            return true;
    return false;
}

void AFaccount::Storage::add(AccountPtr account, bool isNeedSave)
{
    if (contains(account->afObject()->owner()))
        return;

    m_accountList.push_back(account);
    if (isNeedSave)
        save(account);
}

void AFaccount::Storage::add(GroupPtr group, bool isNeedSave)
{
    if (contains(group->afObject()->owner()))
        return;

    m_groupList.push_back(group);
    if (isNeedSave)
        save(group);
}

bool AFaccount::Storage::remove(AFlib::id::Account_bit id)
{
    auto info = getInfo(id);
    if (info.isNull())
        return false;

    auto eraseAcc   = [id](const AccountPtr a){ return a->afObject()->owner() == id.accountId(); };
    auto eraseGroup = [id](const   GroupPtr g){ return g->afObject()->owner() == id.accountId(); };
    m_accountList.erase(std::remove_if(m_accountList.begin(), m_accountList.end(), eraseAcc));
    m_groupList  .erase(std::remove_if(m_groupList  .begin(), m_groupList  .end(), eraseGroup));
    return AFlib::afStorage()->removeObject(*info->afObject());
}

bool AFaccount::Storage::isGuest() const
{
    return m_user == m_guest;
}

AFaccount::Account *AFaccount::Storage::user() const
{
    return AFaccount::user().get();
}

void AFaccount::Storage::login(const QString &login, const QString &password)
{
    auto result = checkLogin(login, password);
    if (result->isNull()){
        m_user = AFaccount::AccountPtr::create(getInfo(login)->afObjectPtr());
        emit userChanged();
    }
    // TODO else send error login
}

void AFaccount::Storage::logout()
{
    m_user = m_guest;
    emit userChanged();
    // TODO send error logout
}

void AFaccount::Storage::reload()
{
    m_groupList.clear();
    m_accountList.clear();

    // read all objects
    QDir accountDir = AFDir()->users();
    if (not AFlib::Dir::cdDirectory(accountDir, "accounts"))
        return;

    QDir groupDir = AFDir()->storage();
    if (not AFlib::Dir::cdDirectory(accountDir, "groups"))
        return;

    AFlib::IdObjectPtrV list;
    for (auto it : accountDir.entryList({"*.afd"}, QDir::Files))
        list += AFlib::afStorage()->getObjectList({ "accounts", it }, AFlib::FileType::Account, AFcompress::Shortest);
    for (auto it : groupDir.entryList({"*.afd"}, QDir::Files))
        list += AFlib::afStorage()->getObjectList({ "groups", it }, AFlib::FileType::Account, AFcompress::Shortest);

    // remove object that not account
//    auto lastObject = std::remove_if(list.begin(), list.end(), [=](AFIdObjectPtr ptr) { return ptr->isAccount(); });
    list.erase(list.end()/* lastObject*/);

    // separate it to group and acount list
    for (auto it : list){
        if (it->owner().accountType() == AFlib::AccountIdType::User){
            auto ptr = AFaccount::AccountPtr::create(it);
            m_accountList.push_back(ptr);
        }
        else {
            auto ptr = AFaccount::GroupPtr::create(it);
            m_groupList.push_back(ptr);
        }
    }
}

void AFaccount::Storage::loginAs(AFlib::id::Account_bit account)
{
    m_user = AFaccount::AccountPtr::create(getInfo(account)->afObjectPtr());
    emit userChanged();
    // TODO else send error login
}

bool AFaccount::Storage::save(AccountPtr account)
{
    return AFlib::afStorage()->updateFile({ "accounts", QString::number(account->afObject()->owner(), 16) },
                                          account->afObject()->getData(), AFlib::FileType::Account);
}

bool AFaccount::Storage::save(GroupPtr group)
{
    return AFlib::afStorage()->updateFile({ "groups", QString::number(group->afObject()-> owner(), 16) },
                                          group->afObject()->getData(), AFlib::FileType::Account);
}

std::optional <AFaccount::AccountPtr> AFaccount::Storage::getAccount(const AFlib::id::Account_bit acc)
{
    for (auto it : m_accountList)
        if (it->afObject()->owner() == acc)
            return it;
    return std::nullopt;
}

std::optional<AFaccount::GroupPtr> AFaccount::Storage::getGroup(const AFlib::id::Account_bit acc)
{
    for (auto it : m_groupList)
        if (it->afObject()->owner() == acc)
            return it;
    return std::nullopt;
}

bool AFaccount::Storage::contains(AFlib::id::Account_bit id) const
{
    auto accId = id.accountId();
    for (auto it : m_accountList)
        if (it->afObject()->owner() == accId)
            return true;

    for (auto it : m_groupList)
        if (it->afObject()->owner() == accId)
            return true;

    return false;
}

AFaccount::StoragePtr AFaccount::storage(QObject* parent)
{
    if (m_accountStorage.isNull())
        m_accountStorage = AFaccount::StoragePtr::create(parent);

    return m_accountStorage;
}

AFaccount::AccountPtr AFaccount::user(QObject *parent)
{
    if (m_user.isNull()){
        if (m_guest.isNull())
            m_guest = AFaccount::AccountPtr::create(parent);
        m_user = m_guest;
    }
    return m_user;
}
