#ifndef LIB_BASEAF_ID_BIT_H
#define LIB_BASEAF_ID_BIT_H

#include <bitset>
#include <iostream>
#include <QtCore/QDataStream>

namespace AFlib::id {
    template <size_t bitsize>
    struct TbitStruct;
}

template <size_t size>
QDataStream& operator << (QDataStream& stream, const std::bitset <size>& data){ return stream << data; }

template <size_t size>
QDataStream& operator >> (QDataStream& stream, std::bitset <size>& data){ return stream >> data; }

template <size_t bitsize>
struct AFlib::id::TbitStruct {
    template <size_t bitsize_>
    std::bitset <bitsize_> getPiece(uint index, size_t size) const
    {
        if (not m_bitset.test(index))
            return 0;

        std::bitset <bitsize_> ret = 0;
        for (uint i = 0; i < size; i++)
            ret[i] = m_bitset[index + i];
        return ret;
    }

    //    std::bitset <bitsize> (*getBitSet)(uint indedx, size_t size) = &getPiece;
    std::bitset <bitsize> getBitSet(uint index, size_t size) const { return getPiece <bitsize> (index, size); }

    template <typename typeNumber>
    typeNumber toTypeNumber(uint index, size_t size) const
    {
        if (sizeof (typeNumber) * 8 < size)
            std::cerr << "Size of type " << typeid(typeNumber).name() << " less than size " << size << " of need take.";

        size = std::min(size, bitsize);
        return static_cast <typeNumber> (getBitSet(index, size).to_ullong());
    }

    template <size_t fpart, size_t spart>
    std::bitset <fpart + spart> unite(std::bitset <fpart> fp, std::bitset <spart> sp) const
    {
        return std::bitset <fpart + spart> (sp) |= fp << spart;
    }

    quint8  toUInt8 (uint index = 0, uint size =  8) const { return toTypeNumber <quint8 >(index, size); }
    quint16 toUInt16(uint index = 0, uint size = 16) const { return toTypeNumber <quint16>(index, size); }
    quint32 toUInt32(uint index = 0, uint size = 32) const { return toTypeNumber <quint32>(index, size); }
    quint64 toUInt64(uint index = 0, uint size = 64) const { return toTypeNumber <quint64>(index, size); }

    friend QDataStream &operator << (QDataStream& stream, const TbitStruct& data){ return stream << data.m_bitset; }
    friend QDataStream &operator >> (QDataStream& stream,       TbitStruct& data){ return stream >> data.m_bitset; }

protected:
    std::bitset <bitsize> m_bitset;

    bool setBoolRange(quint8 iFrom, quint8 iTo, std::vector <bool> values)
    {
        if (iFrom >= bitsize)
            return false;

        for (quint8 i = 0; i < values.size(); i++){
            if (i + iFrom > iTo)
                return false;
            m_bitset.set(i + iFrom, values.at(i));
        }
        return true;
    }

    template <typename typeNumber, size_t size_>
    void setFromValue(typeNumber value, uint index, uint size)
    {
        std::bitset <size_> bits = value;
        if (size > size_)
            size = size_;

        for (uint i = 0; i < size; i++)
            m_bitset[index + i] = bits[i];
    }

    bool setReservedT(quint8 maxSize, quint8 iFrom, quint8 iTo, std::vector<bool> values)
    {
        if (iTo >= maxSize){
            std::cerr << "iTo must be in reserved range [0, 12], iTo = " << iTo;
            return false;
        }

        if (iTo <= iFrom){
            std::cerr << "iTo must be more than iFrom, iTo = " << iTo << ", iFrom = " << iFrom;
            return false;
        }
        return setBoolRange(iFrom, iTo, values);
    }

    TbitStruct <bitsize>& operator = (const quint8  value){ m_bitset = value; return *this; }
    TbitStruct <bitsize>& operator = (const quint16 value){ m_bitset = value; return *this; }
    TbitStruct <bitsize>& operator = (const quint32 value){ m_bitset = value; return *this; }
    TbitStruct <bitsize>& operator = (const quint64 value){ m_bitset = value; return *this; }

    void setUInt8 (quint8  value, uint index = 0, uint size = bitsize){ setFromValue <quint8,   8> (value, index, size); }
    void setUInt16(quint16 value, uint index = 0, uint size = bitsize){ setFromValue <quint16, 16> (value, index, size); }
    void setUInt32(quint32 value, uint index = 0, uint size = bitsize){ setFromValue <quint32, 32> (value, index, size); }
    void setUInt64(quint64 value, uint index = 0, uint size = bitsize){ setFromValue <quint64, 64> (value, index, size); }
};

#endif // LIB_BASEAF_ID_BIT_H
