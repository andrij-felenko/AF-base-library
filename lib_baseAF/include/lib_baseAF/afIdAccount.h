#ifndef LIB_BASEAF_ID_ACCOUNT_H
#define LIB_BASEAF_ID_ACCOUNT_H

#include "afIdBit.h"
#include "afEnum.h"

namespace AFlib::id {
    struct Account_bit;
    typedef Account_bit Acc_bit;
}

/* ________ACCOUNT_ID_Bit_________________
 *
 *   28   24   20   16   12    8    4    0
 *    |    |    |    |    |    |    |    |
 * 0000 0000 0000 0000 00xx xxxx xxxx xxxx - 14: local user
 * 0000 0000 0000 0000 xxxx xxxx xxxx xxxx - 16: local organization
 * 0000 xxxx xxxx xxxx xxxx xxxx xxxx xxxx - 28: global user
 * xxxx xxxx xxxx xxxx xxxx xxxx xxxx xxxx - 32: global organization
 * .... .... .... .... .... .... .... .... */
struct AFlib::id::Account_bit : public TbitStruct <32>
{
    explicit Account_bit(quint32 id = 0);

    quint32 accountId()     const;
    void setAccountId(quint32 id);
    void setUserId   (quint32 id);

    operator quint32() const;

    AccountIdType accountType();

    static Account_bit create(const AccountIdType &type);

    //    friend QDataStream &operator << (QDataStream& stream, const Account_bit& data);
    //    friend QDataStream &operator >> (QDataStream& stream,       Account_bit& data);
};

#endif // LIB_BASEAF_ID_ACCOUNT_H
