#include "accountStorage.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDebug>

#include "lib_baseAF/afDir.h"

AFaccount::Storage::Storage(QObject *parent) : QObject(parent)
{
    load();
}

AFaccount::StoragePtr AFaccount::Storage::instance()
{
    return accountStorage();
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
                return QString(tr("Password not equal."));
        }

    return QString(tr("Login %1 not found in storage.")).arg(login);
}

AFaccount::InfoPtr AFaccount::Storage::getInfo(const AFlib::id::Account_bit &id)
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
        if (it->m_login == nick)
            return true;
    return false;
}

void AFaccount::Storage::add(AccountPtr account)
{
    if (contains(account->owner()))
        return;

    m_accountList.push_back(account);
    save();
}

void AFaccount::Storage::add(GroupPtr group)
{
    if (contains(group->owner()))
        return;

    m_groupList.push_back(group);
    save();
}

void AFaccount::Storage::remove(AFlib::id::Account_bit id)
{
    m_accountList.erase(std::remove_if(m_accountList.begin(), m_accountList.end(),
                                       [id](const AccountPtr a){ return a->owner() == id.accountId(); }));
    m_groupList.erase(std::remove_if(m_groupList.begin(), m_groupList.end(),
                                     [id](const GroupPtr g){ return g->owner() == id.accountId(); }));
    save();
}

void AFaccount::Storage::load()
{
    m_groupList.clear();
    m_accountList.clear();
    QFile file(AFlib::afDir()->storage().absoluteFilePath("accounts.afd"));
    if (file.open(QIODevice::ReadOnly)){
        QDataStream stream(&file);
        stream >> m_accountList << m_groupList;
        file.close();
    }
    else
        qWarning() << "Can`t open account file for load.";
}

void AFaccount::Storage::save()
{
    QFile file(AFlib::afDir()->storage().absoluteFilePath("accounts.afd"));
    if (file.open(QIODevice::Truncate | QIODevice::WriteOnly)){
        QDataStream stream(&file);
        stream << m_accountList << m_groupList;
        file.close();
    }
    else
        qWarning() << "Can`t open account file for save.";
}

bool AFaccount::Storage::contains(AFlib::id::Account_bit id)
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

AFaccount::StoragePtr AFaccount::accountStorage()
{
    static StoragePtr ptr;
    if (ptr.isNull())
        ptr = StoragePtr::create(qApp);

    return ptr;
}
