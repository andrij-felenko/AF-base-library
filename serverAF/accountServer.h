#ifndef AF_ACCOUNT_SERVER
#define AF_ACCOUNT_SERVER

#include <QtCore/QByteArray>
#include <QtCore/QJsonObject>
#include <QtHttpServer/QHttpServer>

class AccountServer final : public QObject
{
    Q_OBJECT
public:
    explicit AccountServer(QObject* parent = nullptr);
    ~AccountServer() = default;

    void init();
    int listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 0);

private:
    // account unauth
    static QByteArray login(const QString& login, const QString& password);
    static QByteArray loginFree(const QString &login);
    static QByteArray registrate(const QHttpServerRequest& request);
    static QByteArray updateListOfAccount(const QHttpServerRequest& request);

    // account auth done
    static QByteArray addOperate(const QHttpServerRequest& request);
    static QByteArray addOperateList(const QHttpServerRequest& request);
    static QByteArray changePassword(const QString& login, const QString& oldPassword,
                                     const QString& newPassword);

    // group
    static QByteArray loadGroup(const QHttpServerRequest& request);

//    static QJsonObject jsonLogin              (const QJsonObject& json);
//    static QJsonObject jsonLoginFree          (const QJsonObject& json);
//    static QJsonObject jsonRegistrate         (const QJsonObject& json);
//    static QJsonObject jsonUpdateAccount      (const QJsonObject& json);
//    static QJsonObject jsonUpdateListOfAccount(const QJsonObject& json);
//    static QJsonObject jsonChangePassword     (const QJsonObject& json);

    QHttpServer* m_server;
};

#endif
