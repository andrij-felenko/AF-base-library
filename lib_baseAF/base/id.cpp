#include "id.h"
#include "enumAF.h"
#include <QtCore/QVariant>

// Operation-----------------------------------------------------------------------------------------
AFlib::id::Operate::Operate(quint32 subjectId, quint32 userId, quint32 ownerId,
                            HistoryIdType historyId, SavedIdType saved, QDateTime dTime)
    : Operate(Subject_bit(subjectId), Account_bit(userId), Account_bit(ownerId),
              historyId, saved, dTime)
{
    // it`s done
}

AFlib::id::Operate::Operate(quint32 subjectId, quint32 userId, quint32 ownerId,
                            quint8 historyId, quint8 saved, QDateTime dTime)
    : Operate(subjectId, userId, ownerId,
              AFlib::toHistoryIdType(historyId), AFlib::toSavedIdType(saved), dTime)
{
    // it`s done
}

AFlib::id::Operate::Operate(Subject_bit subjectId, Account_bit userId, Account_bit ownerId,
                            HistoryIdType history, SavedIdType saved, QDateTime dTime)
{
    // explicit constructor
    setHistoryType(history);
    setSaveType(saved);
    setUserId(userId);
    setSubjectId(subjectId);
    m_owner = ownerId;
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

void AFlib::id::Operate::setOwner(const Account_bit &owner)
{
    m_owner = owner;
}

std::optional <AFlib::id::OperatePtr> AFlib::id::History::getLastOperation() const
{
    auto lastChanged = lastChange();
    for (int i = 0; i < m_historyList.count(); i++)
        if (m_historyList[i]->datetime() == lastChanged)
            return m_historyList[i];
    return std::nullopt;
}

namespace AFlib::id {
    QDataStream &operator << (QDataStream& stream, const AFlib::id::Operate& data)
    {
        stream
            << data.m_owner << data.m_datetime << data.m_value
            << static_cast <const Info&> (data)
            << static_cast <const Operate_bit&> (data);
        return stream;
    }

    QDataStream &operator >> (QDataStream& stream,       AFlib::id::Operate& data)
    {
        stream
            >> data.m_owner >> data.m_datetime >> data.m_value
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
// Operation_end_####################################################################################

// History-------------------------------------------------------------------------------------------
AFlib::id::History::History()// : QObject(parent)
{
    m_lastUpdate = QDateTime::fromTime_t(0);
}

void AFlib::id::History::updateTime(QDateTime dTime)
{
    m_lastUpdate = dTime;
}

bool AFlib::id::History::haveUpdates() const
{
    return lastChange() > m_lastUpdate;
}

QDateTime AFlib::id::History::lastChange() const
{
    QDateTime ret = QDateTime(QDate(2000, 1, 1), QTime(0, 0, 0));
    for (auto it : m_historyList)
        if (ret > it->datetime())
            ret = it->datetime();
    return ret;
}

QDateTime AFlib::id::History::lastUpdate() const
{
    return m_lastUpdate;
}

void AFlib::id::History::addOperation(OperatePtr id)
{
    auto lastChanged = lastChange();
    m_historyList.push_back(id);
//    if (haveUpdates())
//        emit needUpdate();
//    emit changed();
}

namespace AFlib::id {
    QDataStream &operator << (QDataStream& stream, const History& data)
    {
        return stream << data.m_historyList;
    }

    QDataStream &operator >> (QDataStream& stream,       History& data)
    {
        return stream >> data.m_historyList;
    }
}
// History_end_######################################################################################

// Info----------------------------------------------------------------------------------------------
AFlib::id::Info::Info(QObject *parent) : Info(QString(""), QString(""), parent)
{
    // it1s done
}

AFlib::id::Info::Info(QString name, QString descr, QObject *parent)
    : QObject(parent), m_name(name), m_description(descr)
{
    // it`s done
}

void AFlib::id::Info::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

void AFlib::id::Info::setDescription(QString description)
{
    if (m_description == description)
        return;

    m_description = description;
    emit descriptionChanged(m_description);
}
namespace AFlib::id {
    QDataStream &operator << (QDataStream& stream, const Info& data)
    {
        return stream << data.m_name << data.m_description;
    }

    QDataStream &operator >> (QDataStream& stream,       Info& data)
    {
        return stream >> data.m_name >> data.m_description;
    }
}
// Info_end_#########################################################################################

// Subject-------------------------------------------------------------------------------------------
AFlib::id::Subject::Subject(QObject *parent) : Subject("", "", 0, 0, 0, 0, 0, parent)
{
    // it`s done
}

AFlib::id::Subject::Subject(quint16 uniqueId, quint8 type, quint8 pluginId, QObject *parent)
    : Subject("", "", uniqueId, type, pluginId, 0, 0, parent)
{
    // it`s done
}

AFlib::id::Subject::Subject(QString name, QString descr, QObject *parent)
    : Subject(name, descr, 0, 0, 0, 0, 0, parent)
{
    // it`s done
}

AFlib::id::Subject::Subject(QString name, QString descr, quint16 uniqueId, quint8 type,
                            quint8 pluginId, quint8 parentType, quint32 parentId, QObject *parent)
    : Info(name, descr, parent),
    SubjectFull_bit(Subject_bit(uniqueId, type, pluginId), parentType, parentId)
{
    // it`s done
}

void AFlib::id::SubjectFull_bit::setParent(quint8 type, quint32 id)
{
    setParnetId(id);
    setParentType(type);
}

AFlib::id::Subject_bit::Subject_bit(quint32 subjectFull)
{
    setUInt32(subjectFull);
}

AFlib::id::Subject_bit::Subject_bit(quint16 uniqueId, quint8 type, quint8 pluginId)
{
    setUniqueId(uniqueId);
    setType(type);
    setPluginId(pluginId);
}

AFlib::id::SubjectFull_bit::SubjectFull_bit(quint64 subjectFull)
{
    setUInt64(subjectFull);
}

AFlib::id::SubjectFull_bit::SubjectFull_bit(quint32 subjectId, quint8 parentType, quint32 parentId)
{
    setId(subjectId);
    setParent(parentType, parentId);
}

AFlib::id::SubjectFull_bit::SubjectFull_bit(Subject_bit subject, quint8 parentType, quint32 parentId)
{
    setId(subject);
    setParent(parentType, parentId);
}

namespace AFlib::id {
    QDataStream &operator << (QDataStream& stream, const AFlib::id::Subject& data)
    {
        stream
            << *data.m_history
            << static_cast <const Info&> (data)
            << static_cast <const SubjectFull_bit&> (data);
        return stream;
    }

    QDataStream &operator >> (QDataStream& stream,       AFlib::id::Subject& data)
    {
        stream
            >> *data.m_history
            >> static_cast <Info&> (data)
            >> static_cast <SubjectFull_bit&> (data);
        return stream;
    }
}

QDataStream &operator << (QDataStream& stream, const AFlib::id::SubjectPtrList& data)
{
    stream << data.count();
    for (auto it = data.begin(); it != data.end(); ++it)
        stream << *it->data();
    return stream;
}

QDataStream &operator >> (QDataStream& stream,       AFlib::id::SubjectPtrList& data)
{
    int count;
    stream >> count;
    for (int i = 0; i < count; i++){
        AFlib::id::SubjectPtr ptr;
        stream >> *ptr;
        data.push_back(ptr);
    }
    return stream;
}
// Subject_end_######################################################################################
