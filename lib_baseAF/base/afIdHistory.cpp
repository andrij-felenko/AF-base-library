#include "afIdHistory.h"

std::optional <AFlib::id::OperatePtr> AFlib::id::History::getLastOperation() const
{
    auto lastChanged = lastChange();
    for (int i = 0; i < m_historyList.count(); i++)
        if (m_historyList[i]->datetime() == lastChanged)
            return m_historyList[i];
    return std::nullopt;
}

void AFlib::id::History::makeShorten()
{
    auto foundMultiple = [=]() -> std::optional <quint32> {
        QVector <quint32> objList;
        objList.clear();

        for (auto it : m_historyList){
            if (objList.contains(it->objectId()))
                return it->objectId();
            else
                objList.push_back(it->objectId());
        }
        return std::nullopt;
    };

    std::optional <quint32> multiple = 0;
    // Remove all not last changes
    while (multiple){
        // 0: found local last, always have it
        quint32 localLast = 0;
        for (auto it : m_historyList)
            if (it->historyType() == HIdType::SavedIdChangeLine)
                if (it->value().toUInt() == multiple){
                    localLast = it->value().toUInt();
                    break;
                }

        // 1: found ties variant for last id
        OperatePtr last = OperatePtr::create();
        last->setDatetime(QDateTime(QDate(1960, 1, 1)));
        for (auto it : m_historyList){
            if (it->objectId() == multiple.value())
                if (last->datetime() < it->datetime())
                    last = it;
        }
        if (not isHIdEnable(last->historyType()))
            last = OperatePtr::create();

        // 2: remove all same as last unique id but not compare this last
        m_historyList.erase(
            std::remove_if(m_historyList.begin(), m_historyList.end(),
                           [last, localLast, multiple](OperatePtr single) {
                               return (multiple == single->objectId() && last.data() != single.data())
                                      || multiple.value() == localLast;
                           }));

        // 3: found new values for remove it from history list
        multiple = foundMultiple();
    }
}

void AFlib::id::History::makeFull(OperatePtrList list)
{
    for (auto it : list)
        addOperation(it);
}

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
    for (auto it : m_historyList)
        if (it.data() == id.data())
            return;

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
