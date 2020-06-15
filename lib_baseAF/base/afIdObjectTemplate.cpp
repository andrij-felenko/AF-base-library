#include "afIdObjectTemplate.h"
#include <AFaccount/AfAccountStorage>
#include <AFbase/AfStorage>

AFlib::id::ObjectTemplate::ObjectTemplate(quint8 plugin, quint8 type, QObject *parent)
    : ObjectTemplate(plugin, type, FileType::Data, {}, parent)
{
    //
}

AFlib::id::ObjectTemplate::ObjectTemplate(quint8 plugin, quint8 type, QStringList subDPath, QObject *parent)
    : ObjectTemplate(plugin, type, FileType::Data, subDPath, parent)
{
    //
}

// main constructor, other constructor call it for create this object
AFlib::id::ObjectTemplate::ObjectTemplate(quint8 plugin, quint8 type, FileType fileType, QStringList subDPath, QObject *parent)
    : QObject(parent), m_subDPath(subDPath), m_fileType(fileType)
{
    Object obj(AFaccount::storage()->user()->owner(), plugin, type);
    m_ptr = ObjectPtr::create(obj);
}

AFlib::id::ObjectTemplate::ObjectTemplate(quint8 plugin, quint8 type, FileType fileType, QObject *parent)
    : ObjectTemplate(plugin, type, fileType, {}, parent)
{
    //
}

AFlib::id::ObjectTemplate::ObjectTemplate(ObjectPtr ptr, QObject *parent)
    : QObject(parent), m_ptr(ptr)
{
    if (ptr.isNull())
        m_ptr = Object::createPtr();
}

const AFlib::id::Object *AFlib::id::ObjectTemplate::afObject() const
{
    return m_ptr.get();
}

AFlib::id::ObjectPtr AFlib::id::ObjectTemplate::afObjectPtr() const
{
    return m_ptr;
}

AFlib::id::Account_bit AFlib::id::ObjectTemplate::owner() const
{
    return m_ptr->owner();
}

AFlib::id::Object_bit AFlib::id::ObjectTemplate::object_b() const
{
    return m_ptr->object_b();
}

bool AFlib::id::ObjectTemplate::save()
{
    auto cp = m_ptr;
    bool trySave = m_ptr->setUniqueId();
    if (trySave)
        AFlib::afStorage()->addObject(m_subDPath, *m_ptr.data(), m_fileType);
    return trySave;
}
