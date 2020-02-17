#include "afLibSingle.h"

LibrarySingle::LibrarySingle(const QString &fileName, QObject *parent)
    : QLibrary(fileName, parent)
{
    //
}

bool LibrarySingle::load(QString sender)
{
    if (senderList.empty()){
        senderList.push_back(sender);
        return QLibrary::load();
    }
    if (not senderList.contains(sender))
        senderList.push_back(sender);
    return false;
}

bool LibrarySingle::unload(QString sender)
{
    senderList.removeOne(sender);
    if (senderList.empty())
        return QLibrary::unload();
    return false;
}

int LibrarySingle::count() const
{
    return senderList.length();
}
