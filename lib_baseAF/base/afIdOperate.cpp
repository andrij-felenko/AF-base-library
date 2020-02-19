#include "afIdOperate.h"
#include <QtCore/QSharedPointer>

AFlib::id::Operate::Operate(quint32 objectId, quint32 userId,
                            HistoryIdType historyId, SavedIdType saved, QDateTime dTime)
    : Operate(Object_bit(objectId), Account_bit(userId),
              historyId, saved, dTime)
{
    // it`s done
}

AFlib::id::Operate::Operate(quint32 objectId, quint32 userId,
                            quint8 historyId, quint8 saved, QDateTime dTime)
    : Operate(objectId, userId,
              AFlib::toHistoryIdType(historyId), AFlib::toSavedIdType(saved), dTime)
{
    // it`s done
}

AFlib::id::Operate::Operate(const AFlib::id::Operate &copy)
{
    setName(copy.m_name);
    setDescription(copy.m_description);
    m_datetime = copy.m_datetime;
    m_value = copy.m_value;
    m_bitset = copy.m_bitset;
}

AFlib::id::Operate::Operate(Object_bit objectId, Account_bit userId,
                            HistoryIdType history, SavedIdType saved, QDateTime dTime)
{
    // explicit constructor
    setHistoryType(history);
    setSaveType(saved);
    setUserId(userId);
    setObjectId(objectId);
    m_datetime = dTime;
}

void AFlib::id::Operate::setValue(const QVariant &value)
{
    m_value = value;
}

void AFlib::id::Operate::setDatetime(const QDateTime &datetime)
{
    m_datetime = datetime;
}

namespace AFlib::id {
QDataStream &operator << (QDataStream& stream, const AFlib::id::Operate& data)
{
    stream
        << data.m_datetime << data.m_value
        << static_cast <const Info&> (data)
        << static_cast <const Operate_bit&> (data);
    return stream;
}

QDataStream &operator >> (QDataStream& stream,       AFlib::id::Operate& data)
{
    stream
        >> data.m_datetime >> data.m_value
        >> static_cast <Info&> (data)
        >> static_cast <Operate_bit&> (data);
    return stream;
}
}

QDataStream &operator << (QDataStream& stream, const AFlib::id::OperatePtrList& data)
{
    stream << data.count();
    for (auto it = data.begin(); it != data.end(); ++it)
        stream << *it->data();
    return stream;
}

QDataStream &operator >> (QDataStream& stream,       AFlib::id::OperatePtrList& data)
{
    int count;
    stream >> count;
    for (int i = 0; i < count; i++){
        AFlib::id::OperatePtr ptr;
        stream >> *ptr;
        data.push_back(ptr);
    }
    return stream;
}
