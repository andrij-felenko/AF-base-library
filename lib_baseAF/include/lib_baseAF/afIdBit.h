#ifndef LIB_BASEAF_ID_BIT_H
#define LIB_BASEAF_ID_BIT_H

#include <bitset>
#include <iostream>
#include <cmath>
#include <QtCore/QDataStream>
#include <QtCore/QBuffer>
#include <QtCore/QDebug>
#include <QtCore/QSharedPointer>

namespace AFlib::id {
    template <uint bitsize>
    struct TbitStruct;
}

template <uint bsize>
QDataStream& operator << (QDataStream& s, const AFlib::id::TbitStruct <bsize> & data);

template <uint bsize>
QDataStream& operator >> (QDataStream& s,       AFlib::id::TbitStruct <bsize> & data);

template <uint bitsize>
struct AFlib::id::TbitStruct {
    uint bitSize() const { return bitsize; }

    template <uint bitsize_>
    std::bitset <bitsize_> getPiece(uint index, uint size) const
    {
        if (index >= bitsize){
            std::cerr << "Index is out of bitsize range.";
            return 0;
        }

//        if (size == 32)
//            std::cerr << "not 0";
        std::bitset <bitsize_> ret = 0;
        for (uint i = 0; i < size; i++)
            ret[i] = m_bitset[index + i];
        return ret;
    }

    quint8 getCharPiece(uint index) const
    {
        if (m_bitset.size() < index)
            return 0;

        quint8 ret = 0;
        for (uint i = index; i < std::min(index + 8, bitsize); i++)
            if (m_bitset[i])
                ret |= 1 << (i - index);

        return ret;
    }

    std::bitset <bitsize> getBitSet(uint index, uint size) const { return getPiece <bitsize> (index, size); }

    template <typename typeNumber>
    typeNumber toTypeNumber(uint index, uint size) const
    {
        if (sizeof (typeNumber) * 8 < size)
            std::cerr << "Size of type " << typeid(typeNumber).name() << " less than size " << size << " of need take.";

        qDebug() << m_bitset.to_ullong() << getBitSet(index, size).to_ullong();
        size = std::min(size, bitsize);
        return static_cast <typeNumber> (getBitSet(index, size).to_ullong());
    }

    template <uint fpart, uint spart>
    std::bitset <fpart + spart> unite(std::bitset <fpart> fp, std::bitset <spart> sp) const
    {
        return std::bitset <fpart + spart> (sp) |= fp << spart;
    }

    quint8  toUInt8 (uint index = 0, uint size =  8) const { return toTypeNumber <quint8 >(index, size); }
    quint16 toUInt16(uint index = 0, uint size = 16) const { return toTypeNumber <quint16>(index, size); }
    quint32 toUInt32(uint index = 0, uint size = 32) const { return toTypeNumber <quint32>(index, size); }
    quint64 toUInt64(uint index = 0, uint size = 64) const { return toTypeNumber <quint64>(index, size); }

    operator std::string () const { return toString(0); }
    std::string toString(char separator = 8, char ch = '`') const
    {
        std::cerr << "try to call toString function\n";
        std::string ret("");
        for (unsigned char i = 0; i * 8 < bitsize; i++)
             ret.push_back(getCharPiece(8 * i));

        if (separator > 0)
            for (unsigned char i = separator; separator < bitsize; i++, i += separator)
                ret.insert(i, 1, ch);

        std::cerr << "and it return " << ret.c_str() << "\n";

        return ret;
    }

    void fill(const std::string str)
    {
        m_bitset = 0;
        for (unsigned char i = 0; i * 8 < bitsize && str.length() > i; i++)
            setCharPiece(i * 8, str[i]);
    }

protected:
    std::bitset <bitsize> m_bitset;
    friend QDataStream& operator << <bitsize> (QDataStream& s, const TbitStruct <bitsize> & data);
    friend QDataStream& operator >> <bitsize> (QDataStream& s,       TbitStruct <bitsize> & data);

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

    void setCharPiece(uint index, quint8 ch)
    {
        if (index >= m_bitset.size()){
            std::cerr << "Index is out of bitsize range.";
            return;
        }

        setUInt8(ch, index, std::min(index + 8, bitsize));
    }

    template <typename typeNumber, uint size_>
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

template <uint bsize>
QDataStream& operator << (QDataStream& s, const AFlib::id::TbitStruct <bsize> & data)
{
    std::string str = data;
    uint len = str.length();
    s << len;

    for (int i = 0; i < len; i++)
        s << str[i];
    return s;
}

template <uint bsize>
QDataStream& operator >> (QDataStream& s, AFlib::id::TbitStruct <bsize> & data)
{
    uint len;
    s >> len;

    char ch;
    std::string str;

    for (uint i = 0; i < len; ++i){
        s >> ch;
        str.push_back(ch);
    }
    data.fill(str);

    return s;
}

template <typename T>
QDataStream& operator << (QDataStream& s, const std::vector <QSharedPointer <T>> & list){
    s << uint(list.size());
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&buffer);

    for (auto it = list.begin(); it != list.end(); ++it)
        stream << &it;

    buffer.close();
    return s << data;
}

template <typename T>
QDataStream& operator >> (QDataStream& s, std::vector <QSharedPointer <T>> & list){
    list.clear();
    quint32 count;
    s >> count;
    T obj;
    for (uint i = 0; i < count; i++){
        s >> obj;
        list.push_back(QSharedPointer <T>::create(obj));
    }
    return s;
}

#endif // LIB_BASEAF_ID_BIT_H
