#include "afIdObject.h"
#include "afFile.h"
#include "AfStorage"
#include <QtCore/QDebug>
#include "afFunction.h"

AFlib::id::Object::Object()
{
    setAttribute(Attribute::Created, QDateTime::currentDateTime());
    m_lastUpdate = Function::nullDateTime();
}

AFlib::id::Object::Object(const QByteArray &data)
{
    QDataStream stream(data);
    stream >> *this;
}

AFlib::id::Object::Object(const AFlib::id::Object &cpObject)
    : AFlib::id::Object(cpObject.getData())
{
    //
}

AFlib::id::Object::Object(const AFlib::id::Object *cpObject)
    : AFlib::id::Object(cpObject->getData())
{
    //
}

AFlib::id::Object::Object(id::Account_bit owner, quint8 pluginId, quint8 typeId, QString name, QString descr)
    : AFlib::id::Object_bit(pluginId, typeId)
{
    m_lastUpdate = Function::nullDateTime();
    setAttribute(Attribute::Created, QDateTime::currentDateTime());
    setOwner(owner);
    setName(name);
    setDescription(descr);
}

void AFlib::id::Object::makeGlobalId(Object_bit newId)
{
    setAttribute(Attribute::LocalId, localId());
    setId(newId.toUInt32());
}

AFlib::id::ObjectPtr AFlib::id::Object::createPtr()
{
    ObjectPtr ptr = ObjectPtr::create();
    return ptr;
}

AFlib::id::Global_bit AFlib::id::Object::globalId() const
{
    return Global_bit(owner(), object_b());
}

quint64 AFlib::id::Object::global() const
{
    return globalId().toNumber();
}

QString AFlib::id::Object::name() const
{
    return getAttribute(Attribute::Name).toString();
}

QString AFlib::id::Object::description() const
{
    return getAttribute(Attribute::Description).toString();
}

AFlib::id::Object_bit AFlib::id::Object::localId_b() const
{
    auto last = getAttribute(Attribute::LocalId);
    if (last.isNull())
        return object_b();

    return Object_bit(last.toUInt());
}

quint32 AFlib::id::Object::localId() const
{
    return localId_b().toUInt32();
}

void AFlib::id::Object::setName(const QString &name)
{
    setAttribute(Attribute::Name, name);
}

void AFlib::id::Object::setDescription(const QString &description)
{
    setAttribute(Attribute::Description, description);
}

AFlib::id::Object_bit AFlib::id::Object::parent() const
{
    return Object_bit(getAttribute(Attribute::Parent).toUInt());
}

void AFlib::id::Object::setParent(AFlib::id::Object_bit obj_b)
{
    setAttribute(Attribute::Parent, obj_b.toUInt32());
}

QByteArray AFlib::id::Object::getData() const
{
    return *this;
}

AFlib::SavedIdType AFlib::id::Object::savedStatus()
{
    if (uniqueId() == 0)
        return SavedIdType::Temporary;
    else if (object_b().isIdLocal())
        return SavedIdType::Local;
    return SavedIdType::OnServer;
}

AFlib::id::Object::operator QByteArray() const
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    stream << *this;
    return data;
}

bool AFlib::id::Object::setOwner(const Account_bit &owner)
{
    if (not History::isEmpty() && savedStatus() != SavedIdType::Temporary)
        return false;

    m_owner = owner;
    return true;
}

bool AFlib::id::Object::isAccount() const
{
    return id() == 0;
}

QByteArray AFlib::id::Object::listToBytaArray(const ObjectPtrV list)
{
    QByteArray ret;
    QDataStream stream(&ret, QIODevice::ReadWrite);
    stream << list;
    return ret;
}

namespace AFlib::id {
QDataStream &operator << (QDataStream& stream, const AFlib::id::Object& data)
{
    stream << data.m_owner << static_cast <const History&>(data)
           << static_cast <const Object_bit&> (data);
    QByteArray dat;
    QBuffer buf(&dat);
    buf.open(QIODevice::WriteOnly);
    QDataStream s2(&buf);
    s2 << static_cast <const Object_bit&>(data);
    buf.close();
    return stream;
}

QDataStream &operator >> (QDataStream& stream,       AFlib::id::Object& data)
{
    stream >> data.m_owner >> static_cast <History&>(data)
            >> static_cast <Object_bit&> (data);
    return stream;
}
}

AFlib::id::ObjectPtrV AFlib::id::Object::readList(const QByteArray &data)
{
    AFlib::id::ObjectPtrV retList;
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

AFlib::id::ObjectPtrV AFlib::id::Object::readList(const QByteArray &data, const ObjectPtrV list)
{
    auto removeList = list;
    AFlib::id::ObjectPtrV retList;
    int count;
    QDataStream stream(data);
    stream >> count;
    for (int i = 0; i < count; i++){
        AFlib::id::ObjectPtr ptr = AFlib::id::ObjectPtr::create();
        auto check = [ptr](ObjectPtr it){ return it->globalId() == ptr->globalId(); };
        stream >> *ptr;

        bool isFound = false;
        for (auto it : removeList)
            if (ptr->object_b() == it->object_b() && ptr->owner() == it->owner()){
                retList.push_back(ptr);
                isFound = true;
                break;
            }

        if (isFound)
            removeList.erase(std::remove_if(removeList.begin(), removeList.end(),
                                            [ptr](ObjectPtr it){ return it == ptr; }));

        if (removeList.size() == 0)
            break;
    }
    return retList;
}

AFlib::id::ObjectPtrV AFlib::id::Object::readFromFile(const QStringList &dPath, FileType type)
{
    AFfile file(dPath, type);
    if (not file.openRead())
        return AFlib::id::ObjectPtrV();

    return readList((file.readAll()));
}

AFlib::id::ObjectPtr AFlib::id::Object::readFromFile(const QStringList &dPath, FileType type, const AFlib::id::Obj_bit &id)
{
    AFfile file(dPath, type);
    if (not file.openRead())
        return AFlib::id::ObjectPtr();

    return readList(file.readAll(), id);
}

AFlib::id::ObjectPtrV AFlib::id::Object::readFromFile(const QStringList &dPath, FileType type, const AFlib::id::ObjectPtrV list)
{
    AFfile file(dPath, type);
    if (not file.openRead())
        return AFlib::id::ObjectPtrV();

    return readList(file.readAll(), list);
}

void AFlib::id::Object::saveToStorage(const OperatePtr ptr, bool isId)
{
    afStorage()->addOperate(this, *ptr, isId);
}

bool AFlib::id::Object::setUniqueId()
{
    if (savedStatus() != SavedIdType::Temporary)
        return false;

    auto ow = owner();
    auto pi = pluginId();
    auto ty = type();
    auto afs = afStorage();
    setId(afs->foundFreeLocalId(ow, pi, ty).id());
    return true;
}

QDebug operator <<(QDebug d, const AFlib::id::Object &object)
{
    using namespace AFlib;
    return d << "AFlib::id::Object {\n\t"
             << "owner id:    " << Function::toString(object.owner().toUInt32(), 16).c_str() << ";\n\t"
             << "unique id:   " << Function::toString(object.uniqueId(), 16).c_str() << ";\n\t"
             << "name:        " << object.name() << ";\n\t"
             << "description: " << object.description() << ";\n\t"
             << "type:        " << object.type() << ";\n\t"
             << "plugin:      " << object.pluginId() << ";\n}";
}

bool operator ==(const AFlib::id::ObjectPtr ptr, const AFlib::id::Object &object)
{
    if (ptr.isNull())
        return false;
    return ptr->object_b() == object.object_b() && ptr->owner() == object.owner();
}

bool operator ==(const AFlib::id::ObjectPtr l, const AFlib::id::ObjectPtr r)
{
    return l->globalId() == r->globalId();
}

AFlib::id::ObjectPtrV& operator += (AFlib::id::ObjectPtrV& l, const AFlib::id::ObjectPtrV& r)
{
    l.insert(l.end(), r.begin(), r.end());
    return l;
}
