#ifndef LIB_BASEAF_ID_H
#define LIB_BASEAF_ID_H

#include "afIdBit.h"
#include "afIdAccount.h"
#include "afIdOperate.h"
#include "afIdObject.h"
#include "afIdHistory.h"
#include "AfTransferOperateList"

namespace AFlib {
    typedef id::Account_bit IdAccount_bit;

    typedef id::History    IdHistory;
    typedef id::HistoryPtr IdHistoryPtr;

    typedef id::Object           IdObject;
    typedef id::ObjectUnique_bit IdObjectU_bit;
    typedef id::Object_bit       IdObject_bit;
    typedef id::ObjectPtr        IdObjectPtr;
    typedef id::ObjectPtrList    IdObjectPtrList;

    typedef id::Operate        IdOperate;
    typedef id::OperatePtr     IdOperatePtr;
    typedef id::OperatePtrList IdOperatePtrList;

    typedef id::ValueType ValueType;
    typedef  id::CompressValue CompressValue;

    typedef transfer::List TransferOperateList;
}

typedef AFlib::IdAccount_bit AFIdAccount;

typedef AFlib::IdHistory    AFIdHistory;
typedef AFlib::IdHistoryPtr AFIdHistoryPtr;

typedef AFlib::IdObject         AFIdObject;
typedef AFlib::IdObjectU_bit    AFIdObjectU_bit;
typedef AFlib::IdObject_bit     AFIdObject_bit;
typedef AFlib::IdObjectPtr      AFIdObjectPtr;
typedef AFlib::IdObjectPtrList  AFIdObjectPtrList;

typedef AFlib::IdOperate        AFIdOperate;
typedef AFlib::IdOperatePtr     AFIdOperatePtr;
typedef AFlib::IdOperatePtrList AFIdOperatePtrList;

typedef AFlib::ValueType     AFValueType;
typedef AFlib::CompressValue AFCompressValue;

typedef AFlib::TransferOperateList AFTransferOperateList;

#endif // LIB_BASEAF_ID_H
