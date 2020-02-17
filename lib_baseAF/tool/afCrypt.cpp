#include "crypt.h"
#include <bitset>
#include <cmath>

const uint KEYSIZE = 6;

template <size_t size>
void flipData(std::bitset<size> &data)
{
    for (uint i = 0; i < size / 3; i++)
        if ((i % 2 == 0 && !data[i*3]) || (i % 2 == 1 && data[i*3])){
            data.flip(i*3 + 1);
            data.flip(i*3 + 2);
        }
}

template <size_t size>
void swapBit(std::bitset <size> &bit, uint b1, uint b2)
{
    bool tmp(bit[b1]);
    bit.set(b1, bit[b2]);
    bit.set(b2, tmp);
}

// work with none replace key
template <size_t size, size_t size_key>
void addKey(std::bitset<size> &bit, uint key)
{
    std::bitset <size_key> keySet(key);

    for (uint i = 0; i < size_key; i++){
        bit.set(size - 2 - i*2, bit[size - size_key - 1 - i]);
        bit.set(size - 1 - i*2, keySet[size_key - 1 - i]);
    }
}

template <class T>
uint createKey(uint &key, T number, uint prop)
{
    number += key;
    while (number != 0){
        key += number % prop;
        key %= 1 << KEYSIZE;
        number /= prop;
    }
    return key;
}

/// It work only if key already set into data
template <size_t size, size_t size_key>
void replaceKey(std::bitset <size> &data, bool crypt)
{
    // check data %2 == 0 for true
    uint posKey(0);
    for (uint i = 0; i < size / 2; i++)
        if (data[i*2])
            posKey++;
    posKey %= size - size_key - 1;
    posKey++;

    // create position vector
    std::vector <uint> posV;
    for (uint i = 0; i < size_key; i++)
        posV.push_back(uint(std::pow(posKey, i*3)) % size / 3 + 1);

    if (crypt)
        for (uint i = 0; i < size_key; i++)
            data.flip(posV[i] * 2 + 1);

    for (uint i = 0; i < size_key; i++)
        swapBit <size> (data, size - 1 - i*2, posV[i] * 2 + 1);

    if (!crypt)
        for (uint i = 0; i < size_key; i++)
            data.flip(posV[i] * 2 + 1);
}

// work with none replace key
template <size_t size, size_t size_key>
uint separateKey(std::bitset<size> &data)
{
    std::bitset <size_key> key;
    for (uint i = 0; i < size_key; i++)
        key.set(i, data[size - 1 - i*2]);

    for (uint i = 0; i < size_key - 1; i++)
        data.set(size - 11 + i, data[size - 10 + i*2]);

    data <<= size_key;
    data >>= size_key;
    return uint(key.to_ulong());
}

template <class T>
void m_toCrypt(T &number)
{
    auto const size = sizeof (T) * 8;
    std::bitset <size> data(number);

    // create key
    uint key(0);
    createKey <T> (key, number, 2);
    createKey <T> (key, number, 3);
    createKey <T> (key, number, 16);
    createKey <T> (key, number, size);

    addKey     <size, KEYSIZE> (data, key);
    replaceKey <size, KEYSIZE> (data, true);
    flipData   <size> (data);

    number = T(data.to_ullong());
}

template <class T>
T m_getCrypt(T number)
{
    T data = number;
    m_toCrypt <T> (data);
    return data;
}

template <class T>
void m_toEncrypt(T &value)
{
    auto const size = sizeof (T) * 8;
    std::bitset <size> data(value);

    flipData    <size> (data);
    replaceKey  <size, KEYSIZE> (data, false);
    separateKey <size, KEYSIZE> (data);

    value = T(data.to_ullong());
}

template <class T>
T m_getEncrypt(T value)
{
    T data = value;
    m_toEncrypt <T> (data);
    return data;
}

QByteArray AFlib::crypt(QByteArray array)
{
    QByteArray data;
    QByteArray part;
    for (uint i = 0; i < uint(array.length()); i++){
        part[i % 8] = array[i];
        if (i % 8 == 0 && i > 0){
            data += QByteArray::number(getCrypt(part.toULongLong()));
            part.clear();
        }
    }
    return data;
}

QByteArray AFlib::encrypt(QByteArray array)
{
    QByteArray data;
    QByteArray part;
    for (uint i = 0; i < uint(array.length()); i++){
        part[i % 8] = array[i];
        if (i % 8 == 0 && i > 0){
            data += QByteArray::number(getEncrypt(part.toULongLong()));
            part.clear();
        }
    }
    return data;
}

void AFlib::toCrypt(uint &data)
{
    m_toCrypt <uint> (data);
}

void AFlib::toCrypt(quint64 &data)
{
    m_toCrypt <quint64> (data);
}

void AFlib::toEncrypt(uint &data)
{
    m_toEncrypt <uint> (data);
}

void AFlib::toEncrypt(quint64 &data)
{
    m_toEncrypt <quint64> (data);
}

uint AFlib::getCrypt(uint data)
{
    return m_getCrypt <uint> (data);
}

uint AFlib::getEncrypt(uint data)
{
    return m_getEncrypt <uint> (data);
}

quint64 AFlib::getCrypt(quint64 data)
{
    return m_getCrypt <quint64> (data);
}

quint64 AFlib::getEncrypt(quint64 data)
{
    return m_getEncrypt <quint64> (data);
}

uint AFlib::codeString(QString str, uint size)
{
    uint coef = 1;
    uint data = 0;
    uint min = 255;
    for (auto it : str)
        if (uint(it.toLatin1()) < min)
            min = uint(it.toLatin1());
    min++;

    for (auto it : str){
        data += (uint(it.toLatin1()) - min) * coef;
        coef *= 16;
    }
    return data %= uint(1 << size);
}
