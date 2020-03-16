#include "afStorage.h"
#include <QtCore/QDebug>

using namespace AFlib;

Storage::Storage(QObject *parent) : QObject(parent)
{
    if (m_storageDir == QDir::current())
        m_storageDir = afDir()->storage();

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

bool Storage::addObjectId(const QString &key, const QByteArray &data)
{
    return p_addObjectId(getFile(m_storageDir, key), data);
}

bool Storage::addObjectId(const QString &key, const IdObject &object)
{
    return p_addObjectId(getFile(m_storageDir, key), object);
}

bool Storage::addObjectId(const QDir &dir, const QString &key, const QByteArray &data)
{
    return p_addObjectId(getFile(dir, key), data);
}

bool Storage::addObjectId(const QDir &dir, const QString &key, const IdObject &object)
{
    return p_addObjectId(getFile(dir, key), object);
}

bool Storage::removeObjectId(const QString &key, const AFIdObject_bit &object)
{
    return p_removeObjectId(getFile(m_storageDir, key), object);
}

bool Storage::removeObjectId(const QDir &dir, const QString &key, const AFIdObject_bit &object)
{
    return p_removeObjectId(getFile(dir, key), object);
}

bool Storage::updateFile(const QString &key, const QByteArray &data)
{
    return p_updateFile(getFile(m_storageDir, key), data);
}

bool Storage::updateFile(const QDir &dir, const QString &key, const QByteArray &data)
{
    return p_updateFile(getFile(dir, key), data);
}

bool Storage::addOperate(const QString &key, const AFIdObject_bit &object, const QByteArray &data)
{
    return p_addOperate(getFile(m_storageDir, key), object, data);
}

bool Storage::addOperate(const QDir &dir, const QString &key, const AFIdObject_bit &object, const QByteArray &data)
{
    return p_addOperate(getFile(dir, key), object, data);
}

bool Storage::addOperate(const QString &key, const AFIdObject_bit &object, const IdOperate &operate)
{
    return p_addOperate(getFile(m_storageDir, key), object, operate);
}

bool Storage::addOperate(const QDir &dir, const QString &key, const AFIdObject_bit &object, const IdOperate &operate)
{
    return p_addOperate(getFile(dir, key), object, operate);
}

bool Storage::addOperate(const QString &key, const AFIdObject_bit &object, const IdOperatePtr &operate)
{
    return p_addOperate(getFile(m_storageDir, key), object, operate);
}

bool Storage::addOperate(const QDir &dir, const QString &key, const AFIdObject_bit &object, const IdOperatePtr &operate)
{
    return p_addOperate(getFile(dir, key), object, operate);
}

bool Storage::addOperateList(const QString &key, const AFIdObject_bit &object, const QByteArray &data)
{
    return p_addOperateList(getFile(m_storageDir, key), object, data);
}

bool Storage::addOperateList(const QDir &dir, const QString &key, const AFIdObject_bit &object, const QByteArray &data)
{
    return p_addOperateList(getFile(dir, key), object, data);
}

bool Storage::addOperateList(const QString &key, const AFIdObject_bit &object, const IdOperatePtrList &list)
{
    return p_addOperateList(getFile(m_storageDir, key), object, list);
}

bool Storage::addOperateList(const QDir &dir, const QString &key, const AFIdObject_bit &object, const IdOperatePtrList &list)
{
    return p_addOperateList(getFile(dir, key), object, list);
}

IdObjectPtrList Storage::readObjectList(const QDir &dir, bool recurcion, CompressValue compress)
{
    auto list = readObjectList(dir,compress);
    if (not recurcion)
        return list;

    for (auto it : dir.entryList(QDir::Dirs | QDir::NoDot | QDir::NoDotDot))
        list += readObjectList(QDir(dir.path() + "/" + it), recurcion, compress);
    return list;
}

IdObjectPtrList Storage::readObjectList(const QDir &dir, CompressValue compress)
{
    IdObjectPtrList list;
    for (auto it : dir.entryList({"*.afd"}, QDir::Files))
        list += p_readObjectList(it, compress);
    return list;
}

IdObjectPtrList Storage::readObjectList(const QDir &dir, const QString &key, const QList<AFIdObject_bit> list, CompressValue value)
{
    return p_readObjectList(getFile(dir, key), list, value);
}

IdObjectPtrList Storage::readObjectList(const QString &key, const QList<AFIdObject_bit> list, CompressValue value)
{
    return p_readObjectList(getFile(m_storageDir, key), list, value);
}

IdObjectPtr Storage::readObject(const QDir &dir, const QString &key, const AFIdObject_bit &object, CompressValue compress)
{
    return p_readObject(getFile(dir, key), object, compress);
}

IdObjectPtr Storage::readObject(const QString &key, const AFIdObject_bit &object, CompressValue compress)
{
    return p_readObject(getFile(m_storageDir, key), object, compress);
}

QString Storage::getFile(const QDir& dir, quint32 id) const
{
    return getFile(dir, QString::number(id, 16));
}

void Storage::addOperateToStorageList(const QString& filePath, const IdObjectPtr object)
{
    addAccount(object->owner());
    for (auto accIt : m_storageList)
        if (accIt.accountBit == object->owner()){
            accIt.addPlugin(object->pluginId());
            for (auto pluginId : accIt.pluginList){
                if (pluginId.pluginId == object->pluginId()){
                    pluginId.addSingle(filePath, object->lastChange(), object->uniqueId(), object->type());
                    break;
                }
            }
            break;
        }
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
        AFlib::Dir::cdDirectory(dir, it);
    return dir;
}

QDir Storage::getDir(QList<quint32> list) const
{
    QStringList strList = QStringList();
    for (auto it : list)
        strList.push_back(QString::number(it, 16));
    return getDir(strList);
}

QDir Storage::getDir(const QString &name) const
{
    return getDir(QStringList(name));
}

void Storage::loadFromDirectory(QDir &dir)
{
    auto fileList = dir.entryList({"*.afd"}, QDir::Files);
    auto dirList = dir.entryList(QDir::Dirs);
    for (auto it : fileList){
        QString filePath(getFile(dir, it));
        IdObjectPtrList list = p_readObjectList(filePath);
        if (list.isEmpty())
            continue;

        for (auto objIt : list)
            addOperateToStorageList(filePath, objIt);
    }
    for (auto it : dirList){
        QDir subdir = dir;
        AFlib::Dir::cdDirectory(subdir, it);
        loadFromDirectory(subdir);
    }
}

bool Storage::p_addObjectId(QString fileName, const QByteArray &data)
{
    return p_addObjectId(fileName, IdObject(data));
}

bool Storage::p_addObjectId(QString fileName, const IdObject &object)
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
            addOperateToStorageList(file.fileName(), ptr);
            return true;
        }
    }
    qDebug() << "Can`t save to file [" << fileName << "], error:" << file.errorString();
    return false;
}

bool Storage::p_removeObjectId(QString fileName, const AFIdObject_bit &object)
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
            if (list.isEmpty()){
                // remove file, we don't need it anymore
                file.close();
                return file.remove();
            }
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

bool Storage::p_updateFile(QString fileName, const QByteArray &data)
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

bool Storage::p_addOperate(QString fileName, const AFIdObject_bit &object, const QByteArray &data)
{
    return p_addOperate(fileName, object, IdOperate(data));
}

bool Storage::p_addOperate(QString fileName, const AFIdObject_bit &object, const IdOperate &operate)
{
    IdOperatePtr ptr = IdOperatePtr::create(operate);
    return p_addOperate(fileName, object, ptr);
}

bool Storage::p_addOperate(QString fileName, const AFIdObject_bit &object, const IdOperatePtr &operate)
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

bool Storage::p_addOperateList(QString fileName, const AFIdObject_bit &object, const QByteArray &data)
{
    IdOperatePtrList list;
    QDataStream stream(data);
    stream >> list;
    return p_addOperateList(fileName, object, list);
}

bool Storage::p_addOperateList(QString fileName, const AFIdObject_bit &object, const IdOperatePtrList &list)
{
    bool ret = true;
    for (auto it : list)
        ret &= p_addOperate(fileName, object, it);
    return ret;
}

IdObjectPtrList Storage::p_readObjectList(QString fileName, CompressValue compress) const
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

    return list;//
}

IdObjectPtrList Storage::p_readObjectList(QString fileName, QList<AFIdObject_bit> list, CompressValue compress) const
{
    IdObjectPtrList retList;
    QFile file(fileName);
    if (file.open(QIODevice::ReadOnly)){
        IdObjectPtrList dataList;
        QDataStream stream(file.readAll());
        stream >> dataList;
        file.close();

        // compress objects
        for (auto it : list)
            for (auto dataIt : dataList)
                if (dataIt->id_b() == it){
                    dataIt->useCompress(compress);
                    retList += dataIt;
                    break;
                }
    }
    else
        qDebug() << "Can`t save to file [" << fileName << "], error:" << file.errorString();

    return retList;
}

IdObjectPtr Storage::p_readObject(QString fileName, const AFIdObject_bit &object, CompressValue compress) const
{
    auto result = p_readObjectList(fileName, { object }, compress);
    if (result.isEmpty()){
        qDebug() << "Can't find " << object << " in this " << fileName << " file.";
        return IdObjectPtr();
    }

    return result.first();
}

IdOperatePtrList Storage::p_getOperateListAfter(const QString &fileName, const AFIdObject_bit &object, const QDateTime &afterDate) const
{
    // TODO
}

QSharedPointer<AFlib::Storage> Storage::init()
{
    static StoragePtr storage_ptr;
    if (storage_ptr.isNull())
        storage_ptr = StoragePtr::create();

    return storage_ptr;
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

void Storage::PluginStorage::addSingle(QString filePath, QDateTime time, quint16 id, quint8 type)
{
    for (auto it : idList)
        if (it.uniqueId == id && it.typeOfSubject == type)
            return;

    SingleStorage ss;
    ss.filePath = filePath;
    ss.uniqueId = id;
    ss.lastChange = time;
    ss.typeOfSubject = type;
    idList.push_back(ss);
}

StoragePtr afStorage()
{
    return Storage::init();
}
