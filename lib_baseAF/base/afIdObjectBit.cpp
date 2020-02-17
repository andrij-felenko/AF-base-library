#include "afIdObjectBit.h"

AFlib::id::Object_bit::Object_bit(quint32 subjectFull)
{
    setUInt32(subjectFull);
}

AFlib::id::Object_bit::Object_bit(quint16 uniqueId, quint8 type, quint8 pluginId)
{
    setUniqueId(uniqueId);
    setType(type);
    setPluginId(pluginId);
}

AFlib::id::ObjectFull_bit::ObjectFull_bit(quint64 subjectFull)
{
    setUInt64(subjectFull);
}

AFlib::id::ObjectFull_bit::ObjectFull_bit(quint32 subjectId, quint8 parentType, quint32 parentId)
{
    setId(subjectId);
    setParent(parentType, parentId);
}

AFlib::id::ObjectFull_bit::ObjectFull_bit(Object_bit subject, quint8 parentType, quint32 parentId)
{
    setId(subject);
    setParent(parentType, parentId);
}

void AFlib::id::ObjectFull_bit::setParent(quint8 type, quint32 id)
{
    setParnetId(id);
    setParentType(type);
}
