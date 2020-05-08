#ifndef LIB_BASEAF_TRANSFER_OPERATE_LIST_H
#define LIB_BASEAF_TRANSFER_OPERATE_LIST_H

#include "afIdAccount.h"
#include "afIdObjectBit.h"
#include "afIdOperate.h"
#include "afIdObject.h"

namespace AFlib::transfer {
    class  List;

    namespace list {
        struct Objects;
        struct Operates;
    }
}

namespace AFlib::transfer::list {
    struct Objects;
    struct Operates;
}

struct AFlib::transfer::list::Operates : public id::OperatePtrList {
    AFlib::id::Account_bit owner;
    AFlib::id::Object_bit object;
};

struct AFlib::transfer::list::Objects : public QList <Operates> {
    QStringList dPath;
    FileType fileType;
    void addOperate(id::Account_bit owner, id::Object_bit object, id::OperatePtrList list);
    void addOperate(id::Account_bit owner, id::Object_bit object, id::OperatePtr  operate);

    id::ObjectPtr object;

    friend QDataStream& operator << (QDataStream& stream, Objects& data);
    friend QDataStream& operator >> (QDataStream& stream, Objects& data);
};

class AFlib::transfer::List : public QList <transfer::list::Objects>
{
public:
    explicit List() = default;
    List(const QByteArray& data);

    void addNewObject(const QStringList& dPath, FileType fileType, id::ObjectPtr object);
    void addOperate(const QStringList& dPath, FileType fileType, id::Acc_bit owner, id::Object_bit object, id::OperatePtrList list);
    void addOperate(const QStringList& dPath, FileType fileType, id::Acc_bit owner, id::Object_bit object, id::OperatePtr  operate);

    operator QByteArray() const;
    QByteArray getData() const;

};

#endif // LIB_BASEAF_TRANSFER_OPERATE_LIST_H
