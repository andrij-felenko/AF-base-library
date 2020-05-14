#include "lib_baseAF/afFile.h"
#include "lib_baseAF/afDir.h"
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtCore/QSharedPointer>
#include <QtCore/QDebug>

AFlib::File::File(QString dPath, AFlib::FileType type)
    : QFile(getFullPath(dPath)),
    p_dPath(getDPath(dPath)),
    p_fileType(type)
{
    //
}

AFlib::File::File(QStringList list, AFlib::FileType type)
    : QFile(getFullPath(list)),
    p_dPath(list),
    p_fileType(type)
{
    //
}

AFlib::File::~File()
{
    close();
}

bool AFlib::File::openRead()
{
    if (QFile::isOpen())
        close();

    bool success = open(QIODevice::ReadOnly);
    if (not success)
        qDebug() << "File " << fileName() << " not open, cause: " << errorString();

    m_stream.setDevice(this);
    return success;
}

bool AFlib::File::openWrite()
{
    if (QFile::isOpen())
        close();

    bool success = open(QIODevice::WriteOnly | QIODevice::Truncate);
    if (not success)
        qDebug() << "File " << fileName() << " not open, cause: " << errorString();

    m_stream.setDevice(this);
    return success;
}

QByteArray AFlib::File::readAll()
{
    return QFile::readAll();
}

bool AFlib::File::writeAll(const QByteArray &data)
{
    return QFile::write(data) != -1;
}

QDataStream &AFlib::File::stream()
{
    return m_stream;
}

bool AFlib::File::isOpen() const
{
    return QFile::isOpen();
}

QStringList AFlib::File::dPath() const
{
    return p_dPath;
}

AFlib::FileType AFlib::File::dType() const
{
    return p_fileType;
}

bool AFlib::File::rename(QDir dir, QString file, QString newName)
{
    return QFile::rename(dir.absoluteFilePath(file), dir.absoluteFilePath(newName));
}

QDir AFlib::File::getFullDir(const QStringList& list, bool isFileNameIsLast)
{
    QDir dir;
    if (list.isEmpty())
        dir = afDir()->storage();
    else if (list.first() == "accounts" || list.first() == "groups")
        dir = afDir()->users();
    else
        dir = afDir()->storage();

    for (auto it : list)
        if (it != list.last() && isFileNameIsLast)
            AFlib::Dir::cdDirectory(dir, it);
    return dir;
}

QString AFlib::File::getFullPath(const QStringList &list, FileType type)
{
    QString path = getFullDir(list).path();
    path.append("/").append(list.last()).append(".");
    switch (type) {
        case AFlib::FileType::Data:    path.append("afd");  break;
        case AFlib::FileType::Json:    path.append("json"); break;
        case AFlib::FileType::Config:  path.append("afdc"); break;
        case AFlib::FileType::Account: path.append("afda"); break;
        case AFlib::FileType::Setting: path.append("afds"); break;
    }
    return path;
}

QString AFlib::File::getFullPath(const QString &dPath)
{
    return getFullPath(getDPathList(dPath));
}

QString AFlib::File::getDPath(const QStringList& list)
{
    return "/" + list.join("/");
}

QString AFlib::File::getDPath(const QString &fullPath)
{
    auto ret = fullPath;
    auto storageDir = afDir()->storage().path();
    if (fullPath.left(storageDir.length()) != storageDir){
        qDebug() << "Can't convert from " << fullPath;
        return QString("");
    }
    ret.remove(0, storageDir.length());
    return fullPath;
}

QStringList AFlib::File::getDPathList(const QString &dPath)
{
    QStringList list = dPath.split("/");
    list.removeOne("");
    return list;
}

QString AFlib::File::getDPath(const QDir& fullPath)
{
    return getDPath(fullPath.path());
}

