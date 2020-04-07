#include "afIdObjectBit.h"

AFlib::id::ObjectUnique_bit::ObjectUnique_bit(quint32 subjectFull)
{
    setUInt32(subjectFull);
}

AFlib::id::ObjectUnique_bit::ObjectUnique_bit(quint16 uniqueId, quint8 type, quint8 pluginId)
{
    setUniqueId(uniqueId);
    setType(type);
    setPluginId(pluginId);
}

AFlib::id::Object_bit::Object_bit(quint64 subjectFull)
{
    setUInt64(subjectFull);
}

AFlib::id::Object_bit::Object_bit(quint32 unique, quint8 parentType, quint32 parentId)
{
    setUId(unique);
    setParent(parentType, parentId);
}

AFlib::id::Object_bit::Object_bit(ObjectUnique_bit unique, quint8 parentType, quint32 parentId)
{
    setUId(unique);
    setParent(parentType, parentId);
}

void AFlib::id::Object_bit::setParent(quint8 type, quint32 id)
{
    setParnetId(id);
    setParentType(type);
}
