#ifndef LIB_BASEAF_ID_H
#define LIB_BASEAF_ID_H

#include <QtCore/QDataStream>
#include <QtCore/QDateTime>
#include <bitset>
#include <iostream>
#include <memory>
#include "enumAF.h"

namespace AFlib {
template <size_t bitsize>
struct TbitStruct;

struct AccountId;
struct SubjectId;
struct TransactionId;

class HistoryId;
}

template <size_t size>
QDataStream& operator << (QDataStream& stream, const std::bitset <size>& data){ return stream << data; }

template <size_t size>
QDataStream& operator >> (QDataStream& stream, std::bitset <size>& data){ return stream >> data; }

template <size_t bitsize>
struct AFlib::TbitStruct {
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
    typeNumber toTypeNumber(uint index, uint size) const
    {
        if (sizeof (typeNumber) * 8 < size)
            std::cerr << "Size of type " << typeid(typeNumber).name() << " less than size " << size << " of need take.";

        return static_cast <typeNumber> (getBitSet(index, size).to_ullong());
    }

    template <size_t fpart, size_t spart>
    std::bitset <fpart + spart> unite(std::bitset <fpart> fp, std::bitset <spart> sp) const
    {
        return std::bitset <fpart + spart> (sp) |= fp << spart;
    }

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

//    quint8  (*toUInt8 )(uint index, uint size) = &toTypeNumber <quint8>;
//    quint16 (*toUInt16)(uint index, uint size) = &toTypeNumber <quint16>;
//    quint32 (*toUInt32)(uint index, uint size) = &toTypeNumber <quint32>;
//    quint64 (*toUInt64)(uint index, uint size) = &toTypeNumber <quint64>;

    quint8  toUInt8 (uint index = 0, uint size =  8) const { return toTypeNumber <quint8 >(index, size); }
    quint16 toUInt16(uint index = 0, uint size = 16) const { return toTypeNumber <quint16>(index, size); }
    quint32 toUInt32(uint index = 0, uint size = 32) const { return toTypeNumber <quint32>(index, size); }
    quint64 toUInt64(uint index = 0, uint size = 64) const { return toTypeNumber <quint64>(index, size); }

//    quint8  (*setUInt8 )(quint8  value, uint index, uint size) = &setFromValue <quint8,   8>;
//    quint16 (*setUInt16)(quint16 value, uint index, uint size) = &setFromValue <quint16, 16>;
//    quint32 (*setUInt32)(quint32 value, uint index, uint size) = &setFromValue <quint32, 32>;
//    quint64 (*setUInt64)(quint64 value, uint index, uint size) = &setFromValue <quint64, 64>;

    void setUInt8 (quint8  value, uint index, uint size){ setFromValue <quint8,   8> (value, index, size); }
    void setUInt16(quint16 value, uint index, uint size){ setFromValue <quint16, 16> (value, index, size); }
    void setUInt32(quint32 value, uint index, uint size){ setFromValue <quint32, 32> (value, index, size); }
    void setUInt64(quint64 value, uint index, uint size){ setFromValue <quint64, 64> (value, index, size); }

    friend QDataStream &operator << (QDataStream& stream, const TbitStruct& data){ return stream << data.m_bitset; }
    friend QDataStream &operator >> (QDataStream& stream,       TbitStruct& data){ return stream >> data.m_bitset; }

//protected:
    std::bitset <bitsize> m_bitset;
};


///   28   24   20   16   12    8    4    0
///    |    |    |    |    |    |    |    |
/// bbbb bbbb bbbb bbbb bbbb bbbb bbbb bbbb
/// .... .... .... .... .... .... .... .||| - reserved data
/// .... .... .... .... .... .... ..|| |... - reserved type
/// .... .... .... .... .... ..|| ||.. .... - plugin type
/// .... .... .... .... |||| ||.. .... .... - subject type
/// |||| |||| |||| |||| .... .... .... .... - id
/// .... .... .... .... .... .... .... ....
struct AFlib::SubjectId : public TbitStruct <32>
{
    SubjectId(quint32 value);
    quint32 globalId(){ return toUInt32(6, 26); }

    std::bitset <3> reserved() const { return getPiece <3> (0, 3); }
    void setReserved(quint8 value) { setUInt8(value, 0, 3); }
    void setReserved(quint8 index, bool value) { m_bitset.set(index, value); }
    bool setReserved(quint8 iFrom, quint8 iTo, std::vector <bool> values) {
        return setReservedT(3, iFrom, iTo, values);
    }

    quint8  reserveType()       const { return toUInt8       (3, 3); }
    void setReserveType(quint8 value) {       setUInt8(value, 3, 3); }

    quint8  pluginId()       const { return toUInt8       (6, 4); }
    void setPluginId(quint8 value) {       setUInt8(value, 6, 4); }

    quint8  subjectType()       const { return toUInt8       (10, 6); }
    void setSubjectType(quint8 value) {       setUInt8(value, 10, 6); }

    quint16 id()        const { return toUInt16       (16, 16); }
    void setId(quint16 value) {       setUInt16(value, 16, 16); }
};


///   28   24   20   16   12    8    4    0
///    |    |    |    |    |    |    |    |
/// bbbb bbbb bbbb bbbb bbbb bbbb bbbb bbbb
/// |||| |||| |||| |||| |||| |||| |||| |||| - account id
/// 0000 0000 0000 0000 00xx xxxx xxxx xxxx - local user
/// 0000 0000 0000 0000 xxxx xxxx xxxx xxxx - local organization
/// 0000 xxxx xxxx xxxx xxxx xxxx xxxx xxxx - global user
/// xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx.- global organization
/// .... .... .... .... .... .... .... .... .... .... .... ....
struct AFlib::AccountId : public TbitStruct <32>
{
    explicit AccountId(quint32 id = 0);

    quint32 accountId()     const { return toUInt32(    0, 32); }
    void setAccountId(quint32 id) {       setUInt32(id, 0, 32); }
    void setGlobalUser(quint32 id) {
        m_bitset = 0;
        m_bitset |= id % (1 << 28);
    }

    AccountIdType accountType() {
        quint32 accId = accountId();
        if (accId >> 14 == 0)
            return AccountIdType::LocalUser;
        if (accId >> 16 == 0)
            return AccountIdType::LocalOrganization;
        if (accId >> 28 == 0)
            return AccountIdType::GlobalUser;
        return AccountIdType::GlobalOrganization;
    }

    static AccountId create(const AccountIdType &type);
};


/// |______[32]_owner-account-Id______| |_________[32]_date-time__________| |________[28]_user-Id_________| |______[26]_subject-Id______|
///      120      112      104       96       88       80       72       64       56       48       40   36   32       24       16     10          0
///        |        |        |        |        |        |        |        |        |        |        |    |    |        |        |      |          |
/// bbbbbbbb bbbbbbbb bbbbbbbb bbbbbbbb bbbbbbbb bbbbbbbb bbbbbbbb bbbbbbbb bbbbbbbb bbbbbbbb bbbbbbbb bbbb bbbb bbbbbbbb bbbbbbbb bbbbbb bbbbbbbbbb
/// ........ ........ ........ ........ ........ ........ ........ ........ ........ ........ ........ .... .... ........ ........ ...... .......||| - reserved
/// ........ ........ ........ ........ ........ ........ ........ ........ ........ ........ ........ .... .... ........ ........ ...... ....|||... - type of reserved
/// ........ ........ ........ ........ ........ ........ ........ ........ ........ ........ ........ .... .... ........ ........ ...... ..||...... - saved type
/// ........ ........ ........ ........ ........ ........ ........ ........ ........ ........ ........ .... .... ........ ........ ...... ||........ - history type
/// ........ ........ ........ ........ ........ ........ ........ ........ ........ ........ ........ .... |||| |||||||| |||||||| |||||| .......... - subject Id
/// ........ ........ ........ ........ ........ ........ ........ ........ |||||||| |||||||| |||||||| |||| .... ........ ........ ...... .......... - user Id
/// ........ ........ ........ ........ |||||||| |||||||| |||||||| |||||||| ........ ........ ........ .... .... ........ ........ ...... .......... - date time
/// |||||||| |||||||| |||||||| |||||||| ........ ........ ........ ........ ........ ........ ........ .... .... ........ ........ ...... .......... - owner account Id
/// ........ ........ ........ ........ ........ ........ ........ ........ ........ ........ ........ .... .... ........ ........ ...... ..........
struct AFlib::TransactionId : public TbitStruct <128>
{
    // FIXME add constructor
    TransactionId();

    std::bitset <3> reserved() const { return getPiece <3> (0, 3); }
    void setReserved(quint8 value) { setUInt8(value, 0, 3); }
    void setReserved(quint8 index, bool value) { m_bitset.set(index, value); }
    bool setReserved(quint8 iFrom, quint8 iTo, std::vector <bool> values) {
        return setReservedT(3, iFrom, iTo, values);
    }

    void setReserveType(quint8 value) {       setUInt8(value, 3, 3); }
    quint8  reserveType()       const { return toUInt8       (3, 3); }

    void setSavedType(quint8 value) {       setUInt8(value % 0x100, 6, 2); }
    quint8  savedTypeInt()    const { return toUInt8               (6, 2); }
    SavedIdType savedType()   const { return toSavedIdType(toUInt8 (6, 2)); }

    void setHistoryType(quint8 value) {       setUInt8(value % 0x100,  8, 2); }
    quint8  historyTypeInt()    const { return toUInt8                (8, 2); }
    HistoryIdType historyType() const { return toHistoryIdType(toUInt8(8, 2)); }

    void setSubjectId(quint32 value) {       setUInt32(value, 10, 26); }
    quint32 subjectId()        const { return toUInt32       (10, 26); }
    SubjectId subjectObject()  const { return SubjectId(subjectId()); }
    void setSubjectObj(SubjectId so) { setSubjectId(so.globalId()); }

    void setUserId(quint32 value) {       setUInt32(value, 36, 28); }
    quint32  userId()       const { return toUInt32       (36, 28); }
    AccountId userObject()  const { return AccountId(userId()); }
    void setUserObj(AccountId ai) { setUserId(ai.toUInt32(0, 28)); }

    void setDateTime(QDateTime dTime) {           setUInt32(dTime.toTime_t(), 64, 32); }
    QDateTime  dateTime()       const { return QDateTime::fromTime_t(toUInt32(64, 32)); }

    void setOwnerId(quint32 value) {       setUInt32(value, 96, 32); }
    quint32  ownerId()       const { return toUInt32       (96, 32); }
    AccountId ownerObject()  const { return AccountId(ownerId()); }
    void setOwnerObj(AccountId ai) { setOwnerId(ai.toUInt32()); }
};

class AFlib::HistoryId : public QObject
{
    Q_OBJECT
public:
    explicit HistoryId(QObject* parent = nullptr);

    void change(QDateTime dTime);
    void update(QDateTime dTime);
    bool haveUpdates() const;
    QDateTime lastUpdate() const;

protected:
    QDateTime m_lastChange;
    QDateTime m_lastUpdate;

    friend QDataStream &operator << (QDataStream& stream, const HistoryId& data){ return stream << data.m_lastChange << data.m_lastUpdate; }
    friend QDataStream &operator >> (QDataStream& stream,       HistoryId& data){ return stream >> data.m_lastChange >> data.m_lastUpdate; }

signals:
    void needUpdate();
};

#endif // LIB_BASEAF_ID_H
