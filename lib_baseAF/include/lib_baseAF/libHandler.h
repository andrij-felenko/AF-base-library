#ifndef LIB_BASEAF_LIBHANDLER_H
#define LIB_BASEAF_LIBHANDLER_H

#include <QtCore/QDir>
#include <QtCore/QObject>
#include <QtCore/QVariant>

#include "libSingle.h"

namespace AFlib {
    class LibHandler;
    typedef QSharedPointer <LibHandler> LibHandlerPtr;
}

class AFlib::LibHandler : public QObject, public QList <QSharedPointer <LibrarySingle>>
{
    Q_OBJECT
public:
    explicit LibHandler(QObject *parent = nullptr);
    virtual ~LibHandler();

    bool load(QString path, QString sender);
    bool load(QDir dir, QString name, QString sender);
    bool load(QDir dir, QVariantHash hash, QString sender);
    void unload(QString sender);

signals:

public slots:
};

#endif // LIB_BASEAF_LIBHANDLER_H
