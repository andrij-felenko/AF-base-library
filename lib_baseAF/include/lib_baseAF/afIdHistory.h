#ifndef LIB_BASEAF_ID_HISTORY_H
#define LIB_BASEAF_ID_HISTORY_H

#include <QtCore/QDateTime>
#include <QtCore/QSharedPointer>

#include "afIdOperate.h"

namespace AFlib::id {
    class History;
    typedef QSharedPointer <History> HistoryPtr;
}

class AFlib::id::History
{
public:
    friend class Object;
    friend class Storage;
    explicit History();

    void updateTime(QDateTime dTime);
    bool haveUpdates() const;
    QDateTime lastChange() const;
    QDateTime lastUpdate() const;

    void makeShorten();
    void makeFull(const QByteArray& data);

protected:
    QDateTime m_lastUpdate;

    void refreshLastChangeTime();

    friend QDataStream &operator << (QDataStream& stream, const History& data);
    friend QDataStream &operator >> (QDataStream& stream,       History& data);

private:
    OperatePtrList m_historyList;

    void addOperation(Operate    id);
    void addOperation(OperatePtr id);
};

#endif // LIB_BASEAF_ID_HISTORY_H
