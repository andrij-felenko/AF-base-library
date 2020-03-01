#include "afIdOperate.h"
#include <QtCore/QSharedPointer>

void AFlib::id::Operate::setValue(const QVariant &value)
{
    m_value = value;
}

AFlib::id::Operate::Operate(const AFlib::id::Operate &copy)
{
    m_datetime = copy.m_datetime;
    m_value = copy.m_value;
    m_bitset = copy.m_bitset;
}

AFlib::id::Operate::Operate(const QByteArray &data)
{
    QDataStream stream(data);
    stream >> m_datetime >> m_value >> m_bitset;
}

QByteArray AFlib::id::Operate::data() const
{
    return *this;
}

AFlib::id::Operate::operator QByteArray() const
{
    QByteArray data;
    QDataStream stream(data);
    stream << m_datetime << m_value << m_bitset;
    return data;
}

AFlib::id::Operate::Operate(Account_bit userId, HistoryIdType history, SavedIdType saved, QVariant value, quint16 valueKey, QDateTime dTime)
{
    // explicit constructor
    setHistoryType(history);
    setSaveType(saved);
    setUserId(userId);
    setKey(valueKey);
    setValue(value);
    m_datetime = dTime;
}

void AFlib::id::Operate::setDatetime(const QDateTime &datetime)
{
    m_datetime = datetime;
}

QDataStream &operator << (QDataStream& stream, const AFlib::id::OperatePtrList& data)
{
    stream << data.count();
    for (auto it = data.begin(); it != data.end(); ++it)
        stream << it->data();
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
