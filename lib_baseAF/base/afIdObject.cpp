#include "afIdObject.h"

AFlib::id::Object::Object() : Object(Account_bit(), "", "", 0, 0, 0, 0, 0)
{
    // it`s done
}

AFlib::id::Object::Object(const QByteArray &data)
{
    QDataStream stream(data);
    stream >> *this;
}

AFlib::id::Object::Object(Account_bit owner, quint16 uniqueId, quint8 type, quint8 pluginId)
    : Object(owner, "", "", uniqueId, type, pluginId, 0, 0)
{
    // it`s done
}

AFlib::id::Object::Object(Account_bit owner, QString name, QString descr)
    : Object(owner, name, descr, 0, 0, 0, 0, 0)
{
    // it`s done
}

AFlib::id::Object::Object(Account_bit owner, QString name, QString descr, quint16 uniqueId, quint8 type,
                          quint8 pluginId, quint8 parentType, quint32 parentId)
      : ObjectFull_bit(Object_bit(uniqueId, type, pluginId), parentType, parentId),
      m_owner(owner)
{
    setName(name);
    setDescription(descr);
}

void AFlib::id::Object::makeGlobalId(quint32 newId)
{
    setId(newId);
}

QString AFlib::id::Object::name() const
{
    return getValue(ValueType::Name).toString();
}

QString AFlib::id::Object::description() const
{
    return getValue(ValueType::Description).toString();
}

void AFlib::id::Object::setName(const QString &name)
{
    if (getValue(ValueType::Name).isNull())
        addOperate(ValueType::Name, name, m_owner);
    else
        addOperate(ValueType::Name, name, m_owner, HIdType::EditIdLine);
}

void AFlib::id::Object::setDescription(const QString &description)
{
    if (getValue(ValueType::Description).isNull())
        addOperate(ValueType::Description, description, m_owner);
    else
        addOperate(ValueType::Description, description, m_owner, HIdType::EditIdLine);
}

bool AFlib::id::Object::setOwner(const Account_bit &owner)
{
    if (not m_operateList.isEmpty())
        return false;

    m_owner = owner;
    return true;
}

namespace AFlib::id {
QDataStream &operator << (QDataStream& stream, const AFlib::id::Object& data)
{
    stream << data.m_owner << static_cast <const History&>(data) << data.m_bitset;
    return stream;
}

QDataStream &operator >> (QDataStream& stream,       AFlib::id::Object& data)
{
    stream >> data.m_owner >> static_cast <History&>(data) >> data.m_bitset;
    return stream;
}
}

QDataStream &operator << (QDataStream& stream, const AFlib::id::ObjectPtrList& data)
{
    stream << data.count();
    for (auto it = data.begin(); it != data.end(); ++it)
        stream << *it->data();
    return stream;
}

QDataStream &operator >> (QDataStream& stream,       AFlib::id::ObjectPtrList& data)
{
    int count;
    stream >> count;
    for (int i = 0; i < count; i++){
        AFlib::id::ObjectPtr ptr;
        stream >> *ptr;
        data.push_back(ptr);
    }
    return stream;
}
