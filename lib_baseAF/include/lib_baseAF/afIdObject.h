#ifndef LIB_BASEAF_ID_OBJECT_H
#define LIB_BASEAF_ID_OBJECT_H

#include "afIdHistory.h"
#include "afIdObjectBit.h"

namespace AFlib::id {
    class Object;
    class ObjectTemplate;

    typedef QSharedPointer <Object> ObjectPtr;
    typedef std::vector <ObjectPtr> ObjectPtrV;
}

class AFlib::id::Object : public AFlib::id::History, public AFlib::id::Object_bit
{
public:
    explicit Object();
    Object(const QByteArray& data);
    Object(const Object& cpObject);
    Object(const Object* cpObject);
    virtual ~Object() = default;
    // TODO need to change it, and all system
    void makeGlobalId(id::Object_bit newId);

    static ObjectPtr createPtr();

    virtual Global_bit globalId() const final;
    virtual quint64    global()   const final;

    virtual QString name()        const final;
    virtual QString description() const final;

    virtual Object_bit localId_b() const final;
    virtual quint32    localId()   const final;

    virtual void setName       (const QString& name)        final;
    virtual void setDescription(const QString& description) final;

    virtual Object_bit parent() const final;
    virtual void setParent(Object_bit obj_b) final;

    Account_bit owner() const { return History::m_owner; }

    operator QByteArray() const;
    QByteArray getData() const;

    SavedIdType savedStatus() override;

    //! Function set owner, only work if history is empty (empty exists create and date time operate).
    //! \param owner Owner id.
    //! \return Return result of set new id, return false if history is not empty.
    virtual bool setOwner(const Account_bit &owner) final;
    virtual bool isAccount() const final;

    static QByteArray listToBytaArray(const ObjectPtrV list);

    static AFlib::id::ObjectPtrV readList(const QByteArray& data);
    static AFlib::id::ObjectPtr    readList(const QByteArray& data, const Obj_bit id);
    static AFlib::id::ObjectPtrV readList(const QByteArray& data, const ObjectPtrV list);

    static AFlib::id::ObjectPtrV readFromFile(const QStringList &dPath, FileType type);
    static AFlib::id::ObjectPtr    readFromFile(const QStringList &dPath, FileType type, const  AFlib::id::Obj_bit& id);
    static AFlib::id::ObjectPtrV readFromFile(const QStringList &dPath, FileType type, const ObjectPtrV list);

protected:
    friend QDataStream &operator << (QDataStream& stream, const Object& data);
    friend QDataStream &operator >> (QDataStream& stream,       Object& data);

private:
    Object(Account_bit owner, quint8 pluginId, quint8 typeId, QString name = "", QString descr = "");
    void saveToStorage(const OperatePtr ptr, bool isId = false) override final;

    bool setUniqueId();

    friend class AFlib::id::ObjectTemplate;
};

QDebug operator << (QDebug d, const AFlib::id::Object& object);
bool operator == (const AFlib::id::ObjectPtr ptr, const AFlib::id::Object& object);
bool operator == (const AFlib::id::ObjectPtr l,   const AFlib::id::ObjectPtr r);

AFlib::id::ObjectPtrV& operator += (AFlib::id::ObjectPtrV& l, const AFlib::id::ObjectPtrV& r);

#endif // LIB_BASEAF_ID_OBJECT_H
