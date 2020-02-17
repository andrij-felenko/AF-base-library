#include "afIdObject.h"

AFlib::id::Object::Object(QObject *parent) : Object("", "", 0, 0, 0, 0, 0, parent)
{
    // it`s done
}

AFlib::id::Object::Object(quint16 uniqueId, quint8 type, quint8 pluginId, QObject *parent)
    : Object("", "", uniqueId, type, pluginId, 0, 0, parent)
{
    // it`s done
}

AFlib::id::Object::Object(QString name, QString descr, QObject *parent)
    : Object(name, descr, 0, 0, 0, 0, 0, parent)
{
    // it`s done
}

AFlib::id::Object::Object(QString name, QString descr, quint16 uniqueId, quint8 type,
                          quint8 pluginId, quint8 parentType, quint32 parentId, QObject *parent)
    : Info(name, descr, parent),
    ObjectFull_bit(Object_bit(uniqueId, type, pluginId), parentType, parentId)
{
    // it`s done
}

namespace AFlib::id {
QDataStream &operator << (QDataStream& stream, const AFlib::id::Object& data)
{
    stream
        << *data.m_history
        << static_cast <const Info&> (data)
        << static_cast <const ObjectFull_bit&> (data);
    return stream;
}

QDataStream &operator >> (QDataStream& stream,       AFlib::id::Object& data)
{
    stream
        >> *data.m_history
        >> static_cast <Info&> (data)
        >> static_cast <ObjectFull_bit&> (data);
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
