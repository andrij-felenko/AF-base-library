#include "account.h"
#include <QtCore/QVariant>
#include <QtCore/QThread>
#include <QtCore/QJsonArray>
#include <QtCore/QCryptographicHash>
#include <QtCore/QCoreApplication>

AFaccount::Account::Account(QObject *parent) : AFaccount::Info(parent)
{
    //
}

AFaccount::Account::Account(QJsonObject obj, QObject *parent) : AFaccount::Info(obj, parent)
{
    m_login = obj.value("login").toString();
    m_passwordHash = obj.value("password").toString();
    for (auto it : obj.value("currencies").toVariant().toStringList())
        m_currencyList.push_back(CurrencyAF::Type::toEnum(it));
    auto friendArray = obj.value("friend_list").toArray();
    for (auto it : friendArray)
        m_friendList.push_back(InfoPtr::create(it.toObject(), this));
}

AFaccount::AccountPtr AFaccount::Account::me()
{
    static AccountPtr m_me;
    if (m_me.isNull())
        m_me = AccountPtr::create(qApp);

    return m_me;
}

bool AFaccount::Account::check(QString password) const
{
    thread()->wait(20);
    return password == m_passwordHash || password == QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5);
}

QList <CurrencyEnum> AFaccount::Account::currencyList() const
{
    return m_currencyList;
}
      
QStringList AFaccount::Account::currencyStringList() const
{
    QStringList ret;
    for (auto it : m_currencyList)
        ret += CurrencyAF::Type::toShort(it);
    return ret;
}

AFaccount::InfoPtrList AFaccount::Account::friendList() const
{
    return m_friendList;
}

QString AFaccount::Account::login() const
{
    return m_login;
}

AFaccount::Account::operator QJsonObject() const
{
    QJsonObject obj = static_cast <const Info&> (*this);
    obj.insert("login", m_login);
    obj.insert("password", m_passwordHash);
    obj.insert("currencies", QJsonValue::fromVariant(QVariant::fromValue(currencyStringList())));
    QJsonArray friendArray;
    for (auto it : m_friendList)
        friendArray.push_back(QJsonObject(*it.data()));
    obj.insert("friend_list", friendArray);
    return obj;
}

void AFaccount::Account::setLogin(QString login)
{
    if (m_login == login)
        return;

    m_login = login;
    emit loginChanged(m_login);
}

namespace AFaccount {
QDataStream& operator >> (QDataStream &stream, AFaccount::Account &account)
{
    stream >> account.m_login >> account.m_passwordHash >> account.m_friendList >> account.m_currencyList;
    stream >> static_cast <Info&> (account);
    return stream;
}

QDataStream& operator << (QDataStream &stream, const AFaccount::Account &account)
{
    stream << account.m_login << account.m_passwordHash << account.m_friendList << account.m_currencyList;
    stream << static_cast <const Info&> (account);
    return stream;
}
}

QDataStream &operator >>(QDataStream &stream, AFaccount::AccountPtrList &list)
{
    int count = 0;
    stream >> count;
    for (int i = 0; i < count; i++){
        AFaccount::AccountPtr temp = AFaccount::AccountPtr::create();
        stream >> *temp;
        list.push_back(temp);
    }
    return stream;
}

QDataStream &operator <<(QDataStream &stream, AFaccount::AccountPtrList &list)
{
    stream << list.length();
    for (auto it : list)
        stream << it.data();
    return stream;
}
