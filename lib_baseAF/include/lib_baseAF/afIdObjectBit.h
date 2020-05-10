#ifndef LIB_BASEAF_ID_OBJECT_BIT_H
#define LIB_BASEAF_ID_OBJECT_BIT_H

#include "afIdBit.h"

namespace AFlib {
    class Storage;

    namespace id {
        class Object;
        class Storage;
        struct Object_bit;

        typedef Object_bit Obj_bit;
    }
}

//! \brief The Function class
//! \details Bit table, total size <b>28</b>
//! |<center>OBJECT BIT ID TABLE</center>|||||Size|Description|
//! |:-|-:|-:|-:|-:|-:||
//! | |      24|      16|       8|       0| | |
//! |*|########|########|####oooo|oooooooo|20|object unique id by current types|
//! |*|oooooooo|oooooooo|oooo####|###ooooo| 7|type of object|
//! |*|oooooooo|oooooooo|oooooooo|ooo#####| 5|plugin id, 0 - mean global variable|
struct AFlib::id::Object_bit : public TbitStruct <32>
{
    explicit Object_bit(quint32 id = 0);
    Object_bit(quint8 plugin, quint8 type);

    AFlib::id::Object_bit object_b() const;
    bool isIdLocal() const;
    bool isEmpty() const;

    static quint32 createLocalId();
    static quint32 createGlobalId();

    quint8  type()     const { return toUInt8 (40, 7); }
    quint32 id()       const { return toUInt32(); }
    quint32 uniqueId() const { return toUInt32(12, 20); }
    quint8  pluginId() const { return toUInt8 (35, 5); }

    template <typename N> N type  () const { return static_cast <N>(type()); }
    template <typename N> N plugin() const { return static_cast <N>(pluginId()); }

private:
    Object_bit(quint8 plugin, quint8 type, quint32 uid);

    void setType    (quint8 type) { setUInt8 (type, 40, 7); }
    void setId      (quint32 id)  { setUInt32(id); }
    void setUniqueId(quint32 id)  { setUInt32(id, 12, 20); }
    void setPluginId(quint8  id)  { setUInt8 (id, 35, 5); }

    template <typename N> void setType(N type) { setType(static_cast <quint8> (type)); }
    template <typename N> void setPlugin(N id)   { setPluginId(static_cast <quint8> (id)); }

    friend class AFlib::id::Object;
    friend class AFlib::Storage;
};

inline bool operator == (const AFlib::id::Object_bit& left, const AFlib::id::Object_bit& right)
{
    return left.id() == right.id();
}

#endif // LIB_BASEAF_ID_OBJECT_BIT_H
