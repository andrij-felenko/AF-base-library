#include "afStorage.h"
#include <QtCore/QDebug>
#include "lib_baseAF/afFile.h"

using namespace AFlib;
static StoragePtr storage_ptr;

Storage::Storage(QObject *parent) : QObject(parent)
{
    //
}

QSharedPointer<AFlib::Storage> Storage::init()
{
    if (storage_ptr.isNull())
        storage_ptr = StoragePtr::create();

    return storage_ptr;
}

StoragePtr AFlib::afStorage()
{
    return Storage::init();
}

void Storage::loadFromDirectory(const QStringList &dPath, Storage::CompressV compress)
{
    QDir dir = AFfile::getFullDir(dPath);
    auto fileList = dir.entryList({"*.afd"}, QDir::Files);
    auto dirList = dir.entryList(QDir::Dirs);
    for (auto it : fileList){
        QStringList fileDPath(dPath);
        fileDPath.push_back(it);
        getObjectList(fileDPath, compress);
    }
    for (auto it : dirList){
        QStringList dirDPath(dPath);
        dirDPath.push_back(it);
        loadFromDirectory(dirDPath, compress);
    }
}

bool Storage::updateFile(const QStringList dPath, const QByteArray &data, FileType type)
{
    AFfile file(dPath);
    if (not file.openWrite())
        return false;

    if (not file.writeAll(data))
        return false;

    removeListByFile(dPath, type);
    auto list = IdObject::readList(data);
    registrateObject(dPath, type, list);
    return true;
}

bool Storage::addOperate(const IdObject &object, const IdOperate &operate)
{
    auto single = findSingle(object);
    if (not single)
        return false;

    auto data = IdObject::readFromFile(single->dPath, single->fileType);
    if (data.isEmpty())
        return false;

    for (auto it : data)
        if (it->owner() == object.owner() && it->objecttU_b() == object.uid_b()){
            AFfile file(single->dPath);
            if (not file.openWrite())
                break;

            it->addOperate(operate);
            return file.writeAll(IdObject::listToBytaArray(data));
        }

    return false;
}

bool Storage::addOperateList(const transfer::List &list)
{
    bool result = true;
    AFlib::transfer::List retList;
    for (auto it : list){
        AFfile file(it.dPath);
        if (not file.openRead()){
            result &= false;
            continue;
        }

        auto readList = IdObject::readList(file.readAll());

        if (not file.openWrite()){
            result &= false;
            continue;
        }
        for (auto subIt : it){
            bool foundSubIt = false;
            for (auto readIt : readList)
                if (readIt->owner() == subIt.owner && readIt->objecttU_b() == subIt.object){
                    readIt->addOperations(subIt);
                    foundSubIt = true;
                    break;
                }
            if (not foundSubIt){
                result &= false;
                qDebug() << "Object not found, can't add " << subIt.object;
            }
        }
        result &= file.writeAll(AFlib::id::Object::listToBytaArray(readList));
    }
    return result;
}

transfer::List Storage::getOperatesAfter(const QDateTime &dateTime)
{
    transfer::List list;
    for (auto aIt : m_storageList)
        for (auto pIt : aIt.pluginList)
            for (auto sIt : pIt.idList)
                if (sIt.lastChange < dateTime){
                    auto single = findSingle(aIt.accountBit, sIt.id);
                    if (single)
                        list.addOperate(single.value().dPath, single->fileType, aIt.accountBit, sIt.id,
                                        getObject(single->dPath, IdObject_bit(sIt.id))->getListAfter(dateTime));
                }
    return list;
}

bool Storage::addObject(const QStringList dPath, const IdObject &object, FileType type)
{
    if (contains(object)){
        qDebug() << "Can't add object: \n" << object << "\nAlready exists.";
        return false;
    }

    AFfile file(dPath, type);
    if (not file.openRead())
        return false;

    auto list = IdObject::readFromFile(dPath, type);
    IdObjectPtr ptr = IdObjectPtr::create(object);
    list.push_back(ptr);

    if (not file.openWrite())
        return false;

    bool result = file.writeAll(IdObject::listToBytaArray(list));
    if (result)
        registrateObject(dPath, type, object);

    return result;
}

bool Storage::addOperateList(transfer::List &operateList)
{
    bool result = true;
    if (operateList.isEmpty())
        return false;

    for (auto fileIt : operateList){
        auto list = IdObject::readFromFile(fileIt.dPath, fileIt.fileType);
        if (list.isEmpty())
            continue;

        for (auto object : fileIt)
            for (auto it : list)
                if (it->owner() == object.owner && it->object_b() == object.object){
                    it->addOperations(object);
                }

        result &= updateFile(fileIt.dPath, IdObject::listToBytaArray(list));
    }

    return result;
}

Storage::IdObj Storage::getObject(const QStringList dPath, const IdObject_bit &object, FileType type, CompressV compress)
{
    return getObject(dPath, object, compress, type);
}

Storage::IdObj Storage::getObject(const QStringList dPath, const IdObject_bit &object, Storage::CompressV compress, FileType type)
{
    auto result = AFlib::id::Object::readFromFile(dPath, type, object);
    if (result.isNull()){
        qDebug() << "Can't find " << &object << " in this " << dPath << " file.";
        return IdObj();
    }

    registrateObject(dPath, type, result);
    result->useCompress(compress);
    return result;
}

Storage::IdObjList Storage::getObjectList(const QStringList dPath, FileType type, CompressV compress)
{
    return getObjectList(dPath, compress, type);
}

Storage::IdObjList Storage::getObjectList(const QStringList dPath, Storage::CompressV compress, FileType type)
{
    auto list = AFlib::id::Object::readFromFile(dPath, type);
    for (auto it : list)
        it->useCompress(compress);

    registrateObject(dPath, type, list);
    return list;
}

Storage::IdObjList Storage::getObjectList(const QStringList dPath, const IdObjectPtrList list, FileType type, CompressV compress)
{
    return getObjectList(dPath, list, compress, type);
}

Storage::IdObjList Storage::getObjectList(const QStringList dPath, const IdObjectPtrList list, Storage::CompressV compress, FileType type)
{
    auto retList = AFlib::id::Object::readFromFile(dPath, type, list);
    for (auto it : retList)
        it->useCompress(compress);

    registrateObject(dPath, type, retList);
    return retList;
}

bool Storage::removeObject(const IdObject &object)
{
    auto single = findSingle(object);
    if (not single)
        return false;

    IdObjectPtrList list = AFlib::id::Object::readFromFile(single->dPath, single->fileType);
    if (list.isEmpty())
        return false;

    auto newEnd = std::remove_if(list.begin(), list.end(), [object](const IdObjectPtr ptr) { return ptr == object; });
    if (newEnd == list.end())
        return false;

    list.erase(newEnd);

    AFfile file(single->dPath);
    if (file.openWrite()){
        file << list;
        return true;
    }

    return false;
}

bool Storage::contains(const IdObject &ptr) const
{
    return contains(ptr.owner(), ptr.uid_b());
}

bool Storage::contains(const IdAccount_bit &account, const IdObjectU_bit &object) const
{
    for (auto aIt : m_storageList)
        if (aIt.accountBit == account)
            for (auto pIt : aIt.pluginList)
                if (pIt.pluginId == object.pluginId())
                    for (auto it : pIt.idList)
                        if (it.id == object)
                            return true;
    return false;
}

std::optional <Storage::SingleStorage> Storage::findSingle(const IdObject &object)
{
    return findSingle(object.owner(), object.objecttU_b());
}

std::optional<Storage::SingleStorage> Storage::findSingle(const IdAccount_bit &account, const IdObjectU_bit &object)
{
    for (auto accIt : m_storageList)
        if (accIt.accountBit == account){

            for (auto pluginIt : accIt.pluginList)
                if (pluginIt.pluginId == object.pluginId()){

                    for (auto singleIt : pluginIt.idList)
                        if (singleIt.id == object)
                            return singleIt;
                    break;
                }

            break;
        }
    return std::nullopt;
}

void Storage::removeListByFile(const QStringList dPath, FileType fileType)
{
    for (auto accIt : m_storageList)
        for (auto it : accIt.pluginList){
            auto end = std::remove_if(it.idList.begin(), it.idList.end(),
                                      [dPath, fileType](const SingleStorage single){ return single.dPath == dPath && single.fileType == fileType; });
            it.idList.erase(end);
        }
}

void Storage::registrateObject(const QStringList dPath, FileType fileType, const IdObject &object)
{
    addAccount(object.owner());
    for (auto accIt : m_storageList)
        if (accIt.accountBit == object.owner()){
            accIt.addPlugin(object.pluginId());
            for (auto pluginId : accIt.pluginList){
                if (pluginId.pluginId == object.pluginId()){
                    pluginId.addSingle(dPath, fileType, object.lastChange(), object.objecttU_b());
                    break;
                }
            }
            break;
        }
}

void Storage::registrateObject(const QStringList dPath, FileType fileType, const IdObjectPtr object)
{
    registrateObject(dPath, fileType, *object);
}

void Storage::registrateObject(const QStringList dPath, FileType fileType, const IdObjectPtrList list)
{
    for (auto it : list)
        registrateObject(dPath, fileType,  it);
}

void Storage::addAccount(IdAccount_bit id)
{
    // not add new if we already have it
    for (auto it : m_storageList)
        if (it.accountBit == id)
            return;

    AccountStorage as;
    as.accountBit = id;

    m_storageList.push_back(as);
}

void Storage::setLastChangedTime(quint32 account, IdObjectU_bit id, QDateTime dTime)
{
    for (auto accIt : m_storageList)
        if (accIt.accountBit.toUInt32() == account){

            for (auto pluginIt : accIt.pluginList)
                if (pluginIt.pluginId == id){

                    for (auto singleIt : pluginIt.idList)
                        if (singleIt.id == id){
                            singleIt.lastChange = dTime;
                            break;
                        }

                    break;
                }

            break;
        }
}

void Storage::PluginStorage::addSingle(QStringList dPath, FileType fileType, QDateTime time, IdObjectU_bit id)
{
    for (auto it : idList)
        if (it.id == id)
            return;

    SingleStorage ss;
    ss.dPath = dPath;
    ss.fileType = fileType;
    ss.id = id;
    ss.lastChange = time;
    idList.push_back(ss);
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
