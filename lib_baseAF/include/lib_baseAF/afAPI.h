#ifndef LIB_AF_API_H
#define LIB_AF_API_H

#include "afAccountStorage.h"
#include <QtCore/QMap>
#include <QtCore/QTimer>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

namespace AFlib {
    class API;
    typedef QSharedPointer <API> ApiPtr;
    typedef QList <ApiPtr> ApiPtrList;

    enum class RequestType {
        // unlogin
        Login = 0xCE8100,
        Registrate,
        RemindMyLogin,
        RemindMyPassword,
        Ping,

        // already login
        UpdateAfObjects = 0xCE8110,
        AddAccount,
        AddAccountById,
    };

    enum class RequestStatus {
        None,
        Send,
        False,
        True,
    };

    QString getServerKey(const RequestType type, bool isData = false);
    QUrl getServerLink(const RequestType type, bool isData = false);

    ApiPtr api();

    QDataStream &operator << (QDataStream& d, const AFlib::RequestType& type);
    QDataStream &operator >> (QDataStream& d,       AFlib::RequestType& type);
}

class AFlib::API : public QNetworkAccessManager
{
    Q_OBJECT
    Q_PROPERTY(bool online READ online NOTIFY onlineChanged)
public:
    explicit API(QObject* parent = nullptr);
    static ApiPtr instance();
    static QString defaultLink;

    void login(QString login, QString password);
    void registrate(AFaccount::AccountPtr account);
    void remindMyLogin(QString login);
    void remindMyPassword(QString login);

    void addAccount(QString login);
    void addAccount(IdAccount_bit id);
    void updateObjects();

    bool online() const;

signals:
    void onlineChanged(bool online);

private:
    void readServerAnswer(QNetworkReply* reply);
    void post(RequestType type, QByteArray data, bool isData = false);
    void get(RequestType type, bool isData = false);

    QMap <RequestType, RequestStatus> m_requestStatus;

    // ping objects and functions
    bool m_online;
    QTimer* m_pingTimer;
    void sendPing();
    void setOnlineStatus(bool is_online);
};

typedef QSharedPointer <AFlib::API> AFapiPtr;
typedef QList <AFapiPtr> AFapiPtrList;

AFapiPtr afAPI();

#endif // LIB_ACCOUNTAF_ACCOUNT_API_H
