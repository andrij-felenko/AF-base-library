#include "lib_baseAF/afFile.h"
#include "lib_baseAF/afDir.h"
#include <QtCore/QFile>
#include <QtCore/QDataStream>
#include <QtCore/QSharedPointer>
#include <QtCore/QDebug>

AFlib::File::File(QString dPath, AFlib::FileType type)
    : p_dPath(getDPath(dPath)),
      p_fileType(type)
{
    m_file = new QFile(getFullPath(dPath));
    qDebug() << "File name:" << m_file->fileName() << m_file->exists() << endl;
}

AFlib::File::File(QStringList list, AFlib::FileType type)
    : p_dPath(list),
      p_fileType(type)
{
    m_file = new QFile(getFullPath(list));
    qDebug() << "File name:" << m_file->fileName() << m_file->exists() << endl;
}

AFlib::File::~File()
{
    m_file->close();
    m_file->deleteLater();
}

bool AFlib::File::openRead()
{
    if (not m_file->exists()){
        qDebug() << "File can`t open, file not exists: " << m_file->fileName();
        return false;
    }

    if (m_file->isOpen())
        m_file->close();

    qDebug() << m_file->permissions();
    m_file->setPermissions(QFileDevice::ReadOwner);
    bool success = m_file->open(QIODevice::ReadOnly);
    if (not success)
        qDebug() << "File " << m_file->fileName() << " not open, cause: " << m_file->errorString();

    m_stream.setDevice(m_file);
    return success;
}

bool AFlib::File::openWrite()
{
    if (m_file->isOpen())
        m_file->close();

    qDebug() << m_file->permissions();
    m_file->setPermissions(QFileDevice::WriteOwner | QFileDevice::ReadOwner);
    bool success = m_file->open(QIODevice::WriteOnly | QIODevice::Truncate);
    if (not success)
        qDebug() << "File " << m_file->fileName() << " not open, cause: " << m_file->errorString();

    m_stream.setDevice(m_file);
    return success;
}

bool AFlib::File::exists()
{
    return m_file->exists();
}

QByteArray AFlib::File::readAll()
{
    return m_file->readAll();
}

bool AFlib::File::writeAll(const QByteArray &data)
{
    qDebug() << "writeAll : " << data.size();
    return m_file->write(data) != -1;
}

QDataStream &AFlib::File::stream()
{
    return m_stream;
}

bool AFlib::File::isOpen() const
{
    return m_file->isOpen();
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

QStringList AFlib::File::getFullTypeList()
{
    QStringList list;
    list.push_back("*" + getFileTypeName(FileType::Data));
    list.push_back("*" + getFileTypeName(FileType::Json));
    list.push_back("*" + getFileTypeName(FileType::Config));
    list.push_back("*" + getFileTypeName(FileType::Account));
    list.push_back("*" + getFileTypeName(FileType::Setting));
    return list;
}

QString AFlib::File::getFileTypeName(FileType type)
{
    switch (type) {
        case AFlib::FileType::Data:    return ".afd";
        case AFlib::FileType::Json:    return ".json";
        case AFlib::FileType::Config:  return ".afdc";
        case AFlib::FileType::Account: return ".afda";
        case AFlib::FileType::Setting: return ".afds";
    }
    return "";
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
    return getFullDir(list).path().append("/")
            .append(list.last()).append(getFileTypeName(type));
}

QString AFlib::File::getFullPath(const QString &dPath)
{
    return getFullPath(getDPathList(dPath));
}

QString AFlib::File::getDPath(const QStringList& list)
{
    return QDir::separator() + list.join(QDir::separator());
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
    QStringList list = dPath.split(QDir::separator());
    list.removeOne("");
    return list;
}

QString AFlib::File::getDPath(const QDir& fullPath)
{
    return getDPath(fullPath.path());
}

