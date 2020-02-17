#ifndef LIB_BASEAF_LIBSINGLE_H
#define LIB_BASEAF_LIBSINGLE_H

#include <QtCore/QObject>
#include <QtCore/QLibrary>
#include <QtCore/QVariant>

class LibrarySingle : public QLibrary
{
public:
    LibrarySingle(const QString &fileName, QObject *parent = nullptr);

    bool   load(QString sender);
    bool unload(QString sender);
    int count() const;

private:
    QStringList senderList;
};

#endif // LIB_BASEAF_LIBSINGLE_H
