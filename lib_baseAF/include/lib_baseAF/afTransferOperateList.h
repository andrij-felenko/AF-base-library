#ifndef LIB_BASEAF_TRANSFER_OPERATE_LIST_H
#define LIB_BASEAF_TRANSFER_OPERATE_LIST_H

#include "afIdAccount.h"
#include "afIdObjectBit.h"
#include "afIdOperate.h"
#include "afIdObject.h"

namespace AFlib::transfer {
    class Send;
    class Answer;

    namespace send {
        struct Objects;
        struct Operates;
    }

    namespace answer {
        struct Object;
        struct DPathObjectList;
    }
}

// answer struct ------------------------------------------------------------------------
struct AFlib::transfer::answer::Object {
    AFlib::id::Account_bit owner;
    AFlib::id::Object_bit object_local;
    AFlib::id::Object_bit object_global;

    friend QDataStream& operator << (QDataStream& stream, const Object& data);
    friend QDataStream& operator >> (QDataStream& stream,       Object& data);
};

struct AFlib::transfer::answer::DPathObjectList : public QList <Object> {
    QStringList dPath;
    FileType fileType;

    friend QDataStream& operator << (QDataStream& stream, const DPathObjectList& data);
    friend QDataStream& operator >> (QDataStream& stream,       DPathObjectList& data);
};

class AFlib::transfer::Answer : public QList <answer::DPathObjectList>
{
public:
    explicit Answer() = default;
    Answer(const QByteArray& data);

    void addNewGlobalId(QStringList dPath, FileType fileType, id::Account_bit owner,
                        id::Object_bit old, id::Object_bit new_);

    operator QByteArray() const;
    QByteArray getData() const;
};

// send struct --------------------------------------------------------------------------

struct AFlib::transfer::send::Operates : public id::OperatePtrList {
    AFlib::id::Account_bit owner;
    AFlib::id::Object_bit object;

    friend QDataStream& operator << (QDataStream& stream, Operates& data);
    friend QDataStream& operator >> (QDataStream& stream, Operates& data);
};

struct AFlib::transfer::send::Objects : public QList <Operates> {
    QStringList dPath;
    FileType fileType;
    void addOperate(id::Account_bit owner, id::Object_bit object, id::OperatePtrList list);
    void addOperate(id::Account_bit owner, id::Object_bit object, id::OperatePtr  operate);

    id::ObjectPtr object;

    friend QDataStream& operator << (QDataStream& stream, Objects& data);
    friend QDataStream& operator >> (QDataStream& stream, Objects& data);
};

class AFlib::transfer::Send : public QList <transfer::send::Objects>
{
public:
    explicit Send() = default;
    Send(const QByteArray& data);

    void addNewObject(const QStringList& dPath, FileType fileType, id::ObjectPtr object);
    void addOperate(const QStringList& dPath, FileType fileType, id::Acc_bit owner, id::Object_bit object, id::OperatePtrList list);
    void addOperate(const QStringList& dPath, FileType fileType, id::Acc_bit owner, id::Object_bit object, id::OperatePtr  operate);

    operator QByteArray() const;
    QByteArray getData() const;

};

#endif // LIB_BASEAF_TRANSFER_OPERATE_LIST_H
