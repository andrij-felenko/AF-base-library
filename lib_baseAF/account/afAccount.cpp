#include "lib_baseAF/afAccount.h"
#include <QtCore/QVariant>
#include <QtCore/QThread>
#include <QtCore/QJsonArray>
#include <QtCore/QCryptographicHash>
#include <QtCore/QCoreApplication>

AFaccount::Account::Account(const AFlib::IdObject &account) : AFaccount::Info(account)
{
    // TODO
}

AFaccount::Account::Account(QObject *parent) : AFaccount::Info(parent)
{
    //
}

AFaccount::Account::Account(QJsonObject obj, QObject *parent) : AFaccount::Info(obj, parent)
{
    setLogin(obj.value("login").toString());
    setAttribute(AFattribute::Password, obj.value("password").toString());
    auto friendArray = obj.value("friend_list").toArray();
    // TODO rewrite
//    for (auto it : friendArray)
//        m_friendList.push_back(InfoPtr::create(it.toObject(), this));
}

AFaccount::AccountPtr AFaccount::own()
{
    static AccountPtr m_me;
    if (m_me.isNull())
        m_me = AccountPtr::create(qApp);

    return m_me;
}

bool AFaccount::Account::check(QString password) const
{
    thread()->wait(20);
    return password == passwordHash() || password == QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5);
}
// TODO rewrite
//AFaccount::InfoPtrList AFaccount::Account::friendList() const
//{
//    return m_friendList;
//}

QString AFaccount::Account::passwordHash() const
{
    return getAttribute(AFattribute::Password).toString();
}

QString AFaccount::Account::login() const
{
    return getAttribute(AFattribute::Login).toString();
}

QJsonObject AFaccount::Account::toJson() const
{
    QJsonObject obj = static_cast <const Info&> (*this);
    obj.insert("login", login());
    obj.insert("password", passwordHash());
    QJsonArray friendArray;
    // TODO rewrite
//    for (auto it : m_friendList)
//        friendArray.push_back(QJsonObject(*it.data()));
    obj.insert("friend_list", friendArray);
    return obj;
}

AFaccount::Account::operator QJsonObject() const
{
    return toJson();
}

void AFaccount::Account::setLogin(QString login)
{
    setAttribute(AFattribute::Login, login);
}

namespace AFaccount {
QDataStream& operator >> (QDataStream &stream, AFaccount::Account &account)
{
    QString hash, login;
    stream >> login >> hash;// TODO rewrite >> account.m_friendList;
    account.setLogin(login);
    account.setAttribute(AFattribute::Password, hash);
    stream >> static_cast <Info&> (account);
    return stream;
}

QDataStream& operator << (QDataStream &stream, const AFaccount::Account &account)
{
    stream << account.login() << account.passwordHash();// TODO rewrite << account.m_friendList;
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

QDataStream &operator <<(QDataStream &stream, const AFaccount::AccountPtrList &list)
{
    stream << list.length();
    for (auto it : list)
        stream << *it;
    return stream;
}
