#ifndef LIB_BASEAF_ID_HISTORY_H
#define LIB_BASEAF_ID_HISTORY_H

#include <QtCore/QDateTime>
#include <QtCore/QSharedPointer>

namespace AFlib {
    enum class Attribute {
        None = 0,
        Name,
        Created,
        Description,
        Date,
        Time,
        LocalId,
        Parent,
        //
        // for account
        Email = 0x10,
        Login,
        Icon,
        Password,
        UserList,
        GroupMembers,         ///< List of members in Group
        FriendList,
        GroupList,
    };

    enum class Compress {
        Shortest,
        AllActive,
        EachByOne,
        Full,
    };
}

namespace AFlib::id {
    class History;
    typedef QSharedPointer <History> HistoryPtr;
}

#include "afIdOperate.h"
#include "afIdObjectBit.h"
#include "afIdGlobalBit.h"

class AFlib::id::History
{
public:
    explicit History();

    void updateTime(QDateTime dTime);
    bool haveUpdates() const;
    bool isEmpty() const;
    QDateTime lastChange() const;
    QDateTime lastUpdate() const;
    QDateTime timeCreate() const;

    void makeFull(const QByteArray& data);
    void makeShorten(OperatePtrList& list);
    void makeShorten();
    void saveShorten();
    void useCompress(Compress value);

    // single part
    QVariant getAttribute(Attribute key) const;
    void     setAttribute(Attribute key, QVariant value);

    template <typename K>
    QVariant getAttribute(K key) const {
        return getAttribute(static_cast <quint16> (key) + 0x40);
    }

    template <typename K>
    void setAttribute(K key, QVariant value)
    {
        setAttribute(static_cast <quint16> (key) + 0x40, value);
    }

    template <typename K>
    Global_bit getIdAttribute(K key) const {
        return getIdAttribute(static_cast <quint16> (key) + 0x40);
    }

    template <typename K>
    void setIdAttribute(K key, const Global_bit& value) {
        setIdAttribute(static_cast <quint16> (key) + 0x40, value);
    }

    // Multi part
    QVariantList getMultiAttribute(Attribute key) const;
    void         setMultiAttribute(Attribute key, QVariant value, HIdType type);

    template <typename K>
    QVariantList getMultiAttribute(K key) const {
        return getMultiAttribute(m_operateList, static_cast <quint16> (key) + 0x40);
    }

    template <typename K>
    GlobalList_bit getMultiIdAttribute(K key) const {
        return getMultiIdAttribute(static_cast <quint16> (key) + 0x40);
    }

    template <typename K>
    void setMultiAttribute(K key, const QVariant& value, HIdType type) {
        setMultiAttribute(static_cast <quint16> (key) + 0x40, value, type);
    }

    template <typename K>
    void setMultiIdAttribute(K key, const Global_bit& value, HIdType type) {
        setMultiIdAttribute(static_cast <quint16> (key) + 0x40, value, type);
    }

    OperatePtrList getAllOperates() const;
    OperatePtrList getIdOperates() const;
    OperatePtrList getNoIdOperates() const;
    OperatePtrList getListAfter  (const QDateTime& afterTime) const;
    OperatePtrList getListIdAfter(const QDateTime& afterTime) const;
    void addOperate(Operate    id, bool saveToStorage = true);
    void addOperate(OperatePtr id, bool saveToStorage = true, bool isId = false);
    void addOperate(const QByteArray& data);
    void addOperations(const QByteArray& list);
    void addOperations(const OperatePtrList list, bool saveToStorage = true, bool isId = false);
    void addOperate(Attribute attributeKey, QVariant value,
                    Account_bit userId, HIdType history = HIdType::AddLine,
                    SIdType saved = SIdType::Local,
                    QDateTime dTime = QDateTime::currentDateTime());
    void addOperate(Attribute attributeKey, QVariant value,
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

protected:
    QDateTime m_lastUpdate;
    OperatePtrList m_operateList;
    OperatePtrList m_operateIdList;
    Account_bit m_owner;

    virtual void saveToStorage(const OperatePtr ptr, bool isId = false) = 0;
    virtual SavedIdType savedStatus() = 0;

    friend QDataStream &operator << (QDataStream& stream, const History& data);
    friend QDataStream &operator >> (QDataStream& stream,       History& data);
    friend class AFlib::id::Object;

private:
    void refreshLastChangeTime();

    Global_bit getIdAttribute(quint16 key);
    void       setIdAttribute(quint16 key, const Global_bit& globalId);

    QVariant getAttribute(quint16 key) const;
    QVariant getAttribute(const OperatePtrList &list, quint16 key) const;
    void setAttribute(quint16 key, const QVariant& value, bool isId = false);

    QVariantList getMultiAttribute(const OperatePtrList &opList, quint16 key) const;
    void setMultiAttribute(quint16 key, const QVariant& value, HIdType type, bool isId = false);

    GlobalList_bit getMultiIdAttribute(quint16 key) const;
    void setMultiIdAttribute(quint16 key, const Global_bit& value, HIdType type);

    void addOperate(quint16 valueKey, QVariant value, Account_bit userId,
                    HistoryIdType history, SavedIdType saved, bool isId = false,
                    QDateTime dTime = QDateTime::currentDateTime());
};

#endif // LIB_BASEAF_ID_HISTORY_H
