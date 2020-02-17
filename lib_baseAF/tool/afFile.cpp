#include "afFile.h"
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtCore/QSharedPointer>

bool AFlib::File::rename(QDir dir, QString file, QString newName)
{
    return QFile::rename(dir.absoluteFilePath(file), dir.absoluteFilePath(newName));
}
