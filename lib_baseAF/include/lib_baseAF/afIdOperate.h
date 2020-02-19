#ifndef LIB_BASEAF_ID_OPERATE_H
#define LIB_BASEAF_ID_OPERATE_H

#include <QtCore/QDateTime>
#include <QtCore/QVariant>

#include "afEnum.h"
#include "afIdAccount.h"
#include "afIdInfo.h"
#include "afIdObjectBit.h"

namespace AFlib::id {
    class Operate;
    struct Operate_bit;
    typedef QSharedPointer <Operate> OperatePtr;
    typedef QList <OperatePtr> OperatePtrList;
}

/* ___________________________OPERATE_ID_Bit____________________________
 *                                       |_______[28]_OBJECT_ID________|
 * 61  56       48       40       32       24       16        8        0
 * |    |        |        |        |        |        |        |        |
 * ###### ######## ######## ######.. ........ ........ ........ ........ 28: user id whose make transaction
 * ...... ........ ........ ......## #....... ........ ........ ........  3: history enum variable
 * ...... ........ ........ ........ .###.... ........ ........ ........  3: save enum variable
 * ...... ........ ........ ........ ....#### ######## ####.... ........ 16: object unique id by current types
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

    Object_bit objectId_b()   const { return Object_bit(objectId()); }
    void setObjectId(Object_bit id) { setObjectId(id.id()); }

    quint32 objectId()     const { return toUInt32(    0, 28); }
    void setObjectId(quint32 id) {       setUInt32(id, 0, 28); }

    quint32 uniqueId()     const { return toUInt16(    12, 16); }
    void setUniqueId(quint16 id) {       setUInt16(id, 12, 16); }

    quint32 type()      const { return toUInt8(      5, 7); }
    void setType(quint8 type) {       setUInt8(type, 5, 7); }

    quint32 pluginId()    const { return toUInt8(    0, 5); }
    void setPLuginId(quint8 id) {       setUInt8(id, 0, 5); }
};

class AFlib::id::Operate : public Info, public Operate_bit
{
public:
    Operate(quint32 subjectId, quint32 userId, HistoryIdType historyId,
            SavedIdType saved = SavedIdType::LocaleSaved,
            QDateTime dTime = QDateTime::currentDateTime());

    Operate(quint32 subjectId, quint32 userId, quint8 historyId,
            quint8 saved = quint8(SavedIdType::LocaleSaved),
            QDateTime dTime = QDateTime::currentDateTime());

    Operate(const Operate& copy);

    explicit Operate(Object_bit objectId = Object_bit(),
                     Account_bit userId = Account_bit(),
                     HistoryIdType history = HistoryIdType::First,
                     SavedIdType saved = SavedIdType::LocaleSaved,
                     QDateTime dTime = QDateTime::currentDateTime());

    QDateTime datetime() const { return  m_datetime; }
    void setDatetime(const QDateTime &datetime);

    QVariant value() const { return m_value; }
    void setValue(const QVariant &value);

protected:
    friend QDataStream &operator << (QDataStream& stream, const Operate& data);
    friend QDataStream &operator >> (QDataStream& stream,       Operate& data);

private:
    QDateTime m_datetime;
    QVariant m_value;
};

QDataStream &operator << (QDataStream& stream, const AFlib::id::OperatePtrList& data);
QDataStream &operator >> (QDataStream& stream,       AFlib::id::OperatePtrList& data);

#endif // LIB_BASEAF_ID_OPERATE_H
