#ifndef LIB_ACCOUNTAF_ACCOUNT_API_H
#define LIB_ACCOUNTAF_ACCOUNT_API_H

#include "accountStorage.h"
#include <QtNetwork/QNetworkAccessManager>

namespace AFaccount {
    class API;
    typedef QSharedPointer <API> AccountAPIPtr;
    typedef QList <AccountAPIPtr> AccountAPIPtrList;

    enum class RequestType {
        Login = 0xCE8100,
        IsLoginFree,
        Registrate,
        UpdateListOfAccount,

        AddOperate = 0xCE8110,
        AddOperateList,
        ChangePassword,

        LoadGroup = 0xCE8120,
    };
    QString getServerKey(const RequestType type, bool isData = false);
    QUrl getServerLink(const RequestType type);

    AccountAPIPtr api();
}

class AFaccount::API : public QObject
{
public:
    explicit API(QObject* parent = nullptr);
    static AccountAPIPtr instance();
    static QString serverLink;

    static void login(const QString& login, const QString& password, const QString& device_name);
    static void remind(const QString& email_login);
    static void logout(const QString& login, const QString& device_name);
    static void checkNew(const QDateTime& dtime_last_update);
    static void registrate(const Account& account);
    static void checkFreeName(const QString& name);

signals:
    void changeOnlineStatus(const bool&/* online ? true : false */);

    // can offline
    void resultLogin(const QString&/* login */, const QString&/* password */, const QString&/* device name */);
    void resultLogout(const bool&/* login */, const QString&/* device name */);

    // need online
    void resultCheckFreeName(const bool&/* result of online status */,
                             const bool& = false/* result request if online status true */);
    void resultCheckNew(const bool&, QHash <AFlib::id::Operate, QVariant >/* hash */);
    void resultRegistrate(const bool&, const QJsonObject& = QJsonObject()/* json result account */);
    void resultRemind(const QString&/* email or login */);

private:
    QNetworkAccessManager* m_server;
    QSharedPointer <QNetworkRequest> createRequest();
    void readServerAnswer(QNetworkReply* reply);
};

#endif // LIB_ACCOUNTAF_ACCOUNT_API_H
