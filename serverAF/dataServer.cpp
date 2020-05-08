#include "dataServer.h"
#include <AfAPI>
#include <AFaccountLib>
#include <AFbaseLib>

Server::Server(QObject *parent) : QObject(parent)
{
    m_server = new QHttpServer(this);
    init();

    AFlib::afStorage();
    AFaccount::storage(this);
}

void Server::init()
{
    using namespace AFlib;
    m_server->route(getServerKey(RequestType::Ping,             true), &Server::ping);
    m_server->route(getServerKey(RequestType::Login,            true), &Server::login);
    m_server->route(getServerKey(RequestType::AddAccount,       true), &Server::addAccount);
    m_server->route(getServerKey(RequestType::Registrate,       true), &Server::registrate);
    m_server->route(getServerKey(RequestType::RemindMyLogin,    true), &Server::remindMyLogin);
    m_server->route(getServerKey(RequestType::AddAccountById,   true), &Server::addAccountById);
    m_server->route(getServerKey(RequestType::UpdateAfObjects,  true), &Server::updateObjects);
    m_server->route(getServerKey(RequestType::RemindMyPassword, true), &Server::remindMyPassword);
}

int Server::listen(const QHostAddress &address, quint16 port)
{
    return m_server->listen(address, port);
}

QByteArray Server::ping()
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << AFlib::RequestType::Ping;
    return data;
}

QByteArray Server::login(const QHttpServerRequest &request)
{
    QByteArray data = request.body();
    QDataStream streamIn(data);

    // read income data
    QString login;
    QString password;
    streamIn >> login >> password;

    // test is login information is correct
    AFlib::id::Account_bit acc_b;
    auto result = AFaccount::storage()->checkLogin(login, password, acc_b);

    // write return data
    data.clear();
    QDataStream streamOut(&data, QIODevice::WriteOnly);
    streamOut << AFlib::RequestType::Login << result->isNull();
    if (result->isNull())
        streamOut << acc_b;
    else
        streamOut << result.value();

    return data;
}

QByteArray Server::registrate(const QHttpServerRequest &request)
{
    QByteArray data = request.body();
    QDataStream streamIn(data);

    // read income data
    auto accPtr = AFaccount::AccountPtr::create();
    streamIn >> *accPtr->afObjectPtr().get();

    // test is login information is correct
    bool loginFree = false;
    bool mailFree = false;
    bool result = not (accPtr->login().isEmpty() && accPtr->mail().isEmpty());
    if (not accPtr->login().isEmpty()){
        loginFree = AFaccount::storage()->checkNickname(accPtr->login());
        result &= loginFree;
    }
    if (not accPtr->mail().isEmpty()){
        mailFree = AFaccount::storage()->checkNickname(accPtr->mail());
        result &= mailFree;
    }

    // add new Object if result is successfully
    if (result)
        AFaccount::storage()->add(accPtr);

    // write return data
    data.clear();
    QDataStream streamOut(&data, QIODevice::WriteOnly);
    streamOut << AFlib::RequestType::Registrate << result;
    if (result)
        streamOut << *accPtr->afObject();
    else if (loginFree)
        streamOut << "Login already exists.";
    else if (mailFree)
        streamOut << "Mail already exists.";
    else
        streamOut << "Unknown error registarte.";

    return data;
}

QByteArray Server::addAccount(const QHttpServerRequest &request)
{
    Q_UNUSED(request)
    // TODO
    return QByteArray();
}

QByteArray Server::addAccountById(const QHttpServerRequest &request)
{
    Q_UNUSED(request)
    // TODO
    return QByteArray();
}

QByteArray Server::remindMyLogin(const QHttpServerRequest &request)
{
    Q_UNUSED(request)
    // TODO
    return QByteArray();
}

QByteArray Server::remindMyPassword(const QHttpServerRequest &request)
{
    Q_UNUSED(request)
    // TODO
    return QByteArray();
}

QByteArray Server::updateObjects(const QHttpServerRequest &request)
{
    Q_UNUSED(request)
    // TODO
    return QByteArray();
}
