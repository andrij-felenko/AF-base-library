#ifndef LIB_BASEAF_ID_GLOBAL_BIT_H
#define LIB_BASEAF_ID_GLOBAL_BIT_H

#include "afIdBit.h"
#include "afIdAccount.h"
#include "afIdObjectBit.h"

namespace AFlib {
    class Storage;

    namespace id {
        struct Global_bit;

        typedef QList <Global_bit> GlobalList_bit;
    }
}

struct AFlib::id::Global_bit
{
    Account_bit ownerId;
    Object_bit objectId;

    Global_bit(Account_bit owner, Object_bit object)
        : ownerId(owner), objectId(object) {}
    Global_bit(quint64 number)
        : ownerId(number >> 32), objectId(number % (quint64(1) << 32)) {}

    quint64 toNumber() const;

    friend inline bool operator == (const Global_bit& l, const Global_bit& r){
        return l.ownerId == r.ownerId && l.objectId == r.objectId;
    }

    friend QDataStream& operator << (QDataStream& s, const Global_bit& id);
    friend QDataStream& operator >> (QDataStream& s,       Global_bit& id);
};

QDebug operator << (QDebug d, const AFlib::id::Global_bit& object);

#endif // LIB_BASEAF_ID_OBJECT_BIT_H
