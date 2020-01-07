#include "accountStorage.h"
#include <QtCore/QCoreApplication>
#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDebug>

#include "lib_baseAF/dir.h"

AFaccount::Storage::Storage(QObject *parent) : QObject(parent)
{
    load();
}

AFaccount::StoragePtr AFaccount::Storage::instance()
{
    return accountStorage();
}

std::optional <QString> AFaccount::Storage::check(const QString &login, const QString &password) const
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

AFaccount::InfoPtr AFaccount::Storage::getInfo(const AFlib::AccountId &id)
{
    auto accId = id.accountId();
    for (int i = 0; i < m_accountList.length(); i++)
        if (m_accountList[i]->accountId() == accId)
            return m_accountList[i];
    return InfoPtr();
}

void AFaccount::Storage::add(AccountPtr account)
{
    if (contains(static_cast <AFlib::AccountId> (*account)))
        return;

    m_accountList.push_back(account);
    save();
}

void AFaccount::Storage::add(GroupPtr group)
{
    if (contains(static_cast <AFlib::AccountId> (*group)))
        return;

    m_groupList.push_back(group);
    save();
}

void AFaccount::Storage::remove(AFlib::AccountId id)
{
    m_accountList.erase(std::remove_if(m_accountList.begin(), m_accountList.end(),
                                       [id](const AccountPtr a){ return a->accountId() == id.accountId(); }));
    m_groupList.erase(std::remove_if(m_groupList.begin(), m_groupList.end(),
                                     [id](const GroupPtr g){ return g->accountId() == id.accountId(); }));
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

bool AFaccount::Storage::contains(AFlib::AccountId id)
{
    auto accId = id.accountId();
    for (auto it : m_accountList)
        if (it->accountId() == accId)
            return true;

    for (auto it : m_groupList)
        if (it->accountId() == accId)
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
