#ifndef LIB_BASEAF_ID_H
#define LIB_BASEAF_ID_H

#include <QtCore/QDataStream>
#include <QtCore/QDateTime>
#include <QtCore/QPair>
#include <QtCore/QSharedPointer>
#include <QtCore/QVariant>

#include <algorithm>
#include <bitset>
#include <iostream>
#include <memory>

#include "enumAF.h"

namespace AFlib {
    namespace id {
        template <size_t bitsize>
        struct TbitStruct;

        struct Account_bit;
        struct Operate_bit;
        struct Subject_bit;
        struct SubjectFull_bit;

        class Info;
        class History;
        class Operate;
        class Subject;

        typedef QSharedPointer <Subject> SubjectPtr;
        typedef QList <SubjectPtr> SubjectPtrList;

        typedef QSharedPointer <Operate> OperatePtr;
        typedef QList <OperatePtr> OperatePtrList;

        typedef Account_bit Acc_bit;
        typedef Subject_bit Sub_bit;
        typedef QSharedPointer <History> HistoryPtr;
    }
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

    quint8  toUInt8 (uint index = 0, uint size =  8) const { return toTypeNumber <quint8 >(index, size); }
    quint16 toUInt16(uint index = 0, uint size = 16) const { return toTypeNumber <quint16>(index, size); }
    quint32 toUInt32(uint index = 0, uint size = 32) const { return toTypeNumber <quint32>(index, size); }
    quint64 toUInt64(uint index = 0, uint size = 64) const { return toTypeNumber <quint64>(index, size); }

    void setUInt8 (quint8  value, uint index = 0, uint size = bitsize){ setFromValue <quint8,   8> (value, index, size); }
    void setUInt16(quint16 value, uint index = 0, uint size = bitsize){ setFromValue <quint16, 16> (value, index, size); }
    void setUInt32(quint32 value, uint index = 0, uint size = bitsize){ setFromValue <quint32, 32> (value, index, size); }
    void setUInt64(quint64 value, uint index = 0, uint size = bitsize){ setFromValue <quint64, 64> (value, index, size); }

    friend QDataStream &operator << (QDataStream& stream, const TbitStruct& data){ return stream << data.m_bitset; }
    friend QDataStream &operator >> (QDataStream& stream,       TbitStruct& data){ return stream >> data.m_bitset; }

protected:
    std::bitset <bitsize> m_bitset;
};

/* ________ACCOUNT_ID_Bit_________________
 *
 *   28   24   20   16   12    8    4    0
 *    |    |    |    |    |    |    |    |
 * 0000 0000 0000 0000 00xx xxxx xxxx xxxx - 14: local user
 * 0000 0000 0000 0000 xxxx xxxx xxxx xxxx - 16: local organization
 * 0000 xxxx xxxx xxxx xxxx xxxx xxxx xxxx - 28: global user
 * xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx - 32: global organization
 * .... .... .... .... .... .... .... .... */
struct AFlib::id::Account_bit : public TbitStruct <32>
{
    explicit Account_bit(quint32 id = 0) { setUserId(id); }

    quint32 accountId()     const { return toUInt32(    0, 32); }
    void setAccountId(quint32 id) {       setUInt32(id, 0, 32); }
    void setUserId   (quint32 id) {
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

    static Account_bit create(const AccountIdType &type)
    {
        quint32 value = rand() % (1 << 14);
        if (!(type != AccountIdType::LocalUser))
            return Account_bit(value);
        value += (rand() % 3 + 1) << 14;
        if (!(type != AccountIdType::LocalOrganization))
            return Account_bit(value);
        value += (rand() % ((1 << 12) - 1) + 1) << 16;
        if (!(type != AccountIdType::GlobalUser))
            return Account_bit(value);
        value += (rand() % 15 + 1) << 28;
        return Account_bit(value);
    }

    //    friend QDataStream &operator << (QDataStream& stream, const Account_bit& data);
    //    friend QDataStream &operator >> (QDataStream& stream,       Account_bit& data);
};

/* ________SUBJECT_ID_Bit_________
 *
 *   24       16        8        0
 *    |        |        |        |
 * #### ######## ####.... ........ 16: subject unique id by current types
 * .... ........ ....#### ###.....  7: type of subject
 * .... ........ ........ ...#####  5: plugin id [0 - mean global variable]
 * .... ........ ........ ........ */
struct AFlib::id::Subject_bit : public TbitStruct <28>
{
    explicit Subject_bit(quint32 subjectFull = 0);
    Subject_bit(quint16 uniqueId, quint8 type, quint8 pluginId);

    quint32 id()     const { return toUInt32(  ); }
    void setId(quint32 id) {       setUInt32(id); }

    quint32 uniqueId()     const { return toUInt16(    12, 16); }
    void setUniqueId(quint16 id) {       setUInt16(id, 12, 16); }

    quint32 type()      const { return toUInt8(      5, 7); }
    void setType(quint8 type) {       setUInt8(type, 5, 7); }

    quint32 pluginId()    const { return toUInt8(    0, 5); }
    void setPluginId(quint8 id) {       setUInt8(id, 0, 5); } 
};

/* _________________________SUBJECT_FULL_ID_Bit__________________________
 * |_______[28]_SUBJECT_ID_______|
 * 62   56       48       40       32       24       16        8        0
 * |     |        |        |        |        |        |        |        |
 * ####### ######## #....... ........ ........ ........ ........ ........ 16: subject unique id by current types
 * ....... ........ .####### ........ ........ ........ ........ ........  7: type of subject
 * ....... ........ ........ #####... ........ ........ ........ ........  5: plugin id [0 - mean global variable]
 * ....... ........ ........ .....### ####.... ........ ........ ........  7: parent subject type
 * ....... ........ ........ ........ ....#### ######## ######## ######## 28: parent id [use prev subject or in account id on top]
 * ....... ........ ........ ........ ........ ........ ........ ........ */
struct AFlib::id::SubjectFull_bit : public TbitStruct <63>
{
    explicit SubjectFull_bit(quint64 subjectFull = 0);
    SubjectFull_bit(quint32 subjectId, quint8 parentType, quint32 parentId);
    SubjectFull_bit(Subject_bit subject, quint8 parentType, quint32 parentId);

    Sub_bit id_b()   const { return Sub_bit(id()); }
    void setId(Sub_bit id) {      setId( id.id()); }

    quint32 id()     const { return toUInt32(    35, 28); }
    void setId(quint32 id) {       setUInt32(id, 35, 28); }

    quint32 uniqueId()     const { return toUInt16(    47, 16); }
    void setUniqueId(quint16 id) {       setUInt16(id, 47, 16); }

    quint32 type()      const { return toUInt8(      40, 7); }
    void setType(quint8 type) {       setUInt8(type, 40, 7); }

    quint32 pluginId()    const { return toUInt8(    35, 5); }
    void setPLuginId(quint8 id) {       setUInt8(id, 35, 5); }

    quint32 parentType()      const { return toUInt8(      28, 7); }
    void setParentType(quint8 type) {       setUInt8(type, 28, 7); }

    quint32 parentId()     const { return toUInt32(    0, 28); }
    void setParnetId(quint32 id) {       setUInt32(id, 0, 28); }

    void setParent(quint8 type, quint32 id);
};

/* ___________________________OPERATE_ID_Bit____________________________
 *                                       |_______[28]_SUBJECT_ID_______|
 * 61  56       48       40       32       24       16        8        0
 * |    |        |        |        |        |        |        |        |
 * ###### ######## ######## ######.. ........ ........ ........ ........ 28: user id whose make transaction
 * ...... ........ ........ ......## #....... ........ ........ ........  3: history enum variable
 * ...... ........ ........ ........ .###.... ........ ........ ........  3: save enum variable
 * ...... ........ ........ ........ ....#### ######## ####.... ........ 16: subject unique id by current types
 * ...... ........ ........ ........ ........ ........ ....#### ###.....  7: type of subject
 * ...... ........ ........ ........ ........ ........ ........ ...#####  5: plugin id [0 - mean global variable]
 * ...... ........ ........ ........ ........ ........ ........ ........ */
struct AFlib::id::Operate_bit : public TbitStruct <62>
{
    quint32 userId()     const { return toUInt32(    34, 28); }
    void setUserId(quint32 id) {       setUInt32(id, 34, 28); }
    void setUserId(Acc_bit id) {       setUserId(id.accountId()); }

    HIdType historyType()       const { return toHistoryIdType(toUInt8(      31, 3)); }
    void setHistoryType(quint8  type) {                       setUInt8(type, 31, 3); }
    void setHistoryType(HIdType type) {   setHistoryType(fromHisToInt (type)); }

    SIdType saveType()       const { return toSavedIdType(toUInt8(      28, 3)); }
    void setSaveType(quint8  type) {                     setUInt8(type, 28, 3); }
    void setSaveType(SIdType type) {   setSaveType(fromSaveToInt (type)); }

    Sub_bit subjectId_b()   const { return Sub_bit(subjectId()); }
    void setSubjectId(Sub_bit id) { setSubjectId(id.id()); }

    quint32 subjectId()     const { return toUInt32(    0, 28); }
    void setSubjectId(quint32 id) {       setUInt32(id, 0, 28); }

    quint32 uniqueId()     const { return toUInt16(    12, 16); }
    void setUniqueId(quint16 id) {       setUInt16(id, 12, 16); }

    quint32 type()      const { return toUInt8(      5, 7); }
    void setType(quint8 type) {       setUInt8(type, 5, 7); }

    quint32 pluginId()    const { return toUInt8(    0, 5); }
    void setPLuginId(quint8 id) {       setUInt8(id, 0, 5); }
};

class AFlib::id::Info : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name        READ name        WRITE setName        NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
public:
    explicit Info(QObject* parent = nullptr);
    Info(QString name, QString descr, QObject* parent = nullptr);

    virtual QString name()        const final { return m_name; }
    virtual QString description() const final { return m_description; }

public slots:
    void setName(QString name);
    void setDescription(QString description);

protected:
    QString m_name;
    QString m_description;

    friend QDataStream &operator << (QDataStream& stream, const Info& data);
    friend QDataStream &operator >> (QDataStream& stream,       Info& data);

signals:
    void nameChanged(QString name);
    void descriptionChanged(QString description);
};

class AFlib::id::History //: public QObject
{
//    Q_OBJECT
public:
    explicit History(/*QObject* parent = nullptr*/);

    void updateTime(QDateTime dTime);
    bool haveUpdates() const;
    QDateTime lastChange() const;
    QDateTime lastUpdate() const;

    void addOperation(OperatePtr id);
    std::optional <OperatePtr> getLastOperation() const;

protected:
    QDateTime m_lastUpdate;

    friend QDataStream &operator << (QDataStream& stream, const History& data);
    friend QDataStream &operator >> (QDataStream& stream,       History& data);

//signals:
//    void needUpdate();
//    void changed();

private:
    OperatePtrList m_historyList;
};

class AFlib::id::Subject : public Info, public SubjectFull_bit
{
public:
    explicit Subject(QObject* parent = nullptr);
    Subject(quint16 uniqueId, quint8 type, quint8 pluginId, QObject* parent = nullptr);
    Subject(QString name, QString descr = QString(""), QObject* parent = nullptr);
    Subject(QString name, QString descr, quint16 uniqueId, quint8 type, quint8 pluginId,
            quint8 parentType, quint32 parentId,
            QObject* parent = nullptr);

    HistoryPtr history() const;

protected:
    friend QDataStream &operator << (QDataStream& stream, const Subject& data);
    friend QDataStream &operator >> (QDataStream& stream,       Subject& data);

private:
    HistoryPtr m_history;
};

QDataStream &operator << (QDataStream& stream, const AFlib::id::SubjectPtrList& data);
QDataStream &operator >> (QDataStream& stream,       AFlib::id::SubjectPtrList& data);

class AFlib::id::Operate : public Info, public Operate_bit
{
public:
    Operate(quint32 subjectId, quint32 userId, quint32 ownerId, HistoryIdType historyId,
            SavedIdType saved = SavedIdType::LocaleSaved,
            QDateTime dTime = QDateTime::currentDateTime());

    Operate(quint32 subjectId, quint32 userId, quint32 ownerId, quint8 historyId,
            quint8 saved = quint8(SavedIdType::LocaleSaved),
            QDateTime dTime = QDateTime::currentDateTime());

    explicit Operate(Subject_bit subjectId = Subject_bit(),
                     Account_bit userId = Account_bit(),
                     Account_bit ownerId = Account_bit(),
                     HistoryIdType history = HistoryIdType::First,
                     SavedIdType saved = SavedIdType::LocaleSaved,
                     QDateTime dTime = QDateTime::currentDateTime());

    Account_bit owner() const { return m_owner; }
    void setOwner(const Account_bit &owner);

    QDateTime datetime() const { return  m_datetime; }
    void setDatetime(const QDateTime &datetime);

    QVariant value() const { return m_value; }
    void setValue(const QVariant &value);

protected:
    friend QDataStream &operator << (QDataStream& stream, const Operate& data);
    friend QDataStream &operator >> (QDataStream& stream,       Operate& data);

private:
    Account_bit m_owner;
    QDateTime m_datetime;
    QVariant m_value;
};

QDataStream &operator << (QDataStream& stream, const AFlib::id::OperatePtrList& data);
QDataStream &operator >> (QDataStream& stream,       AFlib::id::OperatePtrList& data);

#endif // LIB_BASEAF_ID_H
