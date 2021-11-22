#include "afAccountInfo.h"
#include <QtCore/QVariant>

#define DATE_TIME "yyyy.MM.dd hh:mm:ss"

AFaccount::Info::Info(QObject *parent) : Info(0, parent)
{
    // TODO
}

//AFaccount::Info::Info(uint id, QObject* parent)
//    : AFlib::id::ObjectTemplate(AFIdObjectPtr::create(AFIdAccount(id)), parent)
//{
//    // TODO
//}

AFaccount::Info::Info(AFlib::IdObjectPtr ptr, QObject *parent)
    : AFlib::id::ObjectTemplate(ptr, parent)
{
    // TODO there is we need reparse all for get info value from history
}

//AFaccount::Info::Info(QJsonObject obj, QObject* parent)
//    : Info(obj.value("id").toVariant().toUInt(), parent)
//{
//    // TODO icon can't covert to QString, it's QByteArray
////    m_icon = obj.value("icon").toString();
//    setMail(obj.value("email").toString());
//    m_ptr->setName(obj.value("name").toString());
//    m_ptr->setDescription(obj.value("description").toString());
//    m_ptr->updateTime(QDateTime::fromString(obj.value("last_update").toString(), DATE_TIME));
//}

//AFaccount::Info::Info(AFlib::AccountIdType type, QObject* parent)
//    : Info(AFIdAccount::create(type).toUInt32(), parent)
//{
//    // TODO fix
//}

QByteArray AFaccount::Info::icon() const
{
    return m_ptr->getAttribute(AFattribute::Icon).toByteArray();
}

QString AFaccount::Info::mail() const
{
    return m_ptr->getAttribute(AFattribute::Email).toString();
}

QJsonObject AFaccount::Info::toJson() const
{
    QJsonObject obj;
    obj.insert("id", QJsonValue::fromVariant(m_ptr->owner().toUInt32()));
    // TODO send icon as json
    // obj.insert("icon", m_icon);
    obj.insert("email", mail());
    obj.insert("name", m_ptr->name());
    obj.insert("description", m_ptr->description());
    obj.insert("last_update", m_ptr->lastUpdate().toString(DATE_TIME));
    return obj;
}

AFaccount::Info::operator QJsonObject() const
{
    return toJson();
}

void AFaccount::Info::setIcon(const QByteArray& icon)
{
    m_ptr->setAttribute(AFattribute::Icon, icon);
}

void AFaccount::Info::setMail(const QString& mail)
{
    m_ptr->setAttribute(AFattribute::Email, mail);
    emit mailChanged(mail);
}
