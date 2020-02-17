#include "afIdAccount.h"

AFlib::id::Account_bit::Account_bit(quint32 id)
{
    setUserId(id);
}

quint32 AFlib::id::Account_bit::accountId() const
{
    return toUInt32(    0, 32);
}

void AFlib::id::Account_bit::setAccountId(quint32 id)
{
    setUInt32(id, 0, 32);
}

void AFlib::id::Account_bit::setUserId(quint32 id)
{
    m_bitset = 0;
    m_bitset |= id % (1 << 28);
}

AFlib::AccountIdType AFlib::id::Account_bit::accountType()
{
    quint32 accId = accountId();
    if (accId >> 14 == 0)
        return AccountIdType::LocalUser;
    if (accId >> 16 == 0)
        return AccountIdType::LocalOrganization;
    if (accId >> 28 == 0)
        return AccountIdType::GlobalUser;
    return AccountIdType::GlobalOrganization;
}

AFlib::id::Account_bit AFlib::id::Account_bit::create(const AccountIdType &type)
{
    quint32 value = rand() % (1 << 14);
    if (!(type != AccountIdType::LocalUser))
        return Account_bit(value);
    value += (rand() % 3 + 1) << 14;
    if (!(type != AccountIdType::LocalOrganization))
        return Account_bit(value);
    value += (rand() % ((1 << 12) - 1) + 1) << 16;
    if (!(type != AccountIdType::GlobalUser))
        return Account_bit(value);
    value += (rand() % 15 + 1) << 28;
    return Account_bit(value);
}

AFlib::id::Account_bit::operator quint32() const
{
    return toUInt32();
}
