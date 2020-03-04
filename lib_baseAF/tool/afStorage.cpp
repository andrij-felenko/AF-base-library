#include "afStorage.h"
#include <QtCore/QDebug>

using namespace AFlib;

Storage::Storage(QObject *parent) : QObject(parent), m_storageDir(afDir()->storage())
{
    loadAllData();
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

bool Storage::addObjectId(QDir dir, QString key, const QByteArray &data)
{
    return addObjectId(getFile(dir, key), data);
}

bool Storage::addObjectId(QString fileName, const QByteArray &data)
{
    return addObjectId(fileName, IdObject(data));
}

bool Storage::addObjectId(QDir dir, QString key, const IdObject &object)
{
    return addObjectId(getFile(dir, key), object);
}

bool Storage::addObjectId(QString fileName, const IdObject &object)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)){
        IdObjectPtrList list;
        const QByteArray allData = file.readAll();
        QDataStream stream(allData);
        stream >> list;

        IdObjectPtr ptr = IdObjectPtr::create(object);
        list.push_back(ptr);
        file.close();

        if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
            QDataStream wStream(&file);
            wStream << list;
            file.close();
            return true;
        }
    }
    qDebug() << "Can`t save to file [" << fileName << "], error:" << file.errorString();
    return false;
}

bool Storage::removeObjectId(QDir dir, QString key, const AFIdObject_bit &object)
{
    return removeObjectId(getFile(dir, key), object);
}

bool Storage::removeObjectId(QString fileName, const AFIdObject_bit &object)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)){
        IdObjectPtrList list;
        const QByteArray allData = file.readAll();
        QDataStream stream(allData);
        stream >> list;
        file.close();

        auto newRem = std::remove_if(list.begin(), list.end(), [object](IdObjectPtr single) { return single->id_b() == object; });
        if (newRem != list.end()){
            list.erase(newRem);
            if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
                QDataStream wStream(&file);
                wStream << list;
                file.close();
                return true;
            }
        }
        qDebug() << "Object " << object << " not found.";
    }
    qDebug() << "Can`t save to file [" << fileName << "], error:" << file.errorString();
    return false;
}

bool Storage::updateFile(QDir dir, QString key, const QByteArray &data)
{
    return updateFile(getFile(dir, key), data);
}

bool Storage::updateFile(QString fileName, const QByteArray &data)
{
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
        file.write(data);
        file.close();
        return true;
    }
    qDebug() << "Can`t save to file [" << fileName << "], error:" << file.errorString();
    return false;
}

bool Storage::addOperate(QDir dir, QString key, const AFIdObject_bit &object, const QByteArray &data)
{
    return addOperate(getFile(dir, key), object, data);
}

bool Storage::addOperate(QString fileName, const AFIdObject_bit &object, const QByteArray &data)
{
    return addOperate(fileName, object, IdOperate(data));
}

bool Storage::addOperate(QDir dir, QString key, const AFIdObject_bit &object, const IdOperate &operate)
{
    return addOperate(getFile(dir, key), object, operate);
}

bool Storage::addOperate(QString fileName, const AFIdObject_bit &object, const IdOperate &operate)
{
    IdOperatePtr ptr = IdOperatePtr::create(operate);
    return addOperate(fileName, object, ptr);
}

bool Storage::addOperate(QDir dir, QString key, const AFIdObject_bit &object, const IdOperatePtr &operate)
{
    return addOperate(getFile(dir, key), object, operate);
}

bool Storage::addOperate(QString fileName, const AFIdObject_bit &object, const IdOperatePtr &operate)
{
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)){
        IdObjectPtrList list;
        const QByteArray allData = file.readAll();
        QDataStream stream(allData);
        stream >> list;
        file.close();

        for (auto it : list)
            if (it->id_b() == object){
                it->addOperate(operate);
                if (file.open(QIODevice::WriteOnly | QIODevice::Truncate)){
                    QDataStream wStream(&file);
                    wStream << list;
                    file.close();
                    return true;
                }
            }
        qDebug() << "Object " << object << " not found.";
    }
    qDebug() << "Can`t save to file [" << fileName << "], error:" << file.errorString();
    return false;
}

bool Storage::addOperateList(QDir dir, QString key, const AFIdObject_bit &object, const QByteArray &data)
{
    return addOperateList(getFile(dir, key), object, data);
}

bool Storage::addOperateList(QString fileName, const AFIdObject_bit &object, const QByteArray &data)
{
    IdOperatePtrList list;
    QDataStream stream(data);
    stream >> list;
    return addOperateList(fileName, object, list);
}

bool Storage::addOperateList(QDir dir, QString key, const AFIdObject_bit &object, const IdOperatePtrList &list)
{
    return addOperateList(getFile(dir, key), object, list);
}

bool Storage::addOperateList(QString fileName, const AFIdObject_bit &object, const IdOperatePtrList &list)
{
    bool ret = true;
    for (auto it : list)
        ret &= addOperate(fileName, object, it);
    return ret;
}

IdObjectPtrList Storage::readObjectList(QString fileName, CompressValue compress)
{
    IdObjectPtrList list;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)){
        IdObjectPtrList list;
        const QByteArray allData = file.readAll();
        QDataStream stream(allData);
        stream >> list;
        file.close();

        // compress objects
        for (auto it : list)
            it->useCompress(compress);
    }
    else
        qDebug() << "Can`t read file [" << fileName << "], error:" << file.errorString();

    return list;
}

IdObjectPtr Storage::readObject(QDir dir, QString key, const AFIdObject_bit &object, CompressValue compress)
{
    return readObject(getFile(dir, key), object, compress);
}

IdObjectPtr Storage::readObject(QString fileName, const AFIdObject_bit &object, CompressValue compress)
{
    IdObjectPtrList list;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)){
        IdObjectPtrList list;
        const QByteArray allData = file.readAll();
        QDataStream stream(allData);
        stream >> list;
        file.close();

        // compress objects
        for (auto it : list)
            if (it->id_b() == object){
                it->useCompress(compress);
                return it;
            }
        qDebug() << "Can't find " << object << " in this " << fileName << " file.";
    }
    else
        qDebug() << "Can`t save to file [" << fileName << "], error:" << file.errorString();

    return IdObjectPtr();
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
        IdObjectPtrList list = readObjectList(getFile(dir, it));
        if (list.isEmpty())
            continue;

        for (auto objIt : list){
            addAccount(objIt->owner());
            for (auto accIt : m_storageList)
                if (accIt.accountBit == objIt->owner()){
                    accIt.addPlugin(objIt->pluginId());
                    for (auto pluginId : accIt.pluginList){
                        if (pluginId.pluginId == objIt->pluginId()){
                            pluginId.addSingle(objIt->lastChange(), objIt->uniqueId(), objIt->type());
                            break;
                        }
                    }
                    break;
                }
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
