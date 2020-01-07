#include "accountInfo.h"
#include <QtCore/QVariant>

#define DATE_TIME "yyyy.MM.dd hh:mm:ss"

AFaccount::Info::Info(QObject *parent) : Info(0, parent)
{
    //
}

AFaccount::Info::Info(uint id, QObject* parent) : AFlib::HistoryId(parent), AFlib::AccountId(id)
{
    //
}

AFaccount::Info::Info(QJsonObject obj, QObject* parent) : AFlib::HistoryId(parent), AFlib::AccountId(obj.value("id").toVariant().toUInt())
{
    m_icon = obj.value("icon").toString();
    m_mail = obj.value("email").toString();
    m_name = obj.value("name").toString();
    m_lastChange = QDateTime::fromString(obj.value("last_change").toString(), DATE_TIME);
    m_lastUpdate = QDateTime::fromString(obj.value("last_update").toString(), DATE_TIME);
}

AFaccount::Info::Info(AFlib::AccountIdType type, QObject* parent) : Info(AccountId::create(type).toUInt32(), parent)
{
    //
}

AFaccount::Info::operator QJsonObject() const
{
    QJsonObject obj;
    obj.insert("id", QJsonValue::fromVariant(accountId()));
    obj.insert("icon", m_icon);
    obj.insert("email", m_mail);
    obj.insert("name", m_name);
    obj.insert("last_change", m_lastChange.toString(DATE_TIME));
    obj.insert("last_update", m_lastUpdate.toString(DATE_TIME));
    return obj;
}

void AFaccount::Info::setIcon(QString icon)
{
    if (m_icon == icon)
        return;

    m_icon = icon;
    emit iconChanged(m_icon);
}

void AFaccount::Info::setMail(QString mail)
{
    if (m_mail == mail)
        return;

    m_mail = mail;
    emit mailChanged(m_mail);
}

void AFaccount::Info::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

namespace AFaccount {
QDataStream &operator >>(QDataStream &stream, AFaccount::Info &info)
{
    stream >> info.m_icon >> info.m_mail >> info.m_name;
    stream >> static_cast <AFlib::HistoryId&> (info);
    stream >> static_cast <AFlib::AccountId&> (info);
    return stream;
}

QDataStream &operator <<(QDataStream &stream, const AFaccount::Info &info)
{
    stream << info.m_icon << info.m_mail << info.m_name;
    stream << static_cast <const AFlib::HistoryId&> (info);
    stream << static_cast <const AFlib::AccountId&> (info);
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

QDataStream &operator <<(QDataStream &stream, AFaccount::InfoPtrList &list)
{
    stream << list.length();
    for (auto it : list)
        stream << it.data();
    return stream;
}
