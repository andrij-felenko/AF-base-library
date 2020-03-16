#include "accountStorage.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDebug>

#include <AFbase/AfDir>
#include <AFbase/AfStorage>
#include <AFbase/AfEnum>

AFaccount::Storage::Storage()
{
    if (m_accountStorageDir == QDir::current())
        m_accountStorageDir = AFlib::afDir()->users();
    reload();
}

AFaccount::StoragePtr AFaccount::Storage::instance()
{
    return storage();
}

bool AFaccount::Storage::check(const AFlib::id::Account_bit &id)
{
    for (auto it : m_accountList)
        if (it->owner() == id.accountId())
            return true;

    for (auto it : m_groupList)
        if (it->owner() == id.accountId())
            return true;

    return false;
}

bool AFaccount::Storage::check(const quint32 &id)
{
    return check(AFlib::id::Account_bit(id));
}

std::optional <QString> AFaccount::Storage::checkLogin(const QString &login, const QString &password) const
{
    for (auto it : m_accountList)
        if (it->mail() == login || it->login() == login){
            if (it->check(password))
                return std::nullopt;
            else
                return QString("Password not equal.");
        }

    return QString("Login %1 not found in storage.").arg(login);
}

AFaccount::InfoPtr AFaccount::Storage::getInfo(const AFIdAccount &id)
{
    auto accId = id.accountId();
    for (int i = 0; i < m_accountList.length(); i++)
        if (m_accountList[i]->owner() == accId)
            return m_accountList[i];
    return InfoPtr();
}

AFaccount::InfoPtr AFaccount::Storage::getInfo(const QString &nick)
{
    for (int i = 0; i < m_accountList.length(); i++)
        if (m_accountList[i]->name() == nick)
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
    if (contains(account->owner()))
        return;

    m_accountList.push_back(account);
    if (isNeedSave)
        save(account);
}

void AFaccount::Storage::add(GroupPtr group, bool isNeedSave)
{
    if (contains(group->owner()))
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

    m_accountList.erase(std::remove_if(m_accountList.begin(), m_accountList.end(),
                                       [id](const AccountPtr a){ return a->owner() == id.accountId(); }));
    m_groupList.erase(std::remove_if(m_groupList.begin(), m_groupList.end(),
                                     [id](const GroupPtr g){ return g->owner() == id.accountId(); }));
    return AFlib::afStorage()->removeObjectId(m_accountStorageDir, QString::number(id, 16), info.data()->id_b());
}

void AFaccount::Storage::reload()
{
    m_groupList.clear();
    m_accountList.clear();

    // read all objects
    auto list = AFlib::afStorage()->readObjectList(m_accountStorageDir, true, AFCompressValue::Shortest);

    // remove object that not account
    list.erase(std::remove_if(list.begin(), list.end(), [=](AFIdObjectPtr ptr) { return ptr->uniqueId() != 0; }));

    // separate it to group and acount list
    for (auto it : list){
        if (it->owner().accountType() == AFlib::AccountIdType::User){
            auto ptr = AFaccount::AccountPtr::create(it.operator*());
            m_accountList.push_back(ptr);
        }
        else {
            auto ptr = AFaccount::GroupPtr::create(it.operator*());
            m_groupList.push_back(ptr);
        }
    }
}

bool AFaccount::Storage::save(AccountPtr account)
{
    return AFlib::afStorage()->updateFile(m_accountStorageDir, QString::number(account->id(), 16), account->getData());
}

bool AFaccount::Storage::save(GroupPtr group)
{
    return AFlib::afStorage()->updateFile(m_accountStorageDir, QString::number(group->id(), 16), group->getData());
}

bool AFaccount::Storage::contains(AFlib::id::Account_bit id) const
{
    auto accId = id.accountId();
    for (auto it : m_accountList)
        if (it->owner() == accId)
            return true;

    for (auto it : m_groupList)
        if (it->owner() == accId)
            return true;

    return false;
}

AFaccount::StoragePtr AFaccount::storage()
{
    static StoragePtr ptr;
    if (ptr.isNull())
        ptr = StoragePtr::create();

    return ptr;
}
