#ifndef LIB_BASEAF_ID_OBJECT_BIT_H
#define LIB_BASEAF_ID_OBJECT_BIT_H

#include "afIdBit.h"

namespace AFlib::id {
    struct ObjectUnique_bit;
    struct Object_bit;

    typedef Object_bit Obj_bit;
    typedef ObjectUnique_bit ObjU_bit;
}

//! \brief The Function class
//! \details Bit table, total size <b>28</b>
//! |<center>OBJECT UNIQUE ID TABLE</center>|||||Size|Description|
//! |:-|-:|-:|-:|-:|-:||
//! | |  24|      16|       8|       0| | |
//! |*|####|########|####oooo|oooooooo|16|object unique id by current types|
//! |*|oooo|oooooooo|oooo####|###ooooo| 7|type of object|
//! |*|oooo|oooooooo|oooooooo|ooo#####| 5|plugin id, 0 - mean global variable|
struct AFlib::id::ObjectUnique_bit : public TbitStruct <28>
{
    explicit ObjectUnique_bit(quint32 subjectFull = 0);
    ObjectUnique_bit(quint16 uniqueId, quint8 type, quint8 pluginId);
    operator quint32() const { return toUInt32(); }

    quint32 id()     const { return toUInt32(  ); }
    void setId(quint32 id) {       setUInt32(id); }

    quint16 uniqueId()     const { return toUInt16(    12, 16); }
    void setUniqueId(quint16 id) {       setUInt16(id, 12, 16); }

    quint8  type()      const { return toUInt8(      5, 7); }
    void setType(quint8 type) {       setUInt8(type, 5, 7); }

    template <typename N> N       type() const { return  static_cast <N>(type()); }
    template <typename N> void setType(N type) { setType(static_cast <quint8> (type)); }

    quint8  pluginId()    const { return toUInt8(    0, 5); }
    void setPluginId(quint8 id) {       setUInt8(id, 0, 5); }
};

//! \brief The Function class
//! \details Bit table, total size <b>64</b>
//! |<center>OBJECT ID bit TABLE</center>||||||||||Size|Description|
//! |:-|:-|-:|-:|-:|-:|-:|-:|-:|-:|-:|-:||
//! | |63|     56|      48|      40|   35| 32|      24|      16|       8|       0| | |
//! |*|#|ooooooo|oooooooo|oooooooo|ooooo|ooo|oooooooo|oooooooo|oooooooo|oooooooo| 1|free bit|
//! |*|o|#######|########|########|#####|ooo|oooooooo|oooooooo|oooooooo|oooooooo|28|Object_bit|
//! |*|o|ooooooo|oooooooo|oooooooo|ooooo|###|########|########|########|########|35|Parent id|
//! ||<b>Object_bit:</b>||||||||||||
//! |*||#######|########|#ooooooo|ooooo|   |        |        |        |        |16|object unique id by current types|
//! |*||ooooooo|oooooooo|o#######|ooooo|   |        |        |        |        | 7|type of object|
//! |*||ooooooo|oooooooo|oooooooo|#####|   |        |        |        |        | 5|plugin id, 0 - mean global variable|
//! ||<b>Parent id bit instruction:</b>||||||||||||
//! |*||       |        |        |     |###|####oooo|oooooooo|oooooooo|oooooooo| 7|parent subject type|
//! |*||       |        |        |     |ooo|oooo####|########|########|########|28|parent id, use prev subject or in account id on top|
struct AFlib::id::Object_bit : public TbitStruct <64>
{
    explicit Object_bit(quint64 subjectFull = 0);
    Object_bit(quint32 unique, quint8 parentType, quint32 parentId);
    Object_bit(ObjectUnique_bit unique, quint8 parentType = 0, quint32 parentId = 0);

    AFlib::id::Object_bit object_b() const { return static_cast <AFlib::id::Object_bit>(*this); }
    AFlib::id::ObjectUnique_bit objecttU_b() const { return uid_b(); }

    bool freeBit()          const { return m_bitset    [63]; }
    void setFreeBit(bool freeBit) {        m_bitset.set(63, freeBit); }

    ObjU_bit uid_b()   const { return ObjU_bit(uid()); }
    void setuId(ObjU_bit id) {          setUId(id); }

    quint32 uid()     const { return toUInt32(    35, 28); }
    void setUId(quint32 id) {       setUInt32(id, 35, 28); }

    quint16 subId()     const { return toUInt16(    47, 16); }
    void setSubId(quint16 id) {       setUInt16(id, 47, 16); }

    quint8 type()      const { return toUInt8(      40, 7); }
    void setType(quint8 type) {       setUInt8(type, 40, 7); }

    quint8 pluginId()    const { return toUInt8(    35, 5); }
    void setPLuginId(quint8 id) {       setUInt8(id, 35, 5); }

    quint8 parentType()      const { return toUInt8(      28, 7); }
    void setParentType(quint8 type) {       setUInt8(type, 28, 7); }

    quint32 parentId()     const { return toUInt32(    0, 28); }
    void setParnetId(quint32 id) {       setUInt32(id, 0, 28); }

    void setParent(quint8 type, quint32 id);
};

inline bool operator == (const AFlib::id::Object_bit&       left, const AFlib::id::Object_bit&       right)
{
    return left.toUInt64() == right.toUInt64();
}

inline bool operator == (const AFlib::id::Object_bit&       left, const AFlib::id::ObjectUnique_bit& right)
{
    return left.toUInt32() == right.id();
}

inline bool operator == (const AFlib::id::ObjectUnique_bit& left, const AFlib::id::ObjectUnique_bit& right)
{
    return left.toUInt64() == right.toUInt64();
}

#endif // LIB_BASEAF_ID_OBJECT_BIT_H
