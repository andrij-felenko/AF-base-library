#include "afAPI.h"
#include <optional>
#include <QtCore/QCoreApplication>
#include "afServerSettings.h"
#include "AfStorage"
#include <QtCore/QByteArray>
#include "afAccountStorage.h"

QString AFlib::API::defaultLink = QString("127.0.0.1:55023");

AFlib::API::API(QObject *parent) : QNetworkAccessManager(parent)
{
    m_pingTimer = new QTimer(this);
    // TODO tray settings (30 or 10) / 2
    m_pingTimer->setInterval(5000);

    connect(this, &QNetworkAccessManager::finished, this, &API::readServerAnswer);
}

void AFlib::API::readServerAnswer(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError){
        qDebug() << "Request has error" << reply->errorString() << reply->url();
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QDataStream stream(data);
    RequestType type;
    stream >> type;

    bool result(true);
    if (type != RequestType::Ping)
        stream >> result;
    if (result){
        // TODO success result
        m_requestStatus.insert(type, RequestStatus::True);
    }
    else {
        // TODO emit error string, add this error to error list,
        // need content name of error and text
        QString errorStr;
        stream >> errorStr;
        m_requestStatus.insert(type, RequestStatus::False);
        reply->deleteLater();
        return;
    }

    switch (type){
    case AFlib::RequestType::Ping: setOnlineStatus(true); break;
    case AFlib::RequestType::Login: {
        AFlib::id::Account_bit account;
        stream >> account;
        AFaccount::storage()->loginAs(account);
        break;
    }
    case AFlib::RequestType::AddAccount: {
        AFlib::id::Account_bit owner;
        AFlib::IdObjectPtr account;
        stream >> owner >> *account.get();
        AFaccount::AccountPtr accountPtr = AFaccount::AccountPtr::create(account);
        auto currentUser = AFaccount::storage()->user();
        if (owner == currentUser->afObject()->owner())
            currentUser->addAccount(account);
        // TODO else send error
        break;
    }
    case AFlib::RequestType::Registrate: {
        // 1. registrate new account
        AFaccount::AccountPtr ptr;
        stream >> *ptr.data()->afObjectPtr();
        AFaccount::storage()->add(ptr);
        // 2. autorizate it
        AFaccount::storage()->login(ptr->login(), ptr->passwordHash());
        break;
    }
    case AFlib::RequestType::RemindMyLogin:    break;
    case AFlib::RequestType::RemindMyPassword: break;
    case AFlib::RequestType::UpdateAfObjects: {
        transfer::Answer answer;
        transfer::Send sendNew;
        stream >> answer >> sendNew;
        afStorage()->updateObjects(answer, sendNew);
        break;
    }
    default: qDebug() << "Wrong key request.";
    }

    reply->deleteLater();
}

void AFlib::API::post(RequestType type, QByteArray data, bool isData)
{
    QNetworkRequest request;
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/octet-stream;");
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    request.setUrl(getServerLink(type, isData));
    m_requestStatus.insert(type, RequestStatus::Send);
    QNetworkAccessManager::post(request, data);
    qDebug() << "AFlib::API::post: " << data;
}

void AFlib::API::get(RequestType type, bool isData)
{
    QNetworkRequest request;
    request.setUrl(getServerLink(type, isData));
    m_requestStatus.insert(type, RequestStatus::Send);
    QNetworkAccessManager::get(request);
}

void AFlib::API::sendPing()
{
    get(RequestType::Ping);
    m_pingTimer->start();
}

void AFlib::API::setOnlineStatus(bool is_online)
{
    if (is_online == m_online)
        return;

    m_online = is_online;
    emit onlineChanged(m_online);
    m_pingTimer->start();
}

AFlib::ApiPtr AFlib::API::instance()
{
    return api();
}

void AFlib::API::login(QString login, QString password)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << login << password;
    post(RequestType::Login, data, true);
}

void AFlib::API::registrate(AFaccount::AccountPtr account)
{
    if (m_requestStatus[RequestType::Registrate] == RequestStatus::Send)
        return;

    post(RequestType::Registrate, account->toByteArray(), true);
}

void AFlib::API::remindMyLogin(QString login)
{
    if (m_requestStatus[RequestType::RemindMyLogin] == RequestStatus::Send)
        return;

    post(RequestType::RemindMyLogin, login.toUtf8(), true);
}

void AFlib::API::remindMyPassword(QString login)
{
    if (m_requestStatus[RequestType::RemindMyPassword] == RequestStatus::Send)
        return;

    post(RequestType::RemindMyPassword, login.toUtf8(), true);
}

void AFlib::API::addAccount(QString login)
{
    if (m_requestStatus[RequestType::AddAccount] == RequestStatus::Send)
        return;

    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << AFaccount::storage()->user()->afObject()->owner() << login;

    post(RequestType::AddAccount, login.toUtf8(), true);
}

void AFlib::API::addAccount(IdAccount_bit id)
{
    if (m_requestStatus[RequestType::AddAccountById] == RequestStatus::Send)
        return;

    post(RequestType::AddAccountById, QByteArray::number(id.toUInt32()), true);
}

void AFlib::API::updateObjects()
{
    if (m_requestStatus[RequestType::UpdateAfObjects] == RequestStatus::Send)
        return;

    post(RequestType::UpdateAfObjects,
         afStorage()->getOperatesAfter(afServerSettings()->lastStorageUpdate()), true);
}

bool AFlib::API::online() const
{
    return m_online;
}

AFlib::ApiPtr AFlib::api()
{
    static ApiPtr ptr;
    if (ptr.isNull())
        ptr = ApiPtr::create(qApp);

    return ptr;
}

QString AFlib::getServerKey(const AFlib::RequestType type, bool isData)
{
    QString key = "/";
    if (isData)
        key += "data/";
    switch (type) {
    case RequestType::Ping:             key += "ping";
    case RequestType::Login:            key += "login";
    case RequestType::Registrate:       key += "registrate";
    case RequestType::AddAccount:       key += "add_account";
    case RequestType::AddAccountById:   key += "add_account_by_id";
    case RequestType::RemindMyLogin:    key += "remind_my_login";
    case RequestType::UpdateAfObjects:  key += "update_objects";
    case RequestType::RemindMyPassword: key += "remind_my_password";
    }
    return key;
}

QUrl AFlib::getServerLink(const AFlib::RequestType type, bool isData)
{
    QString key = afServerSettings()->getUrl("base", API::defaultLink);
    key.append(getServerKey(type, isData));
    return QUrl::fromUserInput(key);
}

AFapiPtr afAPI()
{
    return AFlib::api();
}

namespace AFlib {
    QDataStream &operator << (QDataStream &d, const AFlib::RequestType &type)
    {
        return d << static_cast <ushort> (type);
    }

    QDataStream &operator >> (QDataStream &d, AFlib::RequestType &type)
    {
        ushort retShort;
        d >> retShort;
        type = static_cast <AFlib::RequestType> (retShort);
        return d;
    }
}
