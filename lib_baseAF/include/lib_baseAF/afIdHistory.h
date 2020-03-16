#ifndef LIB_BASEAF_ID_HISTORY_H
#define LIB_BASEAF_ID_HISTORY_H

#include <QtCore/QDateTime>
#include <QtCore/QSharedPointer>

#include "afIdOperate.h"

namespace AFlib::id {
    class History;
    typedef QSharedPointer <History> HistoryPtr;

    enum class ValueType {
        None = 0,
        Name,
        Description,
        //
        // for account
        Email = 0x10,
        Login,
        Icon,
        Password,
        UserList,
    };

    enum class CompressValue {
        Shortest,
        AllActive,
        EachByOne,
        Full,
    };
}

class AFlib::id::History
{
public:
    explicit History();

    void updateTime(QDateTime dTime);
    bool haveUpdates() const;
    QDateTime lastChange() const;
    QDateTime lastUpdate() const;

    void makeFull(const QByteArray& data);
    void makeShorten();
    void saveShorten();
    void useCompress(CompressValue value);

    // single part
    QVariant getValue(ValueType key) const;
    void setValue(ValueType key, QVariant value);

    template <typename K>
    QVariant getValue(K key) const
    {
        return getValue(static_cast <quint16> (key) + 0x40);
    }

    template <typename K>
    void setValue(K key, QVariant value)
    {
        setValue(static_cast <quint16> (key) + 0x40, value);
    }

    // Multi part
    OperatePtrList getMultiValue(ValueType key) const;
    void setMultiValue(ValueType key, QVariant value, HIdType type);

    template <typename K>
    OperatePtrList getMultiValue(K key) const
    {
        return getMultiValue(static_cast <quint16> (key) + 0x40);
    }

    template <typename K>
    void setMultiValue(K key, const QVariant& value, HIdType type)
    {
        setMultiValue(static_cast <quint16> (key) + 0x40, value, type);
    }

    OperatePtrList getListAfter(const QDateTime& afterTime) const;
    void addOperate(Operate    id);
    void addOperate(OperatePtr id);
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

protected:
    QDateTime m_lastUpdate;
    OperatePtrList m_operateList;
    Account_bit m_owner;

    friend QDataStream &operator << (QDataStream& stream, const History& data);
    friend QDataStream &operator >> (QDataStream& stream,       History& data);

private:
    void refreshLastChangeTime();

    QVariant getValue(quint16 key) const;
    void setValue(quint16 key, const QVariant& value);

    OperatePtrList getMultiValue(quint16 key) const;
    void setMultiValue(quint16 key, const QVariant& value, HIdType type);

    void addOperate(quint16 valueKey, QVariant value, Account_bit userId,
                    HistoryIdType history, SavedIdType saved, QDateTime dTime);
};

#endif // LIB_BASEAF_ID_HISTORY_H
