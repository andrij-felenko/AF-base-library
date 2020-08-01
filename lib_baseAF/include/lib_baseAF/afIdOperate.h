#ifndef LIB_BASEAF_ID_OPERATE_H
#define LIB_BASEAF_ID_OPERATE_H

#include <QtCore/QDateTime>
#include <QtCore/QVariant>
#include <QtCore/QSharedPointer>

#include "afEnum.h"
#include "afIdAccount.h"
#include "afIdGlobalBit.h"

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
    Account_bit user()    const { return Account_bit(userId()); }
    quint32 userId()      const { return toUInt32(    16, 28);  }
    HIdType historyType() const { return toHistoryIdType(toUInt8(      13, 3)); }
    SIdType saveType()    const { return toSavedIdType(toUInt8(      10, 3)); }
    quint16 key()         const { return toUInt16(     0, 10); }

    quint8  reserved()   const { return  toUInt8(   44, 4); }
    void setReserved(quint8 r) {        setUInt8(r, 44, 4); }

protected:
    virtual void setUserId(quint32 id) final { setUInt32(id, 16, 28);  }
    virtual void setUserId(Acc_bit id) final { setUserId(id.accountId()); }

    virtual void setHistoryType(quint8  type) final { setUInt8(type, 13, 3); }
    virtual void setHistoryType(HIdType type) final { setHistoryType(fromHisToInt (type)); }

    virtual bool setSaveType(quint8  type) final;
    virtual bool setSaveType(SIdType type) final;

    virtual void setKey(quint16 key) final {       setUInt16(key, 0, 10); }

private:
    friend QDataStream& operator << (QDataStream& s, const Operate_bit& op){
        return s << static_cast <const TbitStruct <48>> (op);
    }
    friend QDataStream& operator >> (QDataStream& s, Operate_bit& op){
        TbitStruct <48> bits;
        s >> bits;
        op.setFromValue <quint64, 48> (bits.toUInt64(), 0, 48);
        return s;
    }
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
    virtual ~Operate();

    operator QByteArray() const;
    QByteArray getData() const;

    QDateTime datetime() const { return  m_datetime; }
    void setDatetime(const QDateTime &datetime);

    QVariant value() const { return m_value; }
    void setValue(const QVariant &value);

private:
    QDateTime m_datetime;
    QVariant m_value;

    // FIXME
    friend class History;
    friend class Object;
    friend class AFlib::Storage;

    // use only in update afStorage
    Global_bit valueId() const;
    void setValueId(Global_bit newId);
};

QDataStream &operator << (QDataStream& stream, const AFlib::id::OperatePtrList& data);
QDataStream &operator >> (QDataStream& stream,       AFlib::id::OperatePtrList& data);

#endif // LIB_BASEAF_ID_OPERATE_H
