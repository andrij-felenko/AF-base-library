#include "afIdObjectTemplate.h"

AFlib::id::ObjectTemplate::ObjectTemplate(QObject *parent) : QObject(parent)
{
    //
}

AFlib::id::ObjectTemplate::ObjectTemplate(ObjectPtr ptr, QObject *parent)
    : QObject(parent), m_ptr(ptr)
{
    //
}

const AFlib::id::Object *AFlib::id::ObjectTemplate::afObject() const
{
    return m_ptr.get();
}

AFlib::id::ObjectPtr AFlib::id::ObjectTemplate::afObjectPtr() const
{
    return m_ptr;
}
