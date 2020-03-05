#ifndef LIB_ACCOUNTAF_ACCOUNT_STORAGE_H
#define LIB_ACCOUNTAF_ACCOUNT_STORAGE_H

#include "account.h"
#include "accountGroup.h"
#include "afBaseLib.h"

namespace AFaccount {
    class Storage;
    typedef QSharedPointer <Storage> StoragePtr;

    StoragePtr storage();
}

class AFaccount::Storage final
{
public:
    explicit Storage(QObject* parent = nullptr);

    static StoragePtr instance();

//    std::optional <QString> checkLogin(const QString& login, const QString& password) const;

//    bool check(const AFlib::id::Account_bit& id);
//    bool check(const quint32& id);

//    InfoPtr getInfo(const AFlib::id::Account_bit& id);
//    InfoPtr getInfo(const QString& nick);
//    InfoPtr getInfo(const quint32& id);

//    bool checkNickname(const QString& nick);

//    void add(AccountPtr account, bool isNeedSave = true);
//    void add(GroupPtr group, bool isNeedSave = true);
//    void remove(AFlib::id::Account_bit id);

private:
//    AccountPtrList m_accountList;
//    GroupPtrList m_groupList;

//    void load();
//    void save();

//    bool contains(AFlib::id::Account_bit id);
};

typedef  AFaccount::Storage AFAccountStorage;

#endif // LIB_ACCOUNTAF_ACCOUNT_STORAGE_H
