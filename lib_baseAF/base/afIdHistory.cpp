#include "afIdHistory.h"

AFlib::id::History::History()
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
    for (auto it : m_operateList)
        if (ret > it->datetime())
            ret = it->datetime();
    return ret;
}

QDateTime AFlib::id::History::lastUpdate() const
{
    return m_lastUpdate;
}

void AFlib::id::History::makeFull(const QByteArray& data)
{
    QDataStream stream(data);
    stream >> m_operateList;
}

void AFlib::id::History::makeShorten()
{
    auto foundMultiple = [=]() -> std::optional <quint32> {
        QVector <quint32> objList;
        objList.clear();

        for (auto it : m_operateList){
            if (objList.contains(it->key()))
                return it->key();
            else
                objList.push_back(it->key());
        }
        return std::nullopt;
    };

    std::optional <quint32> multiple = 0;
    // Remove all not last changes
    while (multiple){
        // 0: found local last, always have it
        quint32 localLast = 0;
        for (auto it : m_operateList)
            if (it->historyType() == HIdType::SavedIdChangeLine)
                if (it->value().toUInt() == multiple){
                    localLast = it->value().toUInt();
                    break;
                }

        // 1: found ties variant for last id
        OperatePtr last = OperatePtr::create();
        last->setDatetime(QDate(1960, 1, 1).startOfDay());
        for (auto it : m_operateList){
            if (it->key() == multiple.value())
                if (last->datetime() < it->datetime())
                    last = it;
        }
        if (not isHIdEnable(last->historyType()))
            last = OperatePtr::create();

        // 2: remove all same as last unique id but not compare this last
        m_operateList.erase(
            std::remove_if(m_operateList.begin(), m_operateList.end(),
                           [last, localLast, multiple](OperatePtr single) {
                               return (multiple == single->key() && last.data() != single.data())
                                      || multiple.value() == localLast;
                           }));

        // 3: found new values for remove it from history list
        multiple = foundMultiple();
    }
}

void AFlib::id::History::saveShorten()
{
    //
}

void AFlib::id::History::useCompress(CompressValue value)
{
    // TODO write it
}

QVariant AFlib::id::History::getValue(ValueType key) const
{
    return getValue(static_cast <quint16> (key));
}

void AFlib::id::History::setValue(ValueType key, QVariant value) const
{
    setValue(static_cast <quint16> (key), value);
}

void AFlib::id::History::addOperate(const QByteArray& data)
{
    addOperate(Operate(data));
}

void AFlib::id::History::addOperations(const QByteArray& list)
{
    OperatePtrList ptrList;
    QDataStream stream(list);
    stream >> ptrList;
    for (auto it : ptrList)
        addOperate(it);
}

void AFlib::id::History::addOperate(ValueType valueKey, QVariant value, Account_bit userId,
                                    HIdType history, SIdType saved, QDateTime dTime)
{
    addOperate(static_cast <quint16> (valueKey), value, userId, history, saved, dTime);
}

void AFlib::id::History::addOperate(ValueType valueKey, QVariant value, quint32 userId,
                                    quint8 historyId, quint8 savedId, QDateTime dTime)
{
    addOperate(static_cast <quint16> (valueKey), value, Account_bit(userId), toHistoryIdType(historyId), toSavedIdType(savedId), dTime);
}

void AFlib::id::History::refreshLastChangeTime()
{
    //
}

QVariant AFlib::id::History::getValue(quint16 key) const
{
    OperatePtr last = OperatePtr::create();
    last->setDatetime(QDate(1960, 1, 1).startOfDay());
    for (auto it : m_operateList)
        if (it->key() == key)
            if (it->m_datetime > last->m_datetime)
                last = it;
    if (last->key() == 0 || not isHIdEnable(last->historyType()))
        return QVariant();
    return last->m_value;
}

void AFlib::id::History::setValue(quint16 key, const QVariant &value)
{
    if (getValue(key).isNull())
        addOperate(key, value, m_owner, HIdType::AddIdLine, SIdType::LocaleSaved);
    else
        addOperate(key, value, m_owner, HIdType::EditIdLine, SIdType::LocaleSaved);
}

void AFlib::id::History::addOperate(Operate id)
{
    OperatePtr ptr = OperatePtr::create(id);
    addOperate(ptr);
}

void AFlib::id::History::addOperate(OperatePtr id)
{
    for (auto it : m_operateList)
        if (it.data() == id.data())
            return;

    m_operateList.push_back(id);
    refreshLastChangeTime();
}

void AFlib::id::History::addOperate(quint16 valueKey, QVariant value, Account_bit userId,
                                    HistoryIdType history, SavedIdType saved, QDateTime dTime)
{
    OperatePtr ptr = OperatePtr::create(userId, history, saved, value, valueKey, dTime);
    addOperate(ptr);
}

namespace AFlib::id {
QDataStream &operator << (QDataStream& stream, const History& data)
{
    return stream << data.m_operateList << data.m_lastUpdate;
}

QDataStream &operator >> (QDataStream& stream,       History& data)
{
    return stream >> data.m_operateList >> data.m_lastUpdate;
}
}
