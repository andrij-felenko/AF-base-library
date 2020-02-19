#ifndef LIB_BASEAF_ID_OBJECT_H
#define LIB_BASEAF_ID_OBJECT_H

#include <QtCore/QObject>

#include "afIdInfo.h"
#include "afIdHistory.h"
#include "afIdObjectBit.h"

namespace AFlib::id {
    class  Object;

    typedef QSharedPointer <Object> ObjectPtr;
    typedef QList <ObjectPtr> ObjectPtrList;
    typedef Object_bit Obj_bit;
}

class AFlib::id::Object : public Info, public ObjectFull_bit
{
public:
    explicit Object(QObject* parent = nullptr);
    Object(QByteArray& data);
    Object(Account_bit owner, quint16 uniqueId, quint8 type, quint8 pluginId, QObject* parent = nullptr);
    Object(Account_bit owner, QString name, QString descr = QString(""), QObject* parent = nullptr);
    Object(Account_bit owner, QString name, QString descr, quint16 uniqueId, quint8 type, quint8 pluginId,
           quint8 parentType, quint32 parentId,
           QObject* parent = nullptr);

    HistoryPtr history() const;

    Account_bit owner() const { return m_owner; }
    void setOwner(const Account_bit &owner);

protected:
    friend QDataStream &operator << (QDataStream& stream, const Object& data);
    friend QDataStream &operator >> (QDataStream& stream,       Object& data);

private:
    HistoryPtr m_history;
    Account_bit m_owner;
};

QDataStream &operator << (QDataStream& stream, const AFlib::id::ObjectPtrList& data);
QDataStream &operator >> (QDataStream& stream,       AFlib::id::ObjectPtrList& data);



#endif // LIB_BASEAF_ID_OBJECT_H
