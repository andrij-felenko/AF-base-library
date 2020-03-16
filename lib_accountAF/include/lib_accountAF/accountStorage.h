#ifndef LIB_ACCOUNTAF_ACCOUNT_STORAGE_H
#define LIB_ACCOUNTAF_ACCOUNT_STORAGE_H

#include "account.h"
#include "accountGroup.h"
//#include  "afBaseLib.h"

namespace AFaccount {
    class Storage;
    typedef QSharedPointer <Storage> StoragePtr;

    StoragePtr storage();
}

/*!
 * \brief The AFaccount::Storage class
 * we load all possible account, and take it on RAM, so we can access to them always in any time,
 * but we need to make shorten every object that we have
 * and it give us speed when we parse this list.
 * \note This class work only with local data, for use data from server need to use AccountAPI class.
 */
class AFaccount::Storage final
{
public:
    explicit Storage();

    static StoragePtr instance();

    /*!
     * \brief checkLogin
     * \badcode return need rewrite
     * \param login
     * \param password
     * \return Return true if have error, it's hasn't logic, but for now it's ok.
     */
    std::optional <QString> checkLogin(const QString& login, const QString& password) const;

    bool check(const AFlib::id::Account_bit& id);
    bool check(const quint32& id);

    InfoPtr getInfo(const AFIdAccount& id);
    InfoPtr getInfo(const QString& nick);
    InfoPtr getInfo(const quint32& id);

    bool checkNickname(const QString& nick);

    void add(AccountPtr account, bool isNeedSave = true);
    void add(GroupPtr group, bool isNeedSave = true);
    bool remove(AFlib::id::Account_bit id);

private:
    AccountPtrList m_accountList;
    GroupPtrList m_groupList;
    static QDir m_accountStorageDir;

    void reload();
    bool save(AccountPtr account);
    bool save(GroupPtr group);

    bool contains(AFlib::id::Account_bit id) const;
};

typedef  AFaccount::Storage AFAccountStorage;

#endif // LIB_ACCOUNTAF_ACCOUNT_STORAGE_H
