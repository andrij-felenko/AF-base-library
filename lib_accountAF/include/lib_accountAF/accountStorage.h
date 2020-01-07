#ifndef LIB_ACCOUNTAF_ACCOUNT_STORAGE_H
#define LIB_ACCOUNTAF_ACCOUNT_STORAGE_H

#include "account.h"
#include "accountGroup.h"

namespace AFaccount {
    class Storage;
    typedef QSharedPointer <Storage> StoragePtr;

    static StoragePtr accountStorage();
}

class AFaccount::Storage : public QObject
{
public:
    explicit Storage(QObject* parent = nullptr);

    static StoragePtr instance();
    std::optional<QString> check(const QString& login, const QString& password) const;
    InfoPtr getInfo(const AFlib::AccountId& id);

private:
    AccountPtrList m_accountList;
    GroupPtrList m_groupList;

    void add(AccountPtr account);
    void add(GroupPtr group);
    void remove(AFlib::AccountId id);

    void load();
    void save();

    bool contains(AFlib::AccountId id);
};

#endif // LIB_ACCOUNTAF_ACCOUNT_STORAGE_H
