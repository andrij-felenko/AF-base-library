#include "afTransferOperateList.h"

void AFlib::transfer::list::Objects::addOperate(AFlib::id::Account_bit owner, AFlib::id::Object_bit object,
                                                AFlib::id::OperatePtrList list)
{
    for (auto it = begin(); it != end(); ++it)
        if (it->owner == owner && it->object == object){
            *it += list;
            return;
        }

    Operates operate;
    operate += list;
    push_back(operate);
}

void AFlib::transfer::list::Objects::addOperate(AFlib::id::Account_bit owner, AFlib::id::Object_bit object,
                                                AFlib::id::OperatePtr operate)
{
    id::OperatePtrList list;
    list.push_back(operate);
    addOperate(owner, object, list);
}

AFlib::transfer::List::List(const QByteArray &data)
{
    QDataStream stream(data);
    stream >> *this;
}

void AFlib::transfer::List::addOperate(const QStringList &dPath, FileType fileType, AFlib::id::Acc_bit owner,
                                       AFlib::id::ObjU_bit object, AFlib::id::OperatePtrList list)
{
    for (auto it = begin(); it != end(); ++it)
        if (it->dPath == dPath && it->fileType == fileType){
            it->addOperate(owner, object, list);
            return;
        }

    list::Objects file;
    file.dPath = dPath;
    file.fileType = fileType;
    file.addOperate(owner, object, list);
    push_back(file);
}

void AFlib::transfer::List::addOperate(const QStringList &dPath, FileType fileType, AFlib::id::Acc_bit owner,
                                       id::ObjU_bit object, AFlib::id::OperatePtr operate)
{
    id::OperatePtrList list;
    list.push_back(operate);
    addOperate(dPath, fileType, owner, object, list);
}

QByteArray AFlib::transfer::List::getData() const
{
    return *this;
}

AFlib::transfer::List::operator QByteArray() const
{
    QByteArray data;
    QDataStream stream(data);
    stream << *this;
    return data;
}