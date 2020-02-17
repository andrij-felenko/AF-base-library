#ifndef LIB_BASEAF_ID_SUBJECT_BIT_H
#define LIB_BASEAF_ID_SUBJECT_BIT_H

#include "afIdBit.h"

namespace AFlib::id {
    struct Object_bit;
    struct ObjectFull_bit;

    typedef Object_bit Obj_bit;
}

/* _________OBJECT_ID_Bit_________
 *
 *   24       16        8        0
 *    |        |        |        |
 * #### ######## ####.... ........ 16: object unique id by current types
 * .... ........ ....#### ###.....  7: type of object
 * .... ........ ........ ...#####  5: plugin id [0 - mean global variable]
 * .... ........ ........ ........ */
struct AFlib::id::Object_bit : public TbitStruct <28>
{
    explicit Object_bit(quint32 subjectFull = 0);
    Object_bit(quint16 uniqueId, quint8 type, quint8 pluginId);
    operator quint32() const { return toUInt32(); }

    quint32 id()     const { return toUInt32(  ); }
    void setId(quint32 id) {       setUInt32(id); }

    quint32 uniqueId()     const { return toUInt16(    12, 16); }
    void setUniqueId(quint16 id) {       setUInt16(id, 12, 16); }

    quint32 type()      const { return toUInt8(      5, 7); }
    void setType(quint8 type) {       setUInt8(type, 5, 7); }

    quint32 pluginId()    const { return toUInt8(    0, 5); }
    void setPluginId(quint8 id) {       setUInt8(id, 0, 5); }
};

/* _________________________OBJECT_FULL_ID_Bit___________________________
 * |_______[28]_OBJECT_ID________|
 * 62   56       48       40       32       24       16        8        0
 * |     |        |        |        |        |        |        |        |
 * ####### ######## #....... ........ ........ ........ ........ ........ 16: object unique id by current types
 * ....... ........ .####### ........ ........ ........ ........ ........  7: type of object
 * ....... ........ ........ #####... ........ ........ ........ ........  5: plugin id [0 - mean global variable]
 * ....... ........ ........ .....### ####.... ........ ........ ........  7: parent subject type
 * ....... ........ ........ ........ ....#### ######## ######## ######## 28: parent id [use prev subject or in account id on top]
 * ....... ........ ........ ........ ........ ........ ........ ........ */
struct AFlib::id::ObjectFull_bit : public TbitStruct <63>
{
    explicit ObjectFull_bit(quint64 subjectFull = 0);
    ObjectFull_bit(quint32 objectId, quint8 parentType, quint32 parentId);
    ObjectFull_bit(Object_bit subject, quint8 parentType, quint32 parentId);

    Obj_bit id_b()   const { return Obj_bit(id()); }
    void setId(Obj_bit id) {      setId( id.id()); }

    quint32 id()     const { return toUInt32(    35, 28); }
    void setId(quint32 id) {       setUInt32(id, 35, 28); }

    quint32 uniqueId()     const { return toUInt16(    47, 16); }
    void setUniqueId(quint16 id) {       setUInt16(id, 47, 16); }

    quint32 type()      const { return toUInt8(      40, 7); }
    void setType(quint8 type) {       setUInt8(type, 40, 7); }

    quint32 pluginId()    const { return toUInt8(    35, 5); }
    void setPLuginId(quint8 id) {       setUInt8(id, 35, 5); }

    quint32 parentType()      const { return toUInt8(      28, 7); }
    void setParentType(quint8 type) {       setUInt8(type, 28, 7); }

    quint32 parentId()     const { return toUInt32(    0, 28); }
    void setParnetId(quint32 id) {       setUInt32(id, 0, 28); }

    void setParent(quint8 type, quint32 id);
};

#endif // LIB_BASEAF_ID_SUBJECT_BIT_H
