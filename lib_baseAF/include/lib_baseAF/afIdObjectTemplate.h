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
    ObjectTemplate(quint8 plugin, quint8 type, QObject *parent = nullptr);
    ObjectTemplate(quint8 plugin, quint8 type, QStringList subDPath, QObject *parent = nullptr);
    ObjectTemplate(quint8 plugin, quint8 type, FileType fileType, QStringList subDPath, QObject *parent = nullptr);
    ObjectTemplate(quint8 plugin, quint8 type, FileType fileType, QObject *parent = nullptr);
    ObjectTemplate(ObjectPtr ptr,  QObject *parent = nullptr);
    virtual ~ObjectTemplate() = default;
    const Object* afObject() const;
    ObjectPtr afObjectPtr() const;

    Account_bit owner() const;
    Object_bit object_b() const;

    virtual bool save() final;

protected:
    QStringList m_subDPath;
    FileType m_fileType;
    ObjectPtr m_ptr;
};

#endif
