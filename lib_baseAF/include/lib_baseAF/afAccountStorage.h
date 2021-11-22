#ifndef LIB_ACCOUNTAF_ACCOUNT_STORAGE_H
#define LIB_ACCOUNTAF_ACCOUNT_STORAGE_H

#include "afAccount.h"

namespace AFaccount {
    class Storage;
    typedef QSharedPointer <Storage> StoragePtr;

    StoragePtr storage(QObject* parent = nullptr);
    AccountPtr user(QObject* parent = nullptr);
}

#include "afAccountGroup.h"
#include <QtNetwork/QNetworkAccessManager>

/*!
 * \brief The AFaccount::Storage class
 * we load all possible account, and take it on RAM, so we can access to them always in any time,
 * but we need to make shorten every object that we have
 * and it give us speed when we parse this list.
 * \note This class work only with local data, for use data from server need to use AccountAPI class.
 */
class AFaccount::Storage final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isGuest READ isGuest NOTIFY isGuestChanged)
    Q_PROPERTY(Info* user   READ user    NOTIFY userChanged)
public:
    explicit Storage(QObject* parent = nullptr);

    static StoragePtr instance();

    /*!
     * \brief checkLogin
     * \badcode return need rewrite
     * \param login
     * \param password
     * \return Return true if have error, it's hasn't logic, but for now it's ok.
     */
    std::optional <QString> checkLogin(QString login, QString password) const;
    std::optional <QString> checkLogin(QString login, QString password, AFlib::id::Account_bit acc_b) const;
    QList <AFlib::id::Account_bit> dependsAccount(AFlib::id::Account_bit account = AFlib::id::Acc_bit(0)) const;

    bool check(const AFlib::id::Account_bit& id);
    bool check(const quint32& id);

    InfoPtr getInfo(const AFIdAccount& id);
    InfoPtr getInfo(const QString& nick);
    InfoPtr getInfo(const quint32& id);

    bool checkNickname(const QString& nick);

    void add(AccountPtr account, bool isNeedSave = true);
    void add(GroupPtr group,     bool isNeedSave = true);
    bool remove(AFlib::id::Account_bit id);

    bool isGuest() const;
    Account *user() const;

public slots:
    void login(const QString& login, const QString& password);
    void logout();

signals:
    void isGuestChanged();
    void userChanged();

private:
    static QDir m_accountStorageDir;
    AccountPtrV m_accountList;
    GroupPtrV m_groupList;

    void reload();
    void loginAs(AFlib::id::Account_bit account);
    bool save(AccountPtr account);
    bool save(GroupPtr group);

    std::optional <AccountPtr> getAccount(const AFlib::id::Account_bit acc);
    std::optional <GroupPtr>   getGroup  (const AFlib::id::Account_bit acc);

    bool contains(AFlib::id::Account_bit id) const;

    friend class AFlib::API;
};

typedef  AFaccount::Storage AFAccountStorage;

#endif // LIB_ACCOUNTAF_ACCOUNT_STORAGE_H
