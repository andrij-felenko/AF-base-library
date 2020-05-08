#ifndef LIB_BASEAF_ID_OBJECT_H
#define LIB_BASEAF_ID_OBJECT_H

#include "afIdHistory.h"
#include "afIdObjectBit.h"

namespace AFlib::id {
    class Object;

    typedef QSharedPointer <Object> ObjectPtr;
    typedef QList <ObjectPtr> ObjectPtrList;
}

class AFlib::id::Object : public AFlib::id::History, public AFlib::id::Object_bit
{
public:
    explicit Object();
    Object(const QByteArray& data);
    Object(const Object& cpObject);
    Object(const Object* cpObject);
    // TODO need to change it, and all system
    void makeGlobalId(quint32 newId);

    virtual QString name()        const final;
    virtual QString description() const final;

    virtual Object_bit localId_b() const final;
    virtual quint32    localId()   const final;

    virtual void setName       (const QString& name)        final;
    virtual void setDescription(const QString& description) final;

    Account_bit owner() const { return History::m_owner; }

    operator QByteArray() const;
    QByteArray getData() const;

    //! Function set owner, only work if history is empty.
    //! \param owner Owner id.
    //! \return Return result of set new id, return false if history is not empty.
    virtual bool setOwner(const Account_bit &owner) final;

    static QByteArray listToBytaArray(const ObjectPtrList list);

    static AFlib::id::ObjectPtrList readList(const QByteArray& data);
    static AFlib::id::ObjectPtr     readList(const QByteArray& data, const Obj_bit id);
    static AFlib::id::ObjectPtrList readList(const QByteArray& data, const ObjectPtrList list);

    static AFlib::id::ObjectPtrList readFromFile(const QStringList &dPath, FileType type);
    static AFlib::id::ObjectPtr     readFromFile(const QStringList &dPath, FileType type, const  AFlib::id::Obj_bit& id);
    static AFlib::id::ObjectPtrList readFromFile(const QStringList &dPath, FileType type, const ObjectPtrList list);

protected:
    template <typename Plugin, typename Type>
    Object(Account_bit owner, Plugin plugin, Type type, QString name = "", QString description = "")
        : Object(owner, static_cast <quint8>(plugin) + 1, static_cast <quint8>(type) + 1, name, description)
    {
        //
    }

    friend QDataStream &operator << (QDataStream& stream, const Object& data);
    friend QDataStream &operator >> (QDataStream& stream,       Object& data);

private:
    Object(Account_bit owner, quint8 pluginId, quint8 typeId, QString name, QString descr);
    virtual void saveToStorage(const OperatePtr ptr) override final;
};

QDataStream &operator << (QDataStream& stream, const AFlib::id::ObjectPtrList& data);
QDataStream &operator >> (QDataStream& stream,       AFlib::id::ObjectPtrList& data);
QDebug operator << (QDebug d, const AFlib::id::Object& object);
bool operator == (const AFlib::id::ObjectPtr ptr, const AFlib::id::Object& object);

#endif // LIB_BASEAF_ID_OBJECT_H
