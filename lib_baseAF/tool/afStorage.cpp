#include "afStorage.h"
#include <QtCore/QDebug>
#include "lib_baseAF/afFile.h"
#include "afTransferOperateList.h"
#include "afAccountStorage.h"

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

void Storage::loadFromDirectory(const QStringList &dPath, Compress compress)
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
        if (it->owner() == object.owner() && it->object_b() == object.object_b()){
            AFfile file(single->dPath);
            if (not file.openWrite())
                break;

            it->addOperate(operate, false);
            return file.writeAll(IdObject::listToBytaArray(data));
        }

    return false;
}

bool Storage::addOperateList(const transfer::Send &list)
{
    bool result = true;
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
                if (readIt->owner() == subIt.owner && readIt->object_b() == subIt.object){
                    readIt->addOperations(subIt, false);
                    foundSubIt = true;
                    break;
                }

            // add new object
            if (not foundSubIt & not it.object.isNull()){
                bool isAdd = addObject(it.dPath, *it.object.data(),
                                       findFileTypeByDPath(it.dPath));
                result &= isAdd;
                if (not isAdd)
                    qDebug() << "Object not found, can't add " << &subIt.object;
            }
        }
        result &= file.writeAll(AFlib::id::Object::listToBytaArray(readList));
    }
    return result;
}

transfer::Send Storage::getOperatesAfter(const QDateTime& dateTime, AFaccList_b list)
{
    transfer::Send retList;
    for (auto aIt : m_storageList){
        if (list.isEmpty()){
            if (AFaccount::storage()->getInfo(aIt.accountBit)->afObject()->owner().isLocal())
                continue;
        }
        else if (not list.contains(aIt.accountBit))
            continue;

        bool isAItOwner = list.isEmpty() || (not list.isEmpty() && list.first() == aIt.accountBit);

        for (auto pIt : aIt.pluginList)
            for (auto sIt : pIt.idList)
                if (sIt.lastChange < dateTime){
                    auto single = findSingle(aIt.accountBit, sIt.id);
                    if (single){
                        auto obj = getObject(single->dPath, IdObj_b(sIt.id));
                        // check, if uid != 0 it mean that its not account
                        if (not isAItOwner && obj->isAccount())
                            continue;

                        if (obj->timeCreate() > dateTime)
                            retList.addNewObject(single->dPath, single->fileType, obj);
                        else
                            retList.addOperate(single.value().dPath, single->fileType, aIt.accountBit, sIt.id,
                                            obj->getListAfter(dateTime));
                    }
                }
    }
    return retList;
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

Storage::IdObj Storage::getObject(const QStringList dPath, const IdObject_bit &object, FileType type, Compress compress)
{
    return getObject(dPath, object, compress, type);
}

Storage::IdObj Storage::getObject(const QStringList dPath, const IdObject_bit &object, Compress compress, FileType type)
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

Storage::IdObjList Storage::getObjectList(const QStringList dPath, FileType type, Compress compress)
{
    return getObjectList(dPath, compress, type);
}

Storage::IdObjList Storage::getObjectList(QList<id::Account_bit> accList, quint8 plugin, Compress compress)
{
    QList <QStringList> dPathList;
    for (auto aIt : m_storageList)
        if (accList.contains(aIt.accountBit))
            for (auto pIt : aIt.pluginList)
                if (pIt.pluginId == plugin)
                    for (auto it : pIt.idList)
                        if (not dPathList.contains(it.dPath))
                            dPathList.push_back(it.dPath);

    IdObjList retList;
    for (auto it : dPathList)
        retList += getObjectList(it, compress);
    return retList;
}

Storage::IdObjList Storage::getObjectList(const QStringList dPath, Compress compress, FileType type)
{
    auto list = AFlib::id::Object::readFromFile(dPath, type);
    for (auto it : list)
        it->useCompress(compress);

    registrateObject(dPath, type, list);
    return list;
}

Storage::IdObjList Storage:: getObjectList(const QStringList dPath, const IdObjectPtrList list, FileType type, Compress compress)
{
    return getObjectList(dPath, list, compress, type);
}

Storage::IdObjList Storage::getObjectList(const QStringList dPath, const IdObjectPtrList list, Compress compress, FileType type)
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

bool Storage::updateObjects(transfer::Send &operateList)
{
    return addOperateList(operateList);
}

AFlib::id::Object_bit Storage::foundFreeLocalId(quint8 plugin, quint8 type)
{
    return foundFreeLocalId(AFaccount::user()->afObject()->owner(), plugin, type);
}

AFlib::id::Object_bit Storage::foundFreeLocalId(id::Account_bit account, quint8 plugin, quint8 type)
{
    for (auto aIt : m_storageList)
        if (aIt.accountBit == account)
            for (auto pIt : aIt.pluginList)
                if (pIt.pluginId == plugin)
                    while (true){
                        uint possibleKey = id::Object_bit::createLocalId();
                        bool isFound(false);
                        for (auto sIt : pIt.idList)
                            if (sIt.id.uniqueId() == possibleKey){
                                isFound = true;
                                break;
                            }
                        if (not isFound)
                            return id::Object_bit(plugin, type, possibleKey);
                    }
    return id::Object_bit();
}

FileType Storage::findFileTypeByDPath(QStringList dPath)
{
    for (auto aIt : m_storageList)
        for (auto pIt : aIt.pluginList)
            for (auto it : pIt.idList)
                if (it.dPath == dPath)
                    return it.fileType;
    return FileType::Data;
}

bool Storage::contains(const IdObject &ptr) const
{
    return contains(ptr.owner(), ptr.object_b());
}

bool Storage::contains(const IdAccount_bit &account, const IdObject_bit &object) const
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
    return findSingle(object.owner(), object.object_b());
}

std::optional<Storage::SingleStorage> Storage::findSingle(const IdAccount_bit &account, const IdObject_bit &object)
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
                    pluginId.addSingle(dPath, fileType, object.lastChange(), object.object_b());
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

void Storage::setLastChangedTime(quint32 account, IdObject_bit id, QDateTime dTime)
{
    for (auto accIt : m_storageList)
        if (accIt.accountBit.toUInt32() == account){

            for (auto pluginIt : accIt.pluginList)
                if (pluginIt.pluginId == id.pluginId()){

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

void Storage::PluginStorage::addSingle(QStringList dPath, FileType fileType, QDateTime time, IdObject_bit id)
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
