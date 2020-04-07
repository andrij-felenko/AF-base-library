#ifndef LIB_BASEAF_ID_OPERATE_H
#define LIB_BASEAF_ID_OPERATE_H

#include <QtCore/QDateTime>
#include <QtCore/QVariant>
#include <QtCore/QSharedPointer>

#include "afEnum.h"
#include "afIdAccount.h"

namespace AFlib::id {
    class Operate;
    struct Operate_bit;
    struct TransferOperate;
    struct TransferOperateList;

    typedef QSharedPointer <Operate> OperatePtr;
    typedef QList <OperatePtr> OperatePtrList;
}

/* ___________________________OPERATE_ID_Bit____________________________
 *
 * 47    40       32       24       16        8        0
 * |      |        |        |        |        |        |
 * ####.... ........ ........ ........ ........ ........  4: reserved for now
 * ....#### ######## ######## ######## ........ ........ 28: user id whose make transaction
 * ........ ........ ........ ........ ###..... ........  3: history enum variable
 * ........ ........ ........ ........ ...###.. ........  3: save enum variable
 * ........ ........ ........ ........ ......## ######## 10: operate key (for using as type of value by object)
 * ........ ........ ........ ........ ........ ........ */
struct AFlib::id::Operate_bit : public TbitStruct <48>
{
    Account_bit user()   const { return Account_bit(userId()); }
    quint32 userId()     const { return toUInt32(    16, 28);  }
    void setUserId(quint32 id) {       setUInt32(id, 16, 28);  }
    void setUserId(Acc_bit id) {       setUserId(id.accountId()); }

    HIdType historyType()       const { return toHistoryIdType(toUInt8(      13, 3)); }
    void setHistoryType(quint8  type) {                       setUInt8(type, 13, 3); }
    void setHistoryType(HIdType type) {   setHistoryType(fromHisToInt (type)); }

    SIdType saveType()       const { return toSavedIdType(toUInt8(      10, 3)); }
    void setSaveType(quint8  type) {                     setUInt8(type, 10, 3); }
    void setSaveType(SIdType type) {   setSaveType(fromSaveToInt (type)); }

    quint16 key()      const { return toUInt16(     0, 10); }
    void setKey(quint16 key) {       setUInt16(key, 0, 10); }

    quint8  reserved()   const { return  toUInt8(   44, 4); }
    void setReserved(quint8 r) {        setUInt8(r, 44, 4); }
};

class AFlib::id::Operate final : public Operate_bit
{
public:
    explicit Operate(Account_bit userId = Account_bit(),
                     HistoryIdType history = HistoryIdType::First,
                     SavedIdType saved = SavedIdType::LocaleSaved,
                     QVariant value = QVariant(),
                     quint16 valueKey = 0,
                     QDateTime dTime = QDateTime::currentDateTime());

    Operate(const Operate& copy);
    Operate(const QByteArray& data);

    operator QByteArray() const;
    QByteArray getData() const;

    QDateTime datetime() const { return  m_datetime; }
    void setDatetime(const QDateTime &datetime);

    QVariant value() const { return m_value; }
    void setValue(const QVariant &value);

private:
    QDateTime m_datetime;
    QVariant m_value;

    friend class History;
    friend class Object;
    friend class Storage;
};

QDataStream &operator << (QDataStream& stream, const AFlib::id::OperatePtrList& data);
QDataStream &operator >> (QDataStream& stream,       AFlib::id::OperatePtrList& data);

#endif // LIB_BASEAF_ID_OPERATE_H
