#include "accountAPI.h"
#include <optional>
#include <QtCore/QCoreApplication>

AFaccount::API::API(QObject *parent) : QObject(parent)
{
    m_server = new QNetworkAccessManager(this);
}

QSharedPointer<QNetworkRequest> AFaccount::API::createRequest()
{
    // TODO preferences file need to add request url for account API
    QUrl accountUrlAPI = QUrl::fromUserInput("");
    auto req = QSharedPointer <QNetworkRequest>::create(accountUrlAPI);
    return req;
}

void AFaccount::API::readServerAnswer(QNetworkReply *reply)
{
    // TODO
}

AFaccount::AccountAPIPtr AFaccount::API::instance()
{
    return api();
}

void AFaccount::API::login(const QString &login, const QString &password, const QString &device_name)
{
    // TODO
}

void AFaccount::API::remind(const QString &email_login)
{
    // TODO
}

void AFaccount::API::logout(const QString &login, const QString &device_name)
{
    // TODO
}

void AFaccount::API::checkNew(const QDateTime &dtime_last_update)
{
    // TODO
}

void AFaccount::API::registrate(const Account &account)
{
    // TODO
}

void AFaccount::API::checkFreeName(const QString &name)
{
    // TODO
}

AFaccount::AccountAPIPtr AFaccount::api()
{
    static AccountAPIPtr ptr;
    if (ptr.isNull())
        ptr = AccountAPIPtr::create(qApp);

    return ptr;
}
