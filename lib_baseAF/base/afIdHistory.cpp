#include "afIdHistory.h"
#include "afStorage.h"
#include "afFunction.h"

AFlib::id::History::History()
{
//    setAttribute(Attribute::Created, QDateTime::currentDateTime());
//    m_lastUpdate = QDateTime::fromTime_t(0);
}

void AFlib::id::History::updateTime(QDateTime dTime)
{
    m_lastUpdate = dTime;
}

bool AFlib::id::History::haveUpdates() const
{
    return lastChange() > m_lastUpdate;
}

bool AFlib::id::History::isEmpty() const
{
    if (not m_operateIdList.isEmpty())
        return false;

    for (const auto& it : m_operateList)
        if (it->key() >= 4)
            return false;
    return true;
}

QDateTime AFlib::id::History::lastChange() const
{
    QDateTime ret = QDateTime(QDate(2000, 1, 1), QTime(0, 0, 0));
    for (const auto& it : m_operateList)
        if (ret > it->datetime())
            ret = it->datetime();
    for (const auto& it : m_operateIdList)
        if (ret > it->datetime())
            ret = it->datetime();
    return ret;
}

QDateTime AFlib::id::History::lastUpdate() const
{
    return m_lastUpdate;
}

QDateTime AFlib::id::History::timeCreate() const
{
    return getAttribute(Attribute::Created).toDateTime();
}

void AFlib::id::History::makeFull(const QByteArray& data)
{
    QDataStream stream(data);
    stream >> m_operateList >> m_operateIdList;
}

void AFlib::id::History::makeShorten(OperatePtrList &list)
{
    auto foundMultiple = [=]() -> std::optional <quint32> {
        QVector <quint32> objList;
        objList.clear();

        for (const auto& it : list){
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
        for (const auto& it : list)
            if (it->historyType() == HIdType::SavedChangeLine)
                if (it->value().toUInt() == multiple){
                    localLast = it->value().toUInt();
                    break;
                }

        // 1: found ties variant for last id
        OperatePtr last = OperatePtr::create();
        last->setDatetime(AFlib::Function::nullDateTime());

        for (const auto& it : list){
            if (it->key() == multiple.value())
                if (last->datetime() < it->datetime())
                    last = it;
        }
        if (not isHIdEnable(last->historyType()))
            last = OperatePtr::create();

        // 2: remove all same as last unique id but not compare this last
        list.erase(
            std::remove_if(list.begin(), list.end(),
                           [last, localLast, multiple](OperatePtr single) {
                               return (multiple == single->key() && last.data() != single.data())
                                      || multiple.value() == localLast;
                           }));

        // 3: found new values for remove it from history list
        multiple = foundMultiple();
    }
}

void AFlib::id::History::makeShorten()
{
    makeShorten(m_operateList);
    makeShorten(m_operateIdList);
}

void AFlib::id::History::saveShorten()
{
    // TODO
}

void AFlib::id::History::useCompress(Compress value)
{
    // TODO not forget about that we have 2 operate lists
    auto removeFunction = [value](const OperatePtr ptr){
        switch (value){
        case Compress::Shortest: {
            //
            break;
        }
        case Compress::AllActive: {
            //
            break;
        }
        case Compress::EachByOne: {
            //
            break;
        }
        default:;
        }

        return ptr.isNull();
    };
    m_operateList.erase(std::remove_if(m_operateList.begin(), m_operateList.end(), removeFunction), m_operateList.end());
}

QVariant AFlib::id::History::getAttribute(Attribute key) const
{
    return getAttribute(static_cast <quint16> (key));
}

void AFlib::id::History::setAttribute(Attribute key, QVariant value)
{
    setAttribute(static_cast <quint16> (key), value);
}

QVariantList AFlib::id::History::getMultiAttribute(Attribute key) const
{
    return getMultiAttribute(m_operateList, static_cast <quint16> (key));
}

void AFlib::id::History::setMultiAttribute(Attribute key, QVariant value, HIdType type)
{
    setMultiAttribute(static_cast <quint16> (key), value, type);
}

AFlib::id::OperatePtrList AFlib::id::History::getAllOperates() const
{
    return m_operateList + m_operateIdList;
}

AFlib::id::OperatePtrList AFlib::id::History::getIdOperates() const
{
    return m_operateIdList;
}

AFlib::id::OperatePtrList AFlib::id::History::getNoIdOperates() const
{
    return m_operateList;
}

AFlib::id::OperatePtrList AFlib::id::History::getListAfter(const QDateTime &afterTime) const
{
    // WARNING need to test compress flag, so if we have shorten flag, it can`t return full list of operate
    OperatePtrList retList;
    for (const auto& it : m_operateList)
        if (it->datetime() > afterTime)
            if (it->setSaveType(SIdType::OnTheWayToServer))
                retList.push_back(it);
    return retList;
}

AFlib::id::OperatePtrList AFlib::id::History::getListIdAfter(const QDateTime &afterTime) const
{
    // WARNING need to test compress flag, so if we have shorten flag, it can`t return full list of operate
    OperatePtrList retList;
    for (const auto& it : m_operateIdList)
        if (it->datetime() > afterTime)
            if (it->setSaveType(SIdType::OnTheWayToServer))
                retList.push_back(it);
    return retList;
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
    for (const auto& it : ptrList)
        addOperate(it);
}

void AFlib::id::History::addOperations(const OperatePtrList list, bool saveToStorage, bool isId)
{
    for (const auto& it : list)
        addOperate(it, saveToStorage, isId);
}

void AFlib::id::History::addOperate(Attribute attributeKey, QVariant value, Account_bit userId,
                                    HIdType history, SIdType saved, QDateTime dTime)
{
    addOperate(static_cast <quint16> (attributeKey), value, userId, history, saved, dTime);
}

void AFlib::id::History::addOperate(Attribute attributeKey, QVariant value, quint32 userId,
                                    quint8 historyId, quint8 savedId, QDateTime dTime)
{
    addOperate(static_cast <quint16> (attributeKey), value, Account_bit(userId), toHistoryIdType(historyId), toSavedIdType(savedId), dTime);
}

void AFlib::id::History::refreshLastChangeTime()
{
    // TODO
}

AFlib::id::Global_bit AFlib::id::History::getIdAttribute(quint16 key)
{
    return Global_bit(getAttribute(m_operateIdList, key).toULongLong());
}

void AFlib::id::History::setIdAttribute(quint16 key, const Global_bit &globalId)
{
    setAttribute(key, globalId.toNumber(), true);
}

QVariant AFlib::id::History::getAttribute(quint16 key) const
{
    return getAttribute(m_operateList, key);
}

QVariant AFlib::id::History::getAttribute(const OperatePtrList& list, quint16 key) const
{
    OperatePtr last = OperatePtr::create();
    last->setDatetime(Function::nullDateTime());
    for (const auto& it : list)
        if (it->key() == key)
            if (it->m_datetime > last->m_datetime){
                last = it;
                break;
            }
    if (last.isNull())
        return QVariant();
    if (last->key() == 0 || not isHIdEnable(last->historyType()))
        return QVariant();
    return last->m_value;
}

void AFlib::id::History::setAttribute(quint16 key, const QVariant &value, bool isId)
{
    if (getAttribute(key).isNull())
        addOperate(key, value, m_owner, HIdType::AddLine, SIdType::Local, isId);
    else
        addOperate(key, value, m_owner, HIdType::EditLine, SIdType::Local, isId);
}

QVariantList AFlib::id::History::getMultiAttribute(const OperatePtrList& opList, quint16 key) const
{
    OperatePtrList list;
    QMap <QVariant, QDateTime> valueMap;
    for (const auto& it : opList)
        if (it->key() == key){
            list.push_back(it);
            for (auto vIt = valueMap.begin(); vIt != valueMap.end(); ++vIt)
                if (vIt.key() == it->value())
                    if (vIt.value() < it->datetime()){
                        vIt.value() = it->datetime();
                        break;
                    }
        }

    // remove not last values
    // it's for std::remove_if
    auto foundLast = [valueMap](const OperatePtr ptr){
        for (auto it = valueMap.begin(); it != valueMap.end(); ++it)
            if (it.key() == ptr->value())
                return it.value() != ptr->datetime() || isHIdEnable(ptr->historyType());
        return false;
    };
    auto removes = std::remove_if(list.begin(), list.end(), foundLast);
    list.erase(removes);

    // get variant list by key
    QVariantList retList;
    for (const auto& it : list)
        retList.push_back(it->value());

    return retList;
}

void AFlib::id::History::setMultiAttribute(quint16 key, const QVariant &value, HIdType type, bool isId)
{
    addOperate(key, value, m_owner, type, SIdType::Local, isId);
}

AFlib::id::GlobalList_bit AFlib::id::History::getMultiIdAttribute(quint16 key) const
{
    auto list = getMultiAttribute(m_operateIdList, key);
    GlobalList_bit retList;
    for (const auto &it : list)
        retList.push_back(Global_bit(it.toULongLong()));
    return retList;
}

void AFlib::id::History::setMultiIdAttribute(quint16 key, const Global_bit &value, HIdType type)
{
    setMultiAttribute(key, value.toNumber(), type, true);
}

void AFlib::id::History::addOperate(Operate id, bool saveToStorage)
{
    OperatePtr ptr = OperatePtr::create(id);
    addOperate(ptr, saveToStorage);
}

void AFlib::id::History::addOperate(OperatePtr id, bool saveToStorage, bool isId)
{
    for (const auto &it : m_operateList)
        if (it.data() == id.data())
            return;

    if (isId)
        m_operateIdList.push_back(id);
    else
        m_operateList.push_back(id);
    refreshLastChangeTime();

    // save it to storage
    if (saveToStorage && savedStatus() != SavedIdType::Temporary)
        this->saveToStorage(id, isId);
}

void AFlib::id::History::addOperate(quint16 valueKey, QVariant value, Account_bit userId,
                                    HistoryIdType history, SavedIdType saved, bool isId,
                                    QDateTime dTime)
{
    OperatePtr ptr = OperatePtr::create(userId, history, saved, value, valueKey, dTime);
    addOperate(ptr, true, isId);
}

namespace AFlib::id {
QDataStream &operator << (QDataStream& stream, const History& data)
{
    return stream << data.m_operateList << data.m_operateIdList << data.m_lastUpdate;
}

QDataStream &operator >> (QDataStream& stream,       History& data)
{
    // TODO maybe clear all lists
    return stream >> data.m_operateList >> data.m_operateIdList >> data.m_lastUpdate;
}
}
