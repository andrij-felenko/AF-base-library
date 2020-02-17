#include "afIdInfo.h"

AFlib::id::Info::Info(QObject *parent) : Info(QString(""), QString(""), parent)
{
    // it1s done
}

AFlib::id::Info::Info(QString name, QString descr, QObject *parent)
    : QObject(parent), m_name(name), m_description(descr)
{
    // it`s done
}

void AFlib::id::Info::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

void AFlib::id::Info::setDescription(QString description)
{
    if (m_description == description)
        return;

    m_description = description;
    emit descriptionChanged(m_description);
}
namespace AFlib::id {
QDataStream &operator << (QDataStream& stream, const Info& data)
{
    return stream << data.m_name << data.m_description;
}

QDataStream &operator >> (QDataStream& stream,       Info& data)
{
    return stream >> data.m_name >> data.m_description;
}
}
