#include "afIdGlobalBit.h"
#include "AfFunction"

AFlib::id::Global_bit::Global_bit(Account_bit owner, Object_bit object)
    : ownerId(owner), objectId(object)
{
    //
}

AFlib::id::Global_bit::Global_bit(quint64 number)
    : ownerId(number >> 32), objectId(number % (quint64(1) << 32))
{
    //
}

quint64 AFlib::id::Global_bit::toNumber() const
{
    return (ownerId.toUInt64() << 32) + objectId.toUInt32();
}

namespace AFlib::id {
    QDataStream &operator <<(QDataStream &s, const Global_bit &id)
    {
        return s << id.ownerId << id.objectId;
    }

    QDataStream &operator >>(QDataStream &s, Global_bit &id)
    {
        return s >> id.objectId >> id.objectId;
    }
}

QDebug operator <<(QDebug d, const AFlib::id::Global_bit &object)
{
    return d << "AFlib::id::Account_bit: 0x" << QString::number(object.ownerId.toUInt32(), 16)
             << &object.objectId;
}
