//#ifndef AF_ACCOUNT_HTTP_SERVER
//#define AF_ACCOUNT_HTTP_SERVER

////#include <QtCore/QByteArray>
////#include <QtHttpServer/QHttpServer>

////class AccountHttpServer final : public QObject
////{
////    Q_OBJECT
////public:
////    explicit AccountHttpServer(QObject* parent = nullptr);
////    ~AccountHttpServer() = default;

////    void init();
////    int listen(const QHostAddress &address = QHostAddress::Any, quint16 port = 0);

////private:
////    static QByteArray checkNickname (const QHttpServerRequest &request);
////    static QByteArray checkUpdates  (const QHttpServerRequest &request);
////    static QByteArray updateAccount (const QHttpServerRequest &request);
////    static QByteArray login         (const QHttpServerRequest &request);
////    static QByteArray logout        (const QHttpServerRequest &request);
////    static QByteArray registration  (const QHttpServerRequest &request);
////    static QByteArray remindPassword(const QHttpServerRequest &request);
////    static QByteArray foundAccount  (const QHttpServerRequest &request);

////    QHttpServer* m_server;
////};

//#endif // AF_ACCOUNT_HTTP_SERVER
