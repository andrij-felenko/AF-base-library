#include "accountServer.h"
#include <AFaccount/AfAccountStorage>
#include <AFbase/AfAPI>

AccountServer::AccountServer(QObject *parent) : QObject(parent)
{
    m_server = new QHttpServer(this);
    init();

    // init account storage
    AFaccount::storage();
}

void AccountServer::init()
{
    using namespace AFlib;
    // account
//    m_server->route(getServerKey(RequestType::Login),               &AccountServer::login);
//    m_server->route(getServerKey(RequestType::IsLoginFree),         &AccountServer::loginFree);
//    m_server->route(getServerKey(RequestType::Registrate),          &AccountServer::registrate);
//    m_server->route(getServerKey(RequestType::UpdateListOfAccount), &AccountServer::updateListOfAccount);

    // account auth
//    m_server->route(getServerKey(RequestType::AddOperate),     &AccountServer::addOperate);
//    m_server->route(getServerKey(RequestType::AddOperateList), &AccountServer::addOperateList);
//    m_server->route(getServerKey(RequestType::ChangePassword), &AccountServer::changePassword);

    // group
//    m_server->route(getServerKey(RequestType::LoadGroup), &AccountServer::loadGroup);

//    m_server->route(getServerKey(RequestType::Login),               false, &AccountServer::jsonLogin);
//    m_server->route(getServerKey(RequestType::IsLoginFree),         false, &AccountServer::jsonLoginFree);
//    m_server->route(getServerKey(RequestType::Registrate),          false, &AccountServer::jsonRegistrate);
//    m_server->route(getServerKey(RequestType::UpdateAccount),       false, &AccountServer::jsonUpdateAccount);
//    m_server->route(getServerKey(RequestType::ChangePassword),      false, &AccountServer::jsonChangePassword);
//    m_server->route(getServerKey(RequestType::UpdateListOfAccount), false, &AccountServer::jsonUpdateListOfAccount);
}

int AccountServer::listen(const QHostAddress &address, quint16 port)
{
    return m_server->listen(address, port);
}

//QByteArray AccountServer::login(const QString &login, const QString &password)
//{
//    qDebug() << "Call AccountServer::login:" << login << password;
//    auto result = AFaccount::storage()->checkLogin(login, password);

//    QByteArray retData;
//    QDataStream retStream(&retData, QIODevice::WriteOnly);
//    retStream << AFlib::RequestType::Login << login << not bool(result);
//    qDebug() << "AccountServer::login result:" << login << password << bool(result);
//    if (result){
//        retStream << result.value();
//        qDebug() << result.value();
//    }
//    return retData;
//}

//QByteArray AccountServer::loginFree(const QString& login)
//{
//    qDebug() << "Call AccountServer::loginFree:" << login;
//    bool result = AFaccount::storage()->checkNickname(login);

//    QByteArray retData;
//    QDataStream retStream(&retData, QIODevice::WriteOnly);
//    retStream << AFlib::RequestType::IsLoginFree << login << result;
//    qDebug() << "AccountServer::loginFree result:" << login << result;
//    return retData;
//}

//QByteArray AccountServer::registrate(const QHttpServerRequest &request)
//{
//    // TODO
//    return request.body();
//}

//QByteArray AccountServer::updateListOfAccount(const QHttpServerRequest &request)
//{
//    QDataStream stream(request.body());
//    QHash <AFIdAccount, QDateTime> hash;
//    stream >> hash;
//    qDebug() << "Call AccountServer::updateListOfAccount:" << hash;

//    // TODO
//    return request.body();
//}

//// account auth done
//QByteArray AccountServer::addOperate(const QHttpServerRequest &request)
//{
//    // TODO
//    return request.body();
//}

//QByteArray AccountServer::addOperateList(const QHttpServerRequest &request)
//{
//    // TODO
//    return request.body();
//}

//QByteArray AccountServer::changePassword(const QString &login, const QString &oldPassword, const QString &newPassword)
//{
//    // TODO
//    Q_UNUSED(login)
//    Q_UNUSED(oldPassword)
//    Q_UNUSED(newPassword)
//    return QByteArray();
//}

//// Group
//QByteArray AccountServer::loadGroup(const QHttpServerRequest &request)
//{
//    // TODO
//    return request.body();
//}

//QJsonObject AccountServer::jsonLogin(const QJsonObject &json)
//{
//    QJsonObject result;
//    // TODO
//    Q_UNUSED(json)
//    return result;
//}

//QJsonObject AccountServer::jsonLoginFree(const QJsonObject &json)
//{
//    QJsonObject result;
//    // TODO
//    Q_UNUSED(json)
//    return result;
//}

//QJsonObject AccountServer::jsonRegistrate(const QJsonObject &json)
//{
//    QJsonObject result;
//    // TODO
//    Q_UNUSED(json)
//    return result;
//}

//QJsonObject AccountServer::jsonUpdateAccount(const QJsonObject &json)
//{
//    QJsonObject result;
//    // TODO
//    Q_UNUSED(json)
//    return result;
//}

//QJsonObject AccountServer::jsonUpdateListOfAccount(const QJsonObject &json)
//{
//    QJsonObject result;
//    // TODO
//    Q_UNUSED(json)
//    return result;
//}

//QJsonObject AccountServer::jsonChangePassword(const QJsonObject &json)
//{
//    QJsonObject result;
//    // TODO
//    Q_UNUSED(json)
//    return result;
//}
