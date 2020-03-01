#ifndef LIB_BASEAF_ID_OBJECT_H
#define LIB_BASEAF_ID_OBJECT_H

#include "afIdHistory.h"
#include "afIdObjectBit.h"

namespace AFlib::id {
    class  Object;

    typedef QSharedPointer <Object> ObjectPtr;
    typedef QList <ObjectPtr> ObjectPtrList;
    typedef Object_bit Obj_bit;

    enum class ValueType {
        None = 0,
        Name,
        Description,
        //
    };
}

class AFlib::id::Object : public ObjectFull_bit
{
public:
    explicit Object();
    Object(QByteArray& data);
    Object(Account_bit owner, quint16 uniqueId, quint8 type, quint8 pluginId);
    Object(Account_bit owner, QString name, QString descr = QString(""));
    Object(Account_bit owner, QString name, QString descr, quint16 uniqueId, quint8 type, quint8 pluginId,
           quint8 parentType, quint32 parentId);

    HistoryPtr history() const;

    virtual QString name()        const final;
    virtual QString description() const final;

    virtual void setName       (const QString& name)        final;
    virtual void setDescription(const QString& description) final;

    Account_bit owner() const { return m_owner; }
    void setOwner(const Account_bit &owner);

    void addOperate(const QByteArray& data);
    void addOperations(const QByteArray& list);
    void addOperate(ValueType valueKey, QVariant value,
                      Account_bit userId, HIdType history = HIdType::AddIdLine,
                      SIdType saved = SIdType::LocaleSaved,
                      QDateTime dTime = QDateTime::currentDateTime());
    void addOperate(ValueType valueKey, QVariant value,
                      quint32 userId, quint8 historyId, quint8 savedId,
                      QDateTime dTime = QDateTime::currentDateTime());

    template <typename K>
    void addOperate(K valueKey, QVariant value,
                      Account_bit userId, HistoryIdType history, SavedIdType saved,
                      QDateTime dTime = QDateTime::currentDateTime())
    {
        addOperate(static_cast <quint16> (valueKey) + 0x40, value, userId, history, saved, dTime);
    }

    template <typename K>
    void addOperate(K valueKey, QVariant value,
                      quint32 userId, quint8 historyId, quint8 savedId,
                      QDateTime dTime = QDateTime::currentDateTime())
    {
        addOperate(static_cast <quint16> (valueKey) + 0x40, value, userId, historyId, savedId, dTime);
    }

    QVariant getValue(ValueType key) const;
    template <typename K>
    QVariant getValue(K key) const
    {
        return getValue(static_cast <quint16> (key) + 0x40);
    }

protected:
    friend QDataStream &operator << (QDataStream& stream, const Object& data);
    friend QDataStream &operator >> (QDataStream& stream,       Object& data);

    HistoryPtr m_history;
    Account_bit m_owner;

    void addOperate(quint16 valueKey, QVariant value, Account_bit userId,
                      HistoryIdType history, SavedIdType saved, QDateTime dTime);
    QVariant getValue(quint16 key) const;

    friend class History;
    friend class Storage;
};

QDataStream &operator << (QDataStream& stream, const AFlib::id::ObjectPtrList& data);
QDataStream &operator >> (QDataStream& stream,       AFlib::id::ObjectPtrList& data);

#endif // LIB_BASEAF_ID_OBJECT_H
