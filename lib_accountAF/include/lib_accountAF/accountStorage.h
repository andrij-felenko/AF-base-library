#ifndef LIB_ACCOUNTAF_ACCOUNT_STORAGE_H
#define LIB_ACCOUNTAF_ACCOUNT_STORAGE_H

#include "account.h"
#include "accountGroup.h"

namespace AFaccount {
    class Storage;
    typedef QSharedPointer <Storage> StoragePtr;

    StoragePtr accountStorage();
}

class AFaccount::Storage : public QObject
{
public:
    explicit Storage(QObject* parent = nullptr);

    static StoragePtr instance();

    std::optional <QString> checkLogin(const QString& login, const QString& password) const;

    bool check(const AFlib::id::Account_bit& id);
    bool check(const quint32& id);

    InfoPtr getInfo(const AFlib::id::Account_bit& id);
    InfoPtr getInfo(const QString& nick);
    InfoPtr getInfo(const quint32& id);

    bool checkNickname(const QString& nick);

    void add(AccountPtr account);
    void add(GroupPtr group);
    void remove(AFlib::id::Account_bit id);

private:
    AccountPtrList m_accountList;
    GroupPtrList m_groupList;

    void load();
    void save();

    bool contains(AFlib::id::Account_bit id);
};

#endif // LIB_ACCOUNTAF_ACCOUNT_STORAGE_H
