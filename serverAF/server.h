#ifndef AF_MAIN_HTTP_SERVER
#define AF_MAIN_HTTP_SERVER

#include <QtCore/QByteArray>
#include <QtHttpServer/QHttpServer>

class Server final : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject* parent = nullptr);
    virtual ~Server() = default;

    void init();
    int listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 0);

private:
    static QByteArray ping();
    static QByteArray login(const QHttpServerRequest& request);
    static QByteArray registrate(const QHttpServerRequest& request);
    static QByteArray addAccount(const QHttpServerRequest& request);
    static QByteArray addAccountById(const QHttpServerRequest& request);
    static QByteArray remindMyLogin(const QHttpServerRequest& request);
    static QByteArray remindMyPassword(const QHttpServerRequest& request);
    static QByteArray updateObjects(const QHttpServerRequest& request);

    QHttpServer* m_server;
};

#endif // AF_ACCOUNT_HTTP_SERVER
