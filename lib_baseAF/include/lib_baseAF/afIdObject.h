#ifndef LIB_BASEAF_ID_OBJECT_H
#define LIB_BASEAF_ID_OBJECT_H

#include "afIdHistory.h"
#include "afIdObjectBit.h"

namespace AFlib::id {
    class  Object;

    typedef QSharedPointer <Object> ObjectPtr;
    typedef QList <ObjectPtr> ObjectPtrList;
    typedef Object_bit Obj_bit;
}

class AFlib::id::Object : public History, public ObjectFull_bit
{
public:
    explicit Object();
    Object(const QByteArray& data);
    Object(Account_bit owner, quint16 uniqueId, quint8 type, quint8 pluginId);
    Object(Account_bit owner, QString name, QString descr = QString(""));
    Object(Account_bit owner, QString name, QString descr, quint16 uniqueId, quint8 type, quint8 pluginId,
           quint8 parentType, quint32 parentId);

    void makeGlobalId(quint32 newId);

    virtual QString name()        const final;
    virtual QString description() const final;

    virtual void setName       (const QString& name)        final;
    virtual void setDescription(const QString& description) final;

    Account_bit owner() const { return m_owner; }

    operator QByteArray() const;
    QByteArray getData() const;

    //! Function set owner, only work if history is empty.
    //! \param owner Owner id.
    //! \return Return result of set new id, return false if history is not empty.
    virtual bool setOwner(const Account_bit &owner) final;

protected:
    friend QDataStream &operator << (QDataStream& stream, const Object& data);
    friend QDataStream &operator >> (QDataStream& stream,       Object& data);
};

QDataStream &operator << (QDataStream& stream, const AFlib::id::ObjectPtrList& data);
QDataStream &operator >> (QDataStream& stream,       AFlib::id::ObjectPtrList& data);

#endif // LIB_BASEAF_ID_OBJECT_H
