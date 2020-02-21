#include "afStorage.h"
#include <QtCore/QDebug>

using namespace AFlib;

Storage::Storage(QObject *parent) : QObject(parent), m_storageDir(afDir()->storage())
{
    //
}

void Storage::loadAllData()
{
    loadFromDirectory(m_storageDir);
}

void Storage::setPath(QDir dir)
{
    m_storageDir = dir;
    // FIXME add emit if it be QObject
}

bool Storage::writeData(quint32 obj_id, QByteArray data)
{
    return writeData(
        getFile(
            getDir(), obj_id), data);
}

bool Storage::writeData(QString f_path, QByteArray data)
{
    QFile file(f_path);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        auto result = file.write(data);
        if (result == -1)
            qDebug() << "Can`t save to file [" << f_path << "], write return -1.";
        return result;
    }
    else
        qDebug() << "Can`t save to file [" << f_path << "], error:" << file.errorString();
    return false;
}

bool Storage::writeData(QStringList listOfId, QString object, QByteArray data)
{
    return writeData(getFile(getDir(listOfId), object), data);
}

bool Storage::writeData(QList<quint32> list, quint32 obj_id, QByteArray data)
{
    return writeData(getFile(getDir(list), obj_id), data);
}

bool Storage::writeData(quint32 obj_id, IdOperate &data)
{
    return writeData(getFile(getDir(), obj_id), data);
}

bool Storage::writeData(QString f_path, IdOperate &data)
{
    auto objData = readData(f_path);
    if (objData.isNull()){
        qDebug() << "Object not found for: " << f_path;
        return false;
    }
    IdObject obj(objData);
    obj.history()->addOperation(data);
    auto last = obj.history()->getLastOperation();
    if (last.isNull())
        return false;

    setLastChangedTime(obj.owner(), obj.pluginId(), last->type(), last->uniqueId(), last->datetime());
    return true;
}

bool Storage::writeData(QStringList listOfId, QString object, IdOperate &data)
{
    return writeData(getFile(getDir(listOfId), object), data);
}

bool Storage::writeData(QList<quint32> list, quint32 obj_id, IdOperate &data)
{
    return writeData(getFile(getDir(list), obj_id), data);
}

bool Storage::writeData(quint32 obj_id, IdOperatePtr data)
{
    return writeData(obj_id, *data);
}

bool Storage::writeData(QString f_path, IdOperatePtr data)
{
    return writeData(f_path, *data);
}

bool Storage::writeData(QStringList listOfId, QString object, IdOperatePtr data)
{
    return writeData(listOfId, object, *data);
}

bool Storage::writeData(QList<quint32> list, quint32 obj_id, IdOperatePtr data)
{
    return writeData(list, obj_id, *data);
}

bool Storage::writeData(quint32 obj_id, IdOperatePtrList data)
{
    return writeData(getFile(getDir(), obj_id), data);
}

bool Storage::writeData(QString f_path, IdOperatePtrList data)
{
    auto objData = readData(f_path);
    if (objData.isNull()){
        qDebug() << "Object not found for: " << f_path;
        return false;
    }
    IdObject obj(objData);
    for (auto it : data)
        obj.history()->addOperation(it);
    auto last = obj.history()->getLastOperation();
    if (last.isNull())
        return false;

    setLastChangedTime(obj.owner(), obj.pluginId(), last->type(), last->uniqueId(), last->datetime());
    return true;
}

bool Storage::writeData(QStringList listOfId, QString object, IdOperatePtrList data)
{
    return writeData(getFile(getDir(listOfId), object), data);
}

bool Storage::writeData(QList <quint32> list, quint32 obj_id, IdOperatePtrList data)
{
    return writeData(getFile(getDir(list), obj_id), data);
}

QByteArray Storage::readData(quint32 obj_id) const
{
    return readData(getFile(getDir(), obj_id));
}

QByteArray Storage::readData(QString f_path) const
{
    QFile file(f_path);
    if (file.open(QIODevice::ReadOnly))
        return file.readAll();
    else
        qDebug() << "Can`t open file [" << f_path << "], error:" << file.errorString();
    return QByteArray();
}

QByteArray Storage::readData(QStringList listOfId, QString object) const
{
    return readData(getFile(getDir(listOfId), object));
}

QByteArray Storage::readData(QList<quint32> list, quint32 obj_id) const
{
    return readData(getFile(getDir(list), obj_id));
}

QString Storage::getFile(const QDir& dir, quint32 id) const
{
    return getFile(dir, QString::number(id, 16));
}

void Storage::addAccount(IdAccount_bit id)
{
    for (auto it : m_storageList)
        if (it.accountBit == id)
            return;

    AccountStorage as;
    as.accountBit = id;
    m_storageList.push_back(as);
}

void Storage::setLastChangedTime(quint32 account, quint8 plugin,
                                 quint8 typeOfSubject, quint16 uniqueId,
                                 QDateTime dTime)
{
    for (auto accIt : m_storageList)
        if (accIt.accountBit.toUInt32() == account){

            for (auto pluginIt : accIt.pluginList)
                if (pluginIt.pluginId == plugin){

                    for (auto singleIt : pluginIt.idList)
                        if (singleIt.typeOfSubject == typeOfSubject &&
                            singleIt.uniqueId == uniqueId){
                            singleIt.lastChange = dTime;
                            break;
                        }

                    break;
                }

            break;
        }
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

void Storage::loadFromDirectory(QDir &dir)
{
    auto fileList = dir.entryList({"*.afd"}, QDir::Files);
    auto dirList = dir.entryList(QDir::Dirs);
    for (auto it : fileList){
        auto data = readData(getFile(dir, it));
        if (data.isNull())
            continue;
        IdObject obj(data);
        // add this value to m_storageList
        addAccount(obj.owner());
        for (auto accIt : m_storageList)
            if (accIt.accountBit == obj.owner()){
                accIt.addPlugin(obj.pluginId());
                for (auto pluginId : accIt.pluginList){
                    if (pluginId.pluginId == obj.pluginId()){
                        auto last = obj.history()->getLastOperation();
                        if (not last.isNull())
                            pluginId.addSingle(last->datetime(), last->uniqueId(), last->type());
                        break;
                    }
                }
                break;
            }
    }
    for (auto it : dirList){
        QDir subdir = dir;
        AFDir::cdDirectory(subdir, it);
        loadFromDirectory(subdir);
    }
}

QSharedPointer<AFlib::Storage> Storage::init()
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

void Storage::AccountStorage::addPlugin(quint8 id)
{
    for (auto it : pluginList)
        if (it.pluginId == id)
            return;

    PluginStorage ps;
    ps.pluginId = id;
    pluginList.push_back(ps);
}

void Storage::PluginStorage::addSingle(QDateTime time, quint16 id, quint8 type)
{
    for (auto it : idList)
        if (it.uniqueId == id && it.typeOfSubject == type)
            return;

    SingleStorage ss;
    ss.uniqueId = id;
    ss.lastChange = time;
    ss.typeOfSubject = type;
    idList.push_back(ss);
}
