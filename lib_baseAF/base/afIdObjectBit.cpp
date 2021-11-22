#include "afIdObjectBit.h"
#include "AfFunction"

AFlib::id::Object_bit::Object_bit(quint32 id)
{
    setUInt32(id);
}

AFlib::id::Object_bit::Object_bit(quint8 plugin, quint8 type)
{
    setPluginId(plugin);
    setType(type);
}

AFlib::id::Object_bit AFlib::id::Object_bit::object_b() const
{
    return static_cast <AFlib::id::Object_bit>(*this);
}

bool AFlib::id::Object_bit::isIdLocal() const
{
    return uniqueId() < 1 << 10;
}

bool AFlib::id::Object_bit::isEmpty() const
{
    return uniqueId() == 0;
}

quint32 AFlib::id::Object_bit::createLocalId()
{
    auto i = Function::randomInt(1, (1 << 10) - 1);
    return i;
}

quint32 AFlib::id::Object_bit::createGlobalId()
{
    return Function::randomInt(1 << 10, (1 << 20) - 1);
}

AFlib::id::Object_bit::Object_bit(quint8 plugin, quint8 type, quint32 uid)
{
    setPluginId(plugin);
    setType(type);
    setUniqueId(uid);
}

QDebug operator <<(QDebug d, const AFlib::id::Object_bit &obj_b)
{
    return d << "AFlib::id::Object_bit :\n\t"
             << "unique id:    0x" << QString::number(obj_b.uniqueId(), 16) << ";\n\t"
             << "type:        "    << obj_b.type() << ";\n\t"
             << "plugin:      "    << obj_b.pluginId() << ";\n\n";
}
