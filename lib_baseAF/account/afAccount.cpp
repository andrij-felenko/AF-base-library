#include "lib_baseAF/afAccount.h"
#include <QtCore/QVariant>
#include <QtCore/QThread>
#include <QtCore/QJsonArray>
#include <QtCore/QCryptographicHash>
#include <QtCore/QCoreApplication>

AFaccount::Account::Account(AFlib::IdObjectPtr ptr, QObject* parent)
    : AFaccount::Info(ptr, parent)
{
    // TODO
}

AFaccount::Account::Account(const QByteArray &data, QObject* parent)
  : AFaccount::Info(parent)
{
    QDataStream stream(data);
    stream >> *m_ptr.data();
}

AFaccount::Account::Account(QObject *parent) : AFaccount::Info(parent)
{
    //
}

//AFaccount::Account::Account(QJsonObject obj, QObject *parent) : AFaccount::Info(obj, parent)
//{
//    setLogin(obj.value("login").toString());
//    m_ptr->setAttribute(AFattribute::Password, obj.value("password").toString());
//    auto friendArray = obj.value("friend_list").toArray();
//    // TODO rewrite
////    for (auto it : friendArray)
////        m_friendList.push_back(InfoPtr::create(it.toObject(), this));
//}

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
    return m_ptr->getAttribute(AFattribute::Password).toString();
}

void AFaccount::Account::addAccount(AFlib::IdObjectPtr account)
{
    auto account_b = account->owner().toUInt32();
    m_ptr->setMultiAttribute(AFattribute::FriendList, account_b, AFlib::HIdType::AddIdLine);
}

void AFaccount::Account::removeAccount(AFlib::IdObjectPtr account)
{
    auto account_b = account->owner().toUInt32();
    m_ptr->setMultiAttribute(AFattribute::FriendList, account_b, AFlib::HIdType::RemoveIdLine);
}

void AFaccount::Account::addGroup(AFlib::IdObjectPtr account)
{
    auto account_b = account->owner().toUInt32();
    m_ptr->setMultiAttribute(AFattribute::GroupList, account_b, AFlib::HIdType::RemoveIdLine);
}

void AFaccount::Account::removeGroup(AFlib::IdObjectPtr account)
{
    auto account_b = account->owner().toUInt32();
    m_ptr->setMultiAttribute(AFattribute::GroupList, account_b, AFlib::HIdType::RemoveIdLine);
}

QString AFaccount::Account::login() const
{
    return m_ptr->getAttribute(AFattribute::Login).toString();
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

QByteArray AFaccount::Account::toByteArray() const
{
    QByteArray ret;
    QDataStream stream(&ret, QIODevice::WriteOnly);
    stream << *afObject();
    return ret;
}

void AFaccount::Account::reset()
{
    //
}

AFlib::id::AccList_b AFaccount::Account::friendList() const
{
    AFlib::id::AccList_b ret;
    for (auto it : m_ptr->getMultiAttribute(AFattribute::FriendList))
        ret += AFlib::id::Account_bit(it.toUInt());
    return ret;
}

AFlib::id::AccList_b AFaccount::Account::groupList() const
{
    AFlib::id::AccList_b ret;
    for (auto it : m_ptr->getMultiAttribute(AFattribute::GroupList))
        ret += AFlib::id::Account_bit(it.toUInt());
    return ret;
}

AFaccount::Account::operator QJsonObject() const
{
    return toJson();
}

void AFaccount::Account::setLogin(QString login)
{
    m_ptr->setAttribute(AFattribute::Login, login);
}
