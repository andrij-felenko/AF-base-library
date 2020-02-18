#include "afStorage.h"
#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>

using namespace AFlib;

Storage::Storage() : QObject(qApp), m_storageDir(afDir()->storage())
{
    //
}

void Storage::loadAllData()
{
//    auto dir = getDestinationDir(account, plugin);
//    auto fileList = dir.entryList({"*.afd"});
//    for (auto it : fileList){
//        QFile file(dir.absoluteFilePath(it));
//        if (not file.open(QIODevice::ReadOnly)){
//            qDebug() << "File " << it << " can`t open: " << file.errorString();
//            continue;
//        }
//        //
    //    }
}

void Storage::setPath(QDir dir)
{
    m_storageDir = dir;
    // FIXME add emit if it be QObject
}

bool Storage::writeData(quint32 obj_id, QByteArray data, QDateTime dTime)
{
    return writeData(getFile(getDir(), obj_id), data, dTime);
}

bool Storage::writeData(QString f_path, QByteArray data, QDateTime dTime)
{
    // main write data
}

bool Storage::writeData(QStringList listOfId, QString object, QByteArray data, QDateTime dTime)
{
    return writeData(getFile(getDir(listOfId), object), data, dTime);
}

bool Storage::writeData(QList<quint32> list, quint32 obj_id, QByteArray data, QDateTime dTime)
{
    return writeData(getFile(getDir(list), obj_id), data, dTime);
}

bool Storage::writeData(quint32 obj_id, IdOperate &data)
{
    //
}

bool Storage::writeData(QString f_path, IdOperate &data)
{
    //
}

bool Storage::writeData(QStringList listOfId, QString object, IdOperate &data)
{
    //
}

bool Storage::writeData(QList<quint32> list, quint32 obj_id, IdOperate &data)
{
    //
}

bool Storage::writeData(quint32 obj_id, IdOperatePtr data)
{
    //
}

bool Storage::writeData(QString f_path, IdOperatePtr data)
{
    //
}

bool Storage::writeData(QStringList listOfId, QString object, IdOperatePtr data)
{
    //
}

bool Storage::writeData(QList<quint32> list, quint32 obj_id, IdOperatePtr data)
{
    //
}

bool Storage::writeData(quint32 obj_id, IdOperatePtrList data)
{
    //
}

bool Storage::writeData(QString f_path, IdOperatePtrList data)
{
    //
}

bool Storage::writeData(QStringList listOfId, QString object, IdOperatePtrList data)
{
    //
}

bool Storage::writeData(QList <quint32> list, quint32 obj_id, IdOperatePtrList data)
{
    //
}

QByteArray Storage::readData(quint32 obj_id) const
{
    //
}

QByteArray Storage::readData(QString f_path) const
{
    //
}

QByteArray Storage::readData(QStringList listOfId, QString object) const
{
    //
}

QByteArray Storage::readData(QList<quint32> list, quint32 obj_id) const
{
    //
}

QString Storage::getFile(const QDir& dir, quint32 id) const
{
    return getFile(dir, QString::number(id, 16));
}

QString Storage::getFile(const QDir &dir, QString name) const
{
    return dir.absoluteFilePath(name + ".afd");
}

QDir Storage::getDir(QStringList listOfId) const
{
    QDir dir = m_storageDir;
    for (auto it : listOfId)
        AFDir::cdDirectory(dir, it);
    return dir;
}

QDir Storage::getDir(QList<quint32> list) const
{
    QStringList strList = QStringList();
    for (auto it : list)
        strList.push_back(QString::number(it, 16));
    return getDir(strList);
}

StoragePtr Storage::init()
{
    static StoragePtr storage_ptr;
    if (storage_ptr.isNull())
        storage_ptr = StoragePtr::create();

    return storage_ptr;
}

StoragePtr afStorage()
{
    return Storage::init();
}
