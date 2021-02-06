#ifndef LIB_BASEAF_ID_OBJECT_BIT_H
#define LIB_BASEAF_ID_OBJECT_BIT_H

#include "afIdBit.h"
#include "afIdAccount.h"
#include <QtCore/QDebug>

namespace AFlib {
    class Storage;

    namespace id {
        class Object;
        class Storage;
        struct Object_bit;

        typedef Object_bit Obj_bit;
    }
}

// TODO learn how it works
//enum ObjectEnum {
//    Unique,
//    Type,
//    PLugin
//};

//template <typename Enum, uint bitsize, typename... args>
//class BitTemplate : AFlib::id::TbitStruct <bitsize>
//{
//    // Enum, uint f_index, uint size
//    template <typename Ret>
//    Ret get(Enum e)
//    {
//        return p_get <Ret>(e, args...);
//    }

//private:
//    template <typename Ret>
//    Ret p_get(Enum needed, Enum current, uint firstIndex, uint size,
//            auto... list)
//    {
//        if (needed == current)
//            return this.template toTypeNumber <Ret> (firstIndex, size);
//        if (sizeof...(list) > 3)
//            return p_get(needed, list...);
//        return 0;
//    }
//};

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

    quint8  type()     const {  auto r = toUInt8 (5, 7); qDebug() << "type:" << r; return r;}
    quint32 id()       const {  auto r = toUInt32(0, 32); qDebug() << "id:" << r; return r; }
    quint32 uniqueId() const {  auto r = toUInt32(12, 20); qDebug() << "uid:" << r; return r; }
    quint8  pluginId() const {  auto r = toUInt8 (0, 5); qDebug() << "plugin:" << r; return r; }

    template <typename N> N type  () const { return static_cast <N>(type()); }
    template <typename N> N plugin() const { return static_cast <N>(pluginId()); }

private:
    Object_bit(quint8 plugin, quint8 type, quint32 uid);

    void setType    (quint8 type) { setUInt8 (type, 5, 7); }
    void setId      (quint32 id)  { setUInt32(id); }
    void setUniqueId(quint32 id)  { setUInt32(id, 12, 20); }
    void setPluginId(quint8  id)  { setUInt8 (id, 0, 5); }

    void setId(Object_bit id)  { *this = id; }

    template <typename N> void setType(N type) { setType(static_cast <quint8> (type)); }
    template <typename N> void setPlugin(N id)   { setPluginId(static_cast <quint8> (id)); }

    friend class AFlib::id::Object;
    friend class AFlib::Storage;

    friend QDataStream& operator << (QDataStream& s, const Object_bit& obj){
        return s << static_cast <const TbitStruct <32>> (obj);
    }
    friend QDataStream& operator >> (QDataStream& s,       Object_bit& obj){
        TbitStruct <32> bits;
        s >> bits;
        obj.setId(bits.toUInt32());
        return s;
    }
};

QDebug operator << (QDebug d, const AFlib::id::Object_bit& obj_b);
inline bool operator == (const AFlib::id::Object_bit& left, const AFlib::id::Object_bit& right)
{
    return left.id() == right.id();
}

#endif // LIB_BASEAF_ID_OBJECT_BIT_H
