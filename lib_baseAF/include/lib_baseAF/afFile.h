#ifndef LIB_BASEAF_FILE_H
#define LIB_BASEAF_FILE_H

#include "lib_baseAF/afEnum.h"
#include <QtCore/QDir>
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QDataStream>

namespace AFlib {
    class File;
}

typedef AFlib::File AFfile;

class AFlib::File : protected QFile
{
public:
    File(QString dPath, AFlib::FileType type = AFlib::FileType::Data);
    File(QStringList list, AFlib::FileType type = AFlib::FileType::Data);
    virtual ~File() override;

    bool openRead();
    bool openWrite();

    QByteArray readAll();
    bool writeAll(const QByteArray& data);

    QDataStream& stream();
    bool isOpen() const;
    QStringList dPath() const;
    FileType dType() const;

    static bool rename(QDir dir, QString file, QString newName);

    static QDir    getFullDir (const QStringList& list, bool isFileNameIsLast = true);
    static QString getFullPath(const QStringList& list, AFlib::FileType type = AFlib::FileType::Data);
    static QString getFullPath(const QString& dPath);
    static QString getDPath(const QStringList& list);
    static QString getDPath(const QString& fullPath);
    static QString getDPath(const QDir& fullPath);
    static QStringList getDPathList(const QString& dPath);

private:
    QDataStream m_stream;
    QStringList p_dPath;
    AFlib::FileType p_fileType;
};

template <class T>
AFlib::File& operator << (AFlib::File& file, const T& data)
{
    if (not file.isOpen()){
        qDebug() << "Can't write to unopened file: " << file.dPath();
        return file;
    }
    file.stream() << data;
    return file;
}

template <class T>
AFlib::File& operator >> (AFlib::File& file, T& data)
{
    if (not file.isOpen()){
        qDebug() << "Can't read from unopened file: " << file.dPath();
        return file;
    }
    file.stream() >> data;
    return file;
}

#endif // LIB_BASEAF_FILE_H
