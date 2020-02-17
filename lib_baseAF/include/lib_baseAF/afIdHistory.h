#ifndef LIB_BASEAF_ID_HISTORY_H
#define LIB_BASEAF_ID_HISTORY_H

#include <QtCore/QDateTime>
#include <QtCore/QSharedPointer>

#include "afIdOperate.h"

namespace AFlib::id {
    class History;
    typedef QSharedPointer <History> HistoryPtr;
}

class AFlib::id::History //: public QObject
{
    //    Q_OBJECT
public:
    explicit History(/*QObject* parent = nullptr*/);

    void updateTime(QDateTime dTime);
    bool haveUpdates() const;
    QDateTime lastChange() const;
    QDateTime lastUpdate() const;

    void addOperation(OperatePtr id);
    std::optional <OperatePtr> getLastOperation() const;

    void makeShorten();
    void makeFull(OperatePtrList list);

protected:
    QDateTime m_lastUpdate;

    friend QDataStream &operator << (QDataStream& stream, const History& data);
    friend QDataStream &operator >> (QDataStream& stream,       History& data);

    //signals:
    //    void needUpdate();
    //    void changed();

private:
    OperatePtrList m_historyList;
};

#endif // LIB_BASEAF_ID_HISTORY_H
