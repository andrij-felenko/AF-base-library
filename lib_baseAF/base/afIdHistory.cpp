#include "afIdHistory.h"

void AFlib::id::History::makeShorten()
{
    auto foundMultiple = [=]() -> std::optional <quint32> {
        QVector <quint32> objList;
        objList.clear();

        for (auto it : m_historyList){
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
        for (auto it : m_historyList)
            if (it->historyType() == HIdType::SavedIdChangeLine)
                if (it->value().toUInt() == multiple){
                    localLast = it->value().toUInt();
                    break;
                }

        // 1: found ties variant for last id
        OperatePtr last = OperatePtr::create();
        last->setDatetime(QDate(1960, 1, 1).startOfDay());
        for (auto it : m_historyList){
            if (it->key() == multiple.value())
                if (last->datetime() < it->datetime())
                    last = it;
        }
        if (not isHIdEnable(last->historyType()))
            last = OperatePtr::create();

        // 2: remove all same as last unique id but not compare this last
        m_historyList.erase(
            std::remove_if(m_historyList.begin(), m_historyList.end(),
                           [last, localLast, multiple](OperatePtr single) {
                               return (multiple == single->key() && last.data() != single.data())
                                      || multiple.value() == localLast;
                           }));

        // 3: found new values for remove it from history list
        multiple = foundMultiple();
    }
}

void AFlib::id::History::makeFull(const QByteArray &data)
{
    QDataStream stream(data);
    stream >> m_historyList;
}

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
    for (auto it : m_historyList)
        if (ret > it->datetime())
            ret = it->datetime();
    return ret;
}

QDateTime AFlib::id::History::lastUpdate() const
{
    return m_lastUpdate;
}

void AFlib::id::History::addOperation(Operate id)
{
    OperatePtr ptr = OperatePtr::create(id);
    addOperation(ptr);
}

void AFlib::id::History::addOperation(OperatePtr id)
{
    for (auto it : m_historyList)
        if (it.data() == id.data())
            return;

    m_historyList.push_back(id);
}

namespace AFlib::id {
QDataStream &operator << (QDataStream& stream, const History& data)
{
    return stream << data.m_historyList << data.m_lastUpdate;
}

QDataStream &operator >> (QDataStream& stream,       History& data)
{
    return stream >> data.m_historyList >> data.m_lastUpdate;
}
}
