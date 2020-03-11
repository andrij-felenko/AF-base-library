#include "httpServer.h"
#include "lib_accountAF/accountStorage.h"
#include "lib_baseAF/afId.h"

#include <QtCore/QJsonDocument>

AccountHttpServer::AccountHttpServer(QObject *parent) : QObject(parent)
{
    m_server = new QHttpServer(this);
    init();
}

void AccountHttpServer::init()
{
    m_server->route("/check_nickname",  &AccountHttpServer::checkNickname);
    m_server->route("/check_updates",   &AccountHttpServer::checkUpdates);
    m_server->route("/found_account",   &AccountHttpServer::foundAccount);
    m_server->route("/login",           &AccountHttpServer::login);
    m_server->route("/logout",          &AccountHttpServer::logout);
    m_server->route("/registration",    &AccountHttpServer::registration);
    m_server->route("/remind_password", &AccountHttpServer::remindPassword);
    m_server->route("/update_account",  &AccountHttpServer::updateAccount);
}

int AccountHttpServer::listen(const QHostAddress &address, quint16 port)
{
    return m_server->listen(address, port);
}

QByteArray AccountHttpServer::checkNickname(const QHttpServerRequest &request)
{
    auto storage = AFaccount::accountStorage();
    QJsonObject obj = QJsonDocument::fromJson(request.body()).object();
    obj.insert("request", "check_nickname");
    obj.insert("result", QJsonValue(storage->checkNickname(obj.value("nick").toString())));
    return QJsonDocument(obj).toJson();
}

QByteArray AccountHttpServer::checkUpdates(const QHttpServerRequest &request)
{
    // TODO add check update init
    return request.body();
}

QByteArray AccountHttpServer::foundAccount(const QHttpServerRequest &request)
{
    auto storage = AFaccount::accountStorage();
    QJsonObject obj = QJsonDocument::fromJson(request.body()).object();
    AFaccount::InfoPtr result;
    if (obj.contains("nick"))
        result = storage->getInfo(obj.value("name").toString());
    else if (obj.contains("id"))
        result = storage->getInfo(obj.value("id").toVariant().toUInt());
    obj.insert("request", "found_account");
    obj.insert("is_found", not result.isNull());
    if (result.isNull())
        obj.insert("error", tr("Account not found with this parameter."));
    else {
        // TODO add register login device info
        obj.insert("account", result->toJson());
    }
    return QJsonDocument(obj).toJson();
}

QByteArray AccountHttpServer::login(const QHttpServerRequest &request)
{
    auto storage = AFaccount::accountStorage();
    QJsonObject obj = QJsonDocument::fromJson(request.body()).object();
    auto result = storage->checkLogin(obj.value("name").toString(), obj.value("password").toString());
    obj.insert("request", "login");
    obj.insert("result", QJsonValue(not result.has_value()));
    if (result.has_value())
        obj.insert("error", result.value());
    return QJsonDocument(obj).toJson();
}

QByteArray AccountHttpServer::logout(const QHttpServerRequest &request)
{
    // TODO add logout
    return request.body();
}

QByteArray AccountHttpServer::registration(const QHttpServerRequest &request)
{
    auto storage = AFaccount::accountStorage();
    auto obj = QJsonDocument::fromJson(request.body()).object();
    auto account = AFaccount::Account(obj);
    if (storage->checkNickname(obj.value("login").toString())){
        // TODO maybe add some test before registrate
        auto newAcc = AFIdAccount::create(AFlib::AccountIdType::User);
        while (not storage->check(newAcc))
            newAcc = AFIdAccount::create(AFlib::AccountIdType::User);

        obj.insert("id", QJsonValue::fromVariant(newAcc.accountId()));
        storage->add(AFaccount::AccountPtr::create(obj));
        obj.insert("result", true);
    }
    else {
        obj.insert("result", false);
        obj.insert("error", tr("Login already exist."));
    }

    return QJsonDocument(obj).toJson();
}

QByteArray AccountHttpServer::remindPassword(const QHttpServerRequest &request)
{
    auto storage = AFaccount::accountStorage();
    QJsonObject obj = QJsonDocument::fromJson(request.body()).object();
    obj.insert("request", "check_nickname");
    bool result = storage->checkNickname(obj.value("nick").toString());
    obj.insert("result", QJsonValue(result));
    if (result){
        // TODO send notification to email
    }
    else
        obj.insert("error", "Login not exist.");
    return QJsonDocument(obj).toJson();
}

QByteArray AccountHttpServer::updateAccount(const QHttpServerRequest &request)
{
    // TODO update account
    return request.body();
}
