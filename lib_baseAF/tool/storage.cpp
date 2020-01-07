#include "storage.h"
#include "crypt.h"
#include <QtCore/QDebug>

using namespace AFlib;

Storage::Storage(DirPtr dir, QObject *parent) : QObject(parent), m_dir(dir)
{
    //
}

bool Storage::writeData(QDir dir, QString name, QString sufix, QByteArray data, bool crypt, QString user, QString sub1, QString sub2)
{
    if (not user.isEmpty())
        if (not Dir::cdDirectory(dir, user))
            return false;

    if (not sub1.isEmpty())
        if (not Dir::cdDirectory(dir, sub1))
            return false;

    if (not sub2.isEmpty())
        if (not Dir::cdDirectory(dir, sub2))
            return false;

    if (sufix.isEmpty())
        sufix = "afs";

    QFile file(dir.absoluteFilePath(name + "." + sufix));
    if (not file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        qWarning() << QString("File %1 not created.").arg(file.fileName());
        return false;
    }

    if (crypt)
        data = AFlib::crypt(data);
    data.append('a' + rand() % 12 * 2 + (crypt ? 0 : 1));

    bool result = file.write(data) != -1;
    file.close();
    return result;
}

std::optional<QByteArray> Storage::readData(QString filePath)
{
    QFile file(filePath);
    if (not file.exists()){
        qWarning() << QString("File %1 not found.").arg(filePath);
        return std::nullopt;
    }

    QByteArray data = file.readAll();
    if (data.isEmpty()){
        qWarning() << QString("Can`t read %1 file.").arg(filePath);
        return std::nullopt;
    }

    bool crypt = data.back() - 'a';
    if (crypt)
        data = AFlib::encrypt(data);

    return std::move(data);
}

std::optional<QByteArray> Storage::readData(QDir dir, QString name, QString sufix)
{
    return Storage::readData(dir.absoluteFilePath(QString("%1.%2").arg(name, sufix)));
}

std::optional<QByteArray> Storage::readData(QString plugin, QDir dir, QString name, QString sufix)
{
    if (not Dir::cdDirectory(dir, plugin))
        return std::nullopt;

    return readData(dir, name, sufix);
}

bool Storage::writeData(QString name, QByteArray data, QString sufix, bool user, bool crypt)
{
    return Storage::writeData(user ? m_dir->userData() : m_dir->storage(), name, sufix, data, crypt, "", m_subFolder, m_subSubFolder);
}

bool Storage::writeData(QString name, QByteArray data, bool user, bool crypt)
{
    return writeData(name, data, m_sufix, user, crypt);
}

bool Storage::writeData(QString plugin, QString name, QByteArray data, bool user, bool crypt)
{
    return writeData(user ? m_dir->userData() : m_dir->storage(), name, m_sufix, data, crypt, "", plugin);
}

bool Storage::writeData(QString plugin, QString name, QByteArray data, QString sufix, bool user, bool crypt)
{
    return Storage::writeData(user ? m_dir->userData() : m_dir->storage(), name, sufix, data, crypt, "", plugin);
}
