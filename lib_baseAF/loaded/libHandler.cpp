#include <QtCore/QDebug>

#include "libHandler.h"
#include "function.h"

using namespace AFlib;

LibHandler::LibHandler(QObject* parent) : QObject(parent)
{
    //
}

LibHandler::~LibHandler()
{
    for (auto it = begin(); it != end(); ++it)
        it->clear();
    clear();
}

bool LibHandler::load(QString path, QString sender)
{
    bool loadResult;
    for (auto it = begin(); it != end(); ++it){
        if (it->get()->fileName() == path){
            loadResult = it->get()->load(sender);
            break;
        }
    }

    auto single = QSharedPointer <LibrarySingle>::create(path, this);
    push_back(single);
    loadResult = single->load(sender);
    qInfo() << QString("Try to load %1 by %2, RESULT: ").arg(path).arg(sender) << loadResult;
    return loadResult;
}

bool LibHandler::load(QDir dir, QString name, QString sender)
{
    if (dir.entryList(QDir::Filter::Files).contains(name))
        return load(dir.absoluteFilePath(name), sender);
    qWarning() << QString("File %1 does not exists in %2 directory.").arg(name).arg(dir.path());
    return false;
}

bool LibHandler::load(QDir dir, QVariantHash hash, QString sender)
{
    bool isAllLoadSuccessfull = true;
    // FIXME i don`t know how generate library name, need fix it
    for (auto it = hash.begin(); it != hash.end(); ++it)
        isAllLoadSuccessfull &= load(dir,  AFlib::Function::libraryName(it.key() + "_" + it.value().toString()), sender);
    return isAllLoadSuccessfull;
}

void LibHandler::unload(QString sender)
{
    for (auto it = begin(); it != end(); ++it)
        it->get()->unload(sender);
}
