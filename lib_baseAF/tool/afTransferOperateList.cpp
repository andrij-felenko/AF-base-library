#include "afTransferOperateList.h"

void AFlib::transfer::send::Objects::addOperate(id::Global_bit id,
                                                AFlib::id::OperatePtrList list, bool isId)
{
    for (auto it = begin(); it != end(); ++it)
        if (it->globalId == id){
            if (isId)
                it->operateIdList += list;
            else
                it->operateIdList += list;
            return;
        }

    Operates operate(id);
    if (isId)
        operate.operateIdList += list;
    else
        operate.operateIdList += list;
    push_back(operate);
}

void AFlib::transfer::send::Objects::addOperate(id::Global_bit id,
                                                AFlib::id::OperatePtr operate, bool isId)
{
    id::OperatePtrList list;
    list.push_back(operate);
    addOperate(id, list, isId);
}

namespace AFlib::transfer::answer {
    QDataStream &operator <<(QDataStream &stream, const AFlib::transfer::answer::Object &data)
    {
        return stream << data.owner << data.object_local << data.object_global;
    }

    QDataStream &operator >>(QDataStream &stream, AFlib::transfer::answer::Object &data)
    {
        return stream >> data.owner >> data.object_local >> data.object_global;
    }

    QDataStream &operator <<(QDataStream &stream, const AFlib::transfer::answer::DPathObjectList &data)
    {
        return stream << static_cast <const QList <Object>&> (data) << data.dPath << data.fileType;
    }

    QDataStream &operator >>(QDataStream &stream, AFlib::transfer::answer::DPathObjectList &data)
    {
        return stream >> static_cast <QList <Object>&> (data) >> data.dPath >> data.fileType;
    }
}

AFlib::transfer::Answer::Answer(const QByteArray &data)
{
    QDataStream stream(data);
    stream >> *this;
}

void AFlib::transfer::Answer::addNewGlobalId(QStringList dPath, FileType fileType, id::Account_bit owner, id::Object_bit old, id::Object_bit new_)
{
    answer::Object newObj;
    newObj.owner = owner;
    newObj.object_local = old;
    newObj.object_global = new_;

    for (auto it = begin(); it != end(); ++it)
        if (it->dPath == dPath && it->fileType == fileType){
            it->push_back(newObj);
            return;
        }

    answer::DPathObjectList newDPath;
    newDPath.dPath = dPath;
    newDPath.fileType = fileType;
    newDPath.push_back(newObj);
    push_back(newDPath);
}

std::optional<AFlib::id::Global_bit> AFlib::transfer::Answer::isOldIdPresent(AFlib::id::Global_bit localId, QStringList dPath, FileType type) const
{
    if (not localId.objectId.isIdLocal())
        return std::nullopt;

    for (auto it = begin(); it != end(); ++it)
        if (it->dPath == dPath && it->fileType == type)
            for (auto object = it->begin(); object != it->end(); ++it)
                if (object->owner == localId.ownerId)
                    if (object->object_local == localId.objectId)
                        return id::Global_bit(localId.ownerId, object->object_global);

    return std::nullopt;
}

QByteArray AFlib::transfer::Answer::getData() const
{
    return *this;
}

AFlib::transfer::Answer::operator QByteArray() const
{
    QByteArray data;
    QDataStream stream(data);
    stream << *this;
    return data;
}

AFlib::transfer::Send::Send(const QByteArray &data)
{
    QDataStream stream(data);
    stream >> *this;
}

void AFlib::transfer::Send::addNewObject(const QStringList &dPath, FileType fileType, AFlib::id::ObjectPtr object)
{
    transfer::send::Objects newO;
    newO.dPath = dPath;
    newO.fileType = fileType;
    newO.object = object;
    newO.addOperate(object->globalId(), object->getIdOperates(), true);
    newO.addOperate(object->globalId(), object->getNoIdOperates(), false);
    push_back(newO);
}

void AFlib::transfer::Send::addOperate(const QStringList &dPath, FileType fileType, id::Global_bit id,
                                       AFlib::id::OperatePtrList list, bool isId)
{
    for (auto it = begin(); it != end(); ++it)
        if (it->dPath == dPath && it->fileType == fileType){
            it->addOperate(id, list, isId);
            return;
        }

    send::Objects file;
    file.dPath = dPath;
    file.fileType = fileType;
    file.addOperate(id, list, isId);
    push_back(file);
}

void AFlib::transfer::Send::addOperate(const QStringList &dPath, FileType fileType, id::Global_bit id,
                                       AFlib::id::OperatePtr operate, bool isId)
{
    id::OperatePtrList list;
    list.push_back(operate);
    addOperate(dPath, fileType, id, list, isId);
}

QByteArray AFlib::transfer::Send::getData() const
{
    return *this;
}

AFlib::transfer::Send::operator QByteArray() const
{
    QByteArray data;
    QDataStream stream(data);
    stream << *this;
    return data;
}

namespace AFlib::transfer::send {
    QDataStream& operator >>(QDataStream &stream, Objects &data)
    {
        bool isObject;
        stream >> static_cast <QList <Operates>&> (data) >> isObject;
        if (isObject){
            data.object = id::ObjectPtr::create();
            stream >> *data.object;
        }
        return stream >> data.dPath >> data.fileType;
    }

    QDataStream& operator <<(QDataStream &stream, Objects &data)
    {
        stream << static_cast <QList <Operates>&>(data) << (not data.object.isNull());
        if (not data.object.isNull())
            stream << *data.object;
        return stream << data.dPath << data.fileType;
    }

    QDataStream &operator <<(QDataStream &stream, const AFlib::transfer::send::Operates &data)
    {
        return stream << data.globalId << data.operateList << data.operateIdList;
    }

    QDataStream &operator >>(QDataStream &stream, AFlib::transfer::send::Operates &data)
    {
        return stream >> data.globalId >> data.operateList >> data.operateIdList;
    }
}
