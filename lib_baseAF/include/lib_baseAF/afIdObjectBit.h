#ifndef LIB_BASEAF_ID_OBJECT_BIT_H
#define LIB_BASEAF_ID_OBJECT_BIT_H

#include "afIdBit.h"

namespace AFlib::id {
    struct Object_bit;

    typedef Object_bit Obj_bit;
}

//! \brief The Function class
//! \details Bit table, total size <b>28</b>
//! |<center>OBJECT BIT ID TABLE</center>|||||Size|Description|
//! |:-|-:|-:|-:|-:|-:||
//! | |      24|      16|       8|       0| | |
//! |*|########|########|####oooo|oooooooo|20|object unique id by current types|
//! |*|oooooooo|oooooooo|oooo####|###ooooo| 7|type of object|
//! |*|oooooooo|oooooooo|oooooooo|ooo#####| 5|plugin id, 0 - mean global variable|
struct AFlib::id::Object_bit : private TbitStruct <32>
{
    explicit Object_bit(quint32 subjectFull = 0);

    AFlib::id::Object_bit object_b() const { return static_cast <AFlib::id::Object_bit>(*this); }

    quint32 uniqueId() const { return toUInt32(12, 20); }
    void setUniqueId(quint32 id) { setUInt32(id, 12, 20); }

    quint32 id() const { return toUInt32(); }
    void setId(quint32 id) { setUInt32(id); }

//    bool freeBit()          const { return m_bitset    [63]; }
//    void setFreeBit(bool freeBit) {        m_bitset.set(63, freeBit); }

//    ObjU_bit uid_b()   const { return ObjU_bit(uid()); }
//    void setuId(ObjU_bit id) {          setUId(id); }

//    quint32 uid()     const { return toUInt32(    35, 28); }
//    void setUId(quint32 id) {       setUInt32(id, 35, 28); }

//    quint16 subId()     const { return toUInt16(    47, 16); }
//    void setSubId(quint16 id) {       setUInt16(id, 47, 16); }

    quint8 type()      const { return toUInt8(      40, 7); }
    void setType(quint8 type) {       setUInt8(type, 40, 7); }

    template <typename N> N       type() const { return  static_cast <N>(type()); }
    template <typename N> void setType(N type) { setType(static_cast <quint8> (type)); }

    quint8 pluginId()    const { return toUInt8(    35, 5); }
    void setPluginId(quint8 id) {       setUInt8(id, 35, 5); }

    template <typename N> N       plugin() const { return      static_cast <N>(pluginId()); }
    template <typename N> void setPlugin(N id)   { setPluginId(static_cast <quint8> (id)); }

//    quint8 parentType()      const { return toUInt8(      28, 7); }
//    void setParentType(quint8 type) {       setUInt8(type, 28, 7); }

//    template <typename N> N       parentType() const { return  static_cast <N>(parentType()); }
//    template <typename N> void setParentType(N type) { setParentType(static_cast <quint8> (type)); }

//    quint32 parentId()     const { return toUInt32(    0, 28); }
//    void setParnetId(quint32 id) {       setUInt32(id, 0, 28); }

//    void setParent(quint8 type, quint32 id);
};

inline bool operator == (const AFlib::id::Object_bit& left, const AFlib::id::Object_bit& right)
{
    return left.id() == right.id();
}

#endif // LIB_BASEAF_ID_OBJECT_BIT_H
