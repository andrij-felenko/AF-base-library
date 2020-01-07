#include "id.h"
#include "enumAF.h"

AFlib::SubjectId::SubjectId(quint32 value)
{
    setUInt32((value << 6) + (1 << 6) - 1, 6, 26);
}

AFlib::AccountId::AccountId(quint32 id) {
    setGlobalUser(id);
}

AFlib::AccountId AFlib::AccountId::create(const AccountIdType& type)
{
    quint32 value = rand() % (1 << 14);
    if (!(type != AccountIdType::LocalUser))
        return AccountId(value);
    value += (rand() % 3 + 1) << 14;
    if (!(type != AccountIdType::LocalOrganization))
        return AccountId(value);
    value += (rand() % ((1 << 12) - 1) + 1) << 16;
    if (!(type != AccountIdType::GlobalUser))
        return AccountId(value);
    value += (rand() % 15 + 1) << 28;
    return AccountId(value);
}

AFlib::TransactionId::TransactionId()
{
    //
}

AFlib::HistoryId::HistoryId(QObject *parent) : QObject(parent)
{
    m_lastChange = QDateTime::fromTime_t(0);
    m_lastUpdate = QDateTime::fromTime_t(0);
}

void AFlib::HistoryId::change(QDateTime dTime)
{
    m_lastChange = dTime;
    if (haveUpdates())
        emit needUpdate();
}

void AFlib::HistoryId::update(QDateTime dTime)
{
    m_lastUpdate = dTime;
}

bool AFlib::HistoryId::haveUpdates() const
{
    return m_lastChange > m_lastUpdate;
}

QDateTime AFlib::HistoryId::lastUpdate() const
{
    return m_lastUpdate;
}
