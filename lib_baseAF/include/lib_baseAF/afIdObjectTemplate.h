#ifndef LIB_BASEAF_ID_OBJECT_TEMPLATE_H
#define LIB_BASEAF_ID_OBJECT_TEMPLATE_H

#include <QtCore/QObject>
#include "lib_baseAF/afIdObject.h"

namespace AFlib::id {
    class ObjectTemplate;
}

class AFlib::id::ObjectTemplate : public QObject
{
public:
    explicit ObjectTemplate(QObject *parent = nullptr);
    ObjectTemplate(ObjectPtr ptr,  QObject *parent = nullptr);
    virtual ~ObjectTemplate() = default;
    const Object* afObject() const;
    ObjectPtr afObjectPtr() const;

protected:
    ObjectPtr m_ptr;
};

#endif
