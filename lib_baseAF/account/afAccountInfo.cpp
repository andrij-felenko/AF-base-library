#include "afAccountInfo.h"
#include <QtCore/QVariant>

#define DATE_TIME "yyyy.MM.dd hh:mm:ss"

AFaccount::Info::Info(QObject *parent) : Info(0, parent)
{
    // TODO
}

AFaccount::Info::Info(uint id, QObject* parent)
    : QObject(parent), AFIdObject(AFIdAccount(id), "", "")
{
    // TODO
}

AFaccount::Info::Info(const AFlib::IdObject &obj, QObject *parent)
    : QObject(parent), AFIdObject(obj)
{
    // TODO there is we need reparse all for get info value from history
}

AFaccount::Info::Info(QJsonObject obj, QObject* parent)
    : Info(obj.value("id").toVariant().toUInt(), parent)
{
    // TODO icon can't covert to QString, it's QByteArray
//    m_icon = obj.value("icon").toString();
    setMail(obj.value("email").toString());
    setName(obj.value("name").toString());
    setDescription(obj.value("description").toString());
    updateTime(QDateTime::fromString(obj.value("last_update").toString(), DATE_TIME));
}

AFaccount::Info::Info(AFlib::AccountIdType type, QObject* parent)
    : Info(AFIdAccount::create(type).toUInt32(), parent)
{
    // TODO fix
}

QByteArray AFaccount::Info::icon() const
{
    return getAttribute(AFattribute::Icon).toByteArray();
}

QString AFaccount::Info::mail() const
{
    return getAttribute(AFattribute::Email).toString();
}

QJsonObject AFaccount::Info::toJson() const
{
    QJsonObject obj;
    obj.insert("id", QJsonValue::fromVariant(owner().toUInt32()));
//    obj.insert("icon", m_icon);
    obj.insert("email", mail());
    obj.insert("name", name());
    obj.insert("description", description());
    obj.insert("last_update", lastUpdate().toString(DATE_TIME));
    return obj;
}

AFaccount::Info::operator QJsonObject() const
{
    return toJson();
}

void AFaccount::Info::setIcon(const QByteArray& icon)
{
    setAttribute(AFattribute::Icon, icon);
}

void AFaccount::Info::setMail(const QString& mail)
{
    setAttribute(AFattribute::Email, mail);
    emit mailChanged(mail);
}

namespace AFaccount {
QDataStream &operator >>(QDataStream &stream, AFaccount::Info &info)
{
    QByteArray icon;
    QString mail;
    stream >> icon >> mail;
    info.setIcon(icon);
    info.setMail(mail);
    stream >> static_cast <AFIdObject&> (info);
    return stream;
}

QDataStream &operator <<(QDataStream &stream, const AFaccount::Info &info)
{
    stream << info.icon() << info.mail();
    stream << static_cast <const AFIdObject&> (info);
    return stream;
}
}

QDataStream &operator >>(QDataStream &stream, AFaccount::InfoPtrList &list)
{
    int count = 0;
    stream >> count;
    for (int i = 0; i < count; i++){
        AFaccount::InfoPtr temp = AFaccount::InfoPtr::create();
        stream >> *temp;
        list.push_back(temp);
    }
    return stream;
}

QDataStream &operator <<(QDataStream &stream, const AFaccount::InfoPtrList &list)
{
    stream << list.length();
    for (auto it : list)
        stream << it.data();
    return stream;
}
