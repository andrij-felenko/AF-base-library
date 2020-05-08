#include "afIdObject.h"
#include "afFile.h"
#include "AfStorage"
#include <QtCore/QDebug>

AFlib::id::Object::Object() : Object(Account_bit(), "", "", 0, 0, 0, 0, 0)
{
    // it`s done
}

AFlib::id::Object::Object(const QByteArray &data)
{
    QDataStream stream(data);
    stream >> *this;
}

AFlib::id::Object::Object(const Object &cpObject) : AFlib::id::Object(cpObject.getData())
{
    // it`s done
}

AFlib::id::Object::Object(const Object *cpObject) : AFlib::id::Object(cpObject->getData())
{
    // it`s done
}

AFlib::id::Object::Object(Account_bit owner, quint16 uniqueId, quint8 type, quint8 pluginId)
    : Object(owner, "", "", uniqueId, type, pluginId, 0, 0)
{
    // it`s done
}

AFlib::id::Object::Object(Account_bit owner, QString name, QString descr)
    : Object(owner, name, descr, 0, 0, 0, 0, 0)
{
    // it`s done
}

AFlib::id::Object::Object(Account_bit owner, QString name, QString descr,
                          quint16 uniqueId, quint8 type, quint8 pluginId,
                          quint8 parentType, quint32 parentId)
      : Object_bit(ObjU_bit(uniqueId, type, pluginId), parentType, parentId)
{
    setOwner(owner);
    setName(name);
    setDescription(descr);
}

void AFlib::id::Object::makeGlobalId(quint32 newId)
{
    setUId(newId);
}

QString AFlib::id::Object::name() const
{
    return getAttribute(Attribute::Name).toString();
}

QString AFlib::id::Object::description() const
{
    return getAttribute(Attribute::Description).toString();
}

AFlib::id::ObjU_bit AFlib::id::Object::localUid_b() const
{
    return ObjU_bit(localUid());
}

quint32 AFlib::id::Object::localUid() const
{
    auto last = getAttribute(Attribute::LocalId);
    if (last.isNull())
        return uid_b();

    return last.toUInt();
}

void AFlib::id::Object::setName(const QString &name)
{
    setAttribute(Attribute::Name, name);
}

void AFlib::id::Object::setDescription(const QString &description)
{
    setAttribute(Attribute::Description, description);
}

QByteArray AFlib::id::Object::getData() const
{
    return *this;
}

AFlib::id::Object::operator QByteArray() const
{
    QByteArray data;
    QDataStream stream(data);
    stream << *this;
    return data;
}

bool AFlib::id::Object::setOwner(const Account_bit &owner)
{
    if (not m_operateList.isEmpty())
        return false;

    m_owner = owner;
    return true;
}

bool AFlib::id::Object::isAccount() const
{
    return id() == 0;
}

QByteArray AFlib::id::Object::listToBytaArray(const ObjectPtrList list)
{
    QByteArray ret;
    QDataStream stream(&ret, QIODevice::WriteOnly);
    stream << list;
    return ret;
}

namespace AFlib::id {
QDataStream &operator << (QDataStream& stream, const AFlib::id::Object& data)
{
    stream << data.m_owner << static_cast <const History&>(data) << data.m_bitset;
    return stream;
}

QDataStream &operator >> (QDataStream& stream,       AFlib::id::Object& data)
{
    stream >> data.m_owner >> static_cast <History&>(data) >> data.m_bitset;
    return stream;
}
}

QDataStream &operator << (QDataStream& stream, const AFlib::id::ObjectPtrList& data)
{
    stream << data.count();
    for (auto it = data.begin(); it != data.end(); ++it)
        stream << *it->data();
    return stream;
}

QDataStream &operator >> (QDataStream& stream,       AFlib::id::ObjectPtrList& data)
{
    int count;
    stream >> count;
    for (int i = 0; i < count; i++){
        AFlib::id::ObjectPtr ptr = AFlib::id::ObjectPtr::create();
        stream >> *ptr;
        data.push_back(ptr);
    }
    return stream;
}

AFlib::id::ObjectPtrList AFlib::id::Object::readList(const QByteArray &data)
{
    AFlib::id::ObjectPtrList retList;
    QDataStream stream(data);
    stream >> retList;
    return retList;
}

AFlib::id::ObjectPtr AFlib::id::Object::readList(const QByteArray &data, const AFlib::id::Obj_bit id)
{
    int count;
    QDataStream stream(data);
    stream >> count;
    for (int i = 0; i < count; i++){
        AFlib::id::ObjectPtr ptr = AFlib::id::ObjectPtr::create();
        stream >> *ptr;
        if (ptr->object_b() == id)
            return ptr;
    }
    return AFlib::id::ObjectPtr();
}

AFlib::id::ObjectPtrList AFlib::id::Object::readList(const QByteArray &data, const AFlib::id::ObjectPtrList list)
{
    auto removeList = list;
    AFlib::id::ObjectPtrList retList;
    int count;
    QDataStream stream(data);
    stream >> count;
    for (int i = 0; i < count; i++){
        AFlib::id::ObjectPtr ptr = AFlib::id::ObjectPtr::create();
        stream >> *ptr;

        bool isFound = false;
        for (auto it : removeList)
            if (ptr->uid_b() == it->uid_b() && ptr->owner() == it->owner()){
                retList.push_back(ptr);
                isFound = true;
                break;
            }

        if (isFound)
            removeList.removeOne(ptr);

        if (removeList.isEmpty())
            break;
    }
    return retList;
}

AFlib::id::ObjectPtrList AFlib::id::Object::readFromFile(const QStringList &dPath, FileType type)
{
    AFfile file(dPath, type);
    if (not file.openRead())
        return AFlib::id::ObjectPtrList();

    return readList((file.readAll()));
}

AFlib::id::ObjectPtr AFlib::id::Object::readFromFile(const QStringList &dPath, FileType type, const AFlib::id::Obj_bit &id)
{
    AFfile file(dPath, type);
    if (not file.openRead())
        return AFlib::id::ObjectPtr();

    return readList(file.readAll(), id);
}

AFlib::id::ObjectPtrList AFlib::id::Object::readFromFile(const QStringList &dPath, FileType type, const AFlib::id::ObjectPtrList list)
{
    AFfile file(dPath, type);
    if (not file.openRead())
        return AFlib::id::ObjectPtrList();

    return readList(file.readAll(), list);
}

void AFlib::id::Object::saveToStorage(const OperatePtr ptr)
{
    afStorage()->addOperate(this, *ptr);
}

QDebug operator <<(QDebug d, const AFlib::id::Object &object)
{
    return d << "AFlib::id::Object {\n\t"
             << "owner id:     0x" << QString::number(object.owner().toUInt32(), 16)
             << "name:        "    << object.name() << ";\n\t"
             << "description: "    << object.description() << ";\n\t"
             << "unique id:    0x" << QString::number(object.uid(), 16) << ";\n\t"
             << "type:        "    << object.type() << ";\n\t"
             << "plugin:      "    << object.pluginId() << ";\n\n\t"
             << "parent type: "    << object.parentType() << ";\n\t"
             << "parent id:   "    << object.parentId() << ";\n}";
}

bool operator ==(const AFlib::id::ObjectPtr ptr, const AFlib::id::Object &object)
{
    if (ptr.isNull())
        return false;
    return ptr->object_b() == object.object_b() && ptr->owner() == object.owner();
}
