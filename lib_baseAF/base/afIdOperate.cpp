#include "afIdOperate.h"
#include <QtCore/QSharedPointer>
#include <QtCore/QTimer>
#include <AfFunction>

void AFlib::id::Operate::setValue(const QVariant &value)
{
    m_value = value;
}

AFlib::id::Global_bit AFlib::id::Operate::valueId() const
{
    return Global_bit(value().toULongLong());
}

void AFlib::id::Operate::setValueId(Global_bit newId)
{
    setValue(newId.toNumber());
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
    stream >> m_datetime >> m_value >> static_cast <Operate_bit&> (*this);
}

QByteArray AFlib::id::Operate::getData() const
{
    return *this;
}

AFlib::id::Operate::operator QByteArray() const
{
    QByteArray data;
    QBuffer buffer(&data);
    buffer.open(QIODevice::WriteOnly);
    QDataStream stream(&buffer);
    stream << m_datetime << m_value << static_cast <Operate_bit>(*this);
    buffer.close();
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
    int count = 0;
    stream >> count;
    for (int i = 0; i < count; i++){
        AFlib::id::OperatePtr ptr = AFlib::id::OperatePtr::create();
        stream >> *ptr;
        data.push_back(ptr);
    }
    return stream;
}

bool AFlib::id::Operate_bit::setSaveType(quint8 type)
{
    return setSaveType(toSavedIdType(type));
}

bool AFlib::id::Operate_bit::setSaveType(SIdType type)
{
    auto current = saveType();
    if (current == type)
        return false;

    if (current == SIdType::OnServer && type == SIdType::OnTheWayToServer)
        return false;

    setUInt8(fromSaveToInt(type), 10, 3);
    static auto returnToLocalSaveStatus = [this](){
        if (saveType() == SIdType::OnTheWayToServer)
            setSaveType(SIdType::Local);
    };
    QTimer::singleShot(7000, nullptr, returnToLocalSaveStatus);
    return true;
}

std::ostream& operator <<(std::ostream& os, const AFlib::id::Operate &op)
{
    using namespace AFlib;
    return os << "AFlib::id::Operate {\n\t"
                "value:        " << op.value().constData() << "\n\t"
             << "date:         " << op.datetime().date().toString("dd MMMM yyyy, dddd").toStdString() << "\n\t"
             << "time:         " << op.datetime().time().toString("hh:mm:ss (zzz)").toStdString() << "\n\t"
             << "operate key:  " << Function::toString(op.key(),     16, 10) << "\n\t"
             << "user id:      " << Function::toString(op.userId(),  16, 28) << "\n\t"
             << "reserved:     " << Function::toString(op.reserved(), 2,  4) << "\n\t"
             << "history type: " << AFlib::historyToString(op.historyType()) << "\n\t"
             << "save type:    " << AFlib::saveTypeToString(op.saveType()) << "\n}\n";
}

QDebug operator <<(QDebug d, const AFlib::id::Operate &op)
{
    using namespace AFlib;
    return d << "AFlib::id::Operate {\n\t"
             << "value:        " << op.value() << "\n\t"
             << "date:         " << op.datetime().date().toString("dd MMMM yyyy, dddd").toStdString().c_str() << "\n\t"
             << "time:         " << op.datetime().time().toString("hh:mm:ss (zzz)").toStdString().c_str() << "\n\t"
             << "operate key:  " << Function::toString(op.key(),     16, 10).c_str() << "\n\t"
             << "user id:      " << Function::toString(op.userId(),  16, 28).c_str() << "\n\t"
             << "reserved:     " << Function::toString(op.reserved(), 2,  4).c_str() << "\n\t"
             << "history type: " << AFlib::historyToString(op.historyType()).c_str() << "\n\t"
             << "save type:    " << AFlib::saveTypeToString(op.saveType()).c_str() << "\n}\n";
}

QDebug operator <<(QDebug d, const AFlib::id::Operate_bit &op_b)
{
    using namespace AFlib;
    return d << "AFlib::id::Operate_bit {\n\t"
             << "operate key:  " << Function::toString(op_b.key(),     16, 10).c_str() << "\n\t"
             << "user id:      " << Function::toString(op_b.userId(),  16, 28).c_str() << "\n\t"
             << "reserved:     " << Function::toString(op_b.reserved(), 2,  4).c_str() << "\n\t"
             << "history type: " << AFlib::historyToString(op_b.historyType()).c_str() << "\n\t"
             << "save type:    " << AFlib::saveTypeToString(op_b.saveType()).c_str() << "\n}\n";;
}
