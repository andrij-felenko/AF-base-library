#include "afStorage.h"
#include <QtCore/QDebug>
#include "lib_baseAF/afFile.h"
#include "afTransferOperateList.h"
#include "afAccountStorage.h"

using namespace AFlib;
static StoragePtr storage_ptr;

Storage::Storage(QObject *parent) : QObject(parent)
{
    loadFromDirectory({});
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
    qDebug() << "Read data to af_storage from directory: " << dir.path();
    auto fileList = dir.entryList(File::getFullTypeList(), QDir::Files);
    auto dirList = dir.entryList(QDir::Dirs | QDir::NoDot | QDir::NoDotDot);
    for (auto it : fileList){
        QStringList fileDPath(dPath);
        fileDPath.push_back(it.split(".").first());
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

bool Storage::addOperate(const IdObject &object, const IdOperate &operate, bool isId)
{
    auto single = findSingle(object);
    if (not single)
        return false;

    auto data = IdObject::readFromFile(single->dPath, single->fileType);
    if (data.size() == 0)
        return false;

    for (auto it : data)
        if (it->owner() == object.owner() && it->object_b() == object.object_b()){
            AFfile file(single->dPath);
            if (not file.openWrite())
                break;

            it->addOperate(operate, false);
            // FIXME
            if (isId)
                addLocalShared(object.globalId(), id::Global_bit(operate.value().toULongLong()));
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
                if (readIt->globalId() == subIt.globalId){
                    readIt->addOperations(subIt.operateList, false);
                    readIt->addOperations(subIt.operateIdList, false, true);
                    foundSubIt = true;
                    break;
                }

            // add new object
            if (not foundSubIt & not it.object.isNull()){
                bool isAdd = addObject(it.dPath, *it.object.data(),
                                       findFileTypeByDPath(it.dPath));
                result &= isAdd;
                if (not isAdd)
                    qDebug() << "Object not found, can't add " << &subIt.globalId;
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
                        else {
                            retList.addOperate(single.value().dPath, single->fileType, id::Global_bit(aIt.accountBit, sIt.id),
                                            obj->getListAfter(dateTime), false);
                            retList.addOperate(single.value().dPath, single->fileType, id::Global_bit(aIt.accountBit, sIt.id),
                                               obj->getListIdAfter(dateTime), true);
                        }
                    }
                }
    }
    return retList;
}

bool Storage::addObject(QStringList dPath, const IdObject &object, FileType type)
{
    if (contains(object)){
        qDebug() << "Can't add object: \n" << object << "\nAlready exists.";
        return false;
    }

    if (dPath.isEmpty())
        dPath.push_back(QString::number(object.localId(), 16));

    IdObjectPtrV list;
    AFfile file(dPath, type);
    if (file.exists()){
        if (not file.openRead())
            return false;

        list = IdObject::readFromFile(dPath, type);
    }

    if (not file.openWrite())
        return false;

    IdObjectPtr ptr = IdObjectPtr::create(object);
    list.push_back(ptr);
    auto data = IdObject::listToBytaArray(list);
    bool result = file.writeAll(data);
    qDebug() << "Add Object [result, data]" << result << data;
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

IdObjectPtrV Storage::getObjectList(const QStringList dPath, FileType type, Compress compress)
{
    return getObjectList(dPath, compress, type);
}

IdObjectPtrV Storage::getObjectList(QList<id::Account_bit> accList, quint8 plugin, Compress compress)
{
    QList <QStringList> dPathList;
    for (auto aIt : m_storageList)
        if (accList.contains(aIt.accountBit))
            for (auto pIt : aIt.pluginList)
                if (pIt.pluginId == plugin)
                    for (auto it : pIt.idList)
                        if (not dPathList.contains(it.dPath))
                            dPathList.push_back(it.dPath);

    IdObjectPtrV retList;
    for (auto it : dPathList)
        retList += getObjectList(it, compress);

    return retList;
}

IdObjectPtrV Storage::getObjectList(const QStringList dPath, Compress compress, FileType type)
{
    auto list = AFlib::id::Object::readFromFile(dPath, type);
    for (auto it : list)
        it->useCompress(compress);

    registrateObject(dPath, type, list);
    return list;
}

IdObjectPtrV Storage:: getObjectList(const QStringList dPath, const IdObjectPtrV list, FileType type, Compress compress)
{
    return getObjectList(dPath, list, compress, type);
}

IdObjectPtrV Storage::getObjectList(const QStringList dPath, const IdObjectPtrV list, Compress compress, FileType type)
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

    IdObjectPtrV list = AFlib::id::Object::readFromFile(single->dPath, single->fileType);
    if (list.size() == 0)
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

// from server, only on user side
bool Storage::updateObjects(transfer::Answer answer, transfer::Send &operateList)
{
    // TODO move it to AFapi file
    // first set new global id for all objects,
    for (auto list : answer){
        auto objList = id::Object::readFromFile(list.dPath, list.fileType);
        for (auto it : list)
            for (auto itObj : objList){
                if (itObj->isIdLocal() && itObj->owner() == it.owner)
                    if (itObj->localId_b() == it.object_local){
                        itObj->makeGlobalId(it.object_global);
                        break;
                    }
            }

        // write changes
        AFfile file(list.dPath, list.fileType);
        file.openWrite();
        file.writeAll(IdObject::listToBytaArray(objList));
    }

    // TODO recheck all old localId and change it to Global pointers

    // than only update object by add new operates to them
    return addOperateList(operateList);
}

// saves data on server, please not use it on user side
transfer::Answer Storage::saveObjects(transfer::Send &sendData)
{
    transfer::Answer returnList;
    // TODO move it to AFapi file
    // 1. make all local objects and set global id
    for (auto it : sendData){
        auto obj = it.object;
        if (not obj.isNull()){
            auto newId = foundFreeGlobalId(obj->globalId());
            obj->makeGlobalId(newId);
            returnList.addNewGlobalId(it.dPath, it.fileType, obj->owner(), obj->localId_b(), newId);
        }
    }
    // 2. change shared objects in operate list by new objects ids
    for (auto file : sendData)
        for (auto object : file){
            for (auto operate : object.operateIdList){
                auto newGlobal = returnList.isOldIdPresent(operate->valueId(), file.dPath, file.fileType);
                if (newGlobal)
                    operate->setValueId(newGlobal.value());
            }
        }

    // 3. set objectList to update
    addOperateList(sendData);

    return returnList;
}

id::Object_bit Storage::foundFreeLocalId(quint8 plugin, quint8 type)
{
    return foundFreeLocalId(AFaccount::user()->afObject()->owner(), plugin, type);
}

id::Object_bit Storage::foundFreeLocalId(id::Account_bit account, quint8 plugin, quint8 type)
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

id::Object_bit Storage::foundFreeGlobalId(id::Object_bit id)
{
    return foundFreeGlobalId(id::Global_bit(AFaccount::user()->afObject()->owner(), id));
}

id::Object_bit Storage::foundFreeGlobalId(id::Global_bit id)
{
    for (auto aIt : m_storageList)
        if (aIt.accountBit == id.ownerId)
            for (auto pIt : aIt.pluginList)
                if (pIt.pluginId == id.objectId.pluginId())
                    while (true){
                        uint possibleKey = id::Object_bit::createGlobalId();
                        bool isFound(false);
                        for (auto sIt : pIt.idList)
                            if (sIt.id.uniqueId() == possibleKey){
                                isFound = true;
                                break;
                            }
                        if (not isFound){
                            id::Object_bit ret = id.objectId;
                            ret.setUniqueId(possibleKey);
                            return ret;
                        }
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
    return contains(ptr.globalId());
}

bool Storage::contains(const IdGlobal_bit &globalId) const
{
    for (auto aIt : m_storageList)
        if (aIt.accountBit == globalId.ownerId)
            for (auto pIt : aIt.pluginList)
                if (pIt.pluginId == globalId.objectId.pluginId())
                    for (auto it : pIt.idList)
                        if (it.id == globalId.objectId)
                            return true;
    return false;
}

std::optional <Storage::SingleStorage> Storage::findSingle(const IdObject &object)
{
    return findSingle(object.globalId());
}

std::optional<Storage::SingleStorage> Storage::findSingle(const id::Global_bit &globalId)
{
    for (auto accIt : m_storageList)
        if (accIt.accountBit == globalId.ownerId){

            for (auto pluginIt : accIt.pluginList)
                if (pluginIt.pluginId == globalId.objectId.pluginId()){

                    for (auto singleIt : pluginIt.idList)
                        if (singleIt.id == globalId.objectId)
                            return singleIt;
                    break;
                }

            break;
        }
    return std::nullopt;
}

std::optional<Storage::SingleStorage> Storage::findSingle(const id::Account_bit &owner, const id::Object_bit &object)
{
    return findSingle(IdGlobal_bit(owner, object));
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

void Storage::registrateObject(const QStringList dPath, FileType fileType, const IdObjectPtrV list)
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

void Storage::addLocalShared(IdGlobal_bit sender, IdGlobal_bit whatsAddId)
{
    for (auto accIt : m_storageList)
        if (accIt.accountBit == sender.ownerId)
            for (auto pluginIt : accIt.pluginList)
                if (pluginIt.pluginId == sender.objectId.pluginId())
                    for (auto singleIt : pluginIt.idList)
                        if (singleIt.id == sender.objectId)
                            if (not singleIt.localSharedList.contains(whatsAddId)){
                                singleIt.localSharedList.push_back(whatsAddId);
                                return;
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
