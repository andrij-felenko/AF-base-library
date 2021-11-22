#ifndef LIB_BASEAF_STORAGE
#define LIB_BASEAF_STORAGE

#include <QtCore/QDateTime>
#include "afDir.h"
#include "afId.h"
#include <QtCore/QCoreApplication>

namespace AFlib {
    class Storage;
    typedef QSharedPointer <Storage> StoragePtr;

    StoragePtr afStorage();
}

class AFlib::Storage final : public QObject
{
    Q_OBJECT
    typedef IdObjectPtr     IdObj;
    typedef AFIdObject_bit  IdObj_b;

public:
    explicit Storage(QObject* parent = qApp);
    static QSharedPointer <Storage> init();

    // TODO add refresh object function, send object and read all new to it file
    void loadFromDirectory(const QStringList& dPath, Compress compress = Compress::AllActive);
    bool updateFile(const QStringList dPath, const QByteArray &data, AFlib::FileType type = AFlib::FileType::Data);
    bool contains(const IdObject& ptr) const;
    bool contains(const IdGlobal_bit& globalId) const;

    bool addOperate(const IdObject& object, const IdOperate& operate, bool isId = false);
    bool addOperateList(const transfer::Send &list);

    transfer::Send getOperatesAfter(const QDateTime& dateTime, AFaccList_b list = AFaccList_b());

    bool addObject(QStringList dPath, const IdObject& object, AFlib::FileType type = AFlib::FileType::Data);

    IdObj     getObject    (const QStringList dPath, const IdObject_bit &object, Compress compress  = Compress::AllActive, AFlib::FileType type = AFlib::FileType::Data);
    IdObj     getObject    (const QStringList dPath, const IdObject_bit &object, AFlib::FileType type, Compress compress = Compress::AllActive);

    IdObjectPtrV getObjectList(const QStringList dPath,                        Compress compress = Compress::AllActive, AFlib::FileType type = AFlib::FileType::Data);
    IdObjectPtrV getObjectList(const QStringList dPath, AFlib::FileType type,  Compress compress = Compress::AllActive);
    IdObjectPtrV getObjectList(QList <id::Account_bit> accList, quint8 plugin, Compress compress = Compress::AllActive);

    IdObjectPtrV getObjectList(const QStringList dPath, const IdObjectPtrV list, Compress compress  =  Compress::AllActive, AFlib::FileType type = AFlib::FileType::Data);
    IdObjectPtrV getObjectList(const QStringList dPath, const IdObjectPtrV list, AFlib::FileType type, Compress compress = Compress::AllActive);

    bool removeObject(const IdObject& object);

    // from server, only used
    bool updateObjects(transfer::Answer answer, transfer::Send &operateList);

    // set data to server, only on server
    transfer::Answer saveObjects(transfer::Send &sendData);

    id::Object_bit foundFreeLocalId(quint8 plugin, quint8 type);
    id::Object_bit foundFreeLocalId(AFlib::id::Account_bit account, quint8 plugin, quint8 type);

    id::Object_bit foundFreeGlobalId(id::Object_bit id);
    id::Object_bit foundFreeGlobalId(id::Global_bit id);

private:
    struct SingleStorage {
        QDateTime lastChange;
        IdObject_bit  id;
        QStringList dPath;
        FileType fileType;

        QList <id::Global_bit> localSharedList;
    };

    struct PluginStorage {
        quint8 pluginId;

        std::vector <SingleStorage> idList;
        void addSingle(QStringList dPath, FileType fileType, QDateTime time, IdObject_bit id);
    };

    struct AccountStorage {
        id::Account_bit accountBit;

        std::vector <PluginStorage> pluginList;
        void addPlugin(quint8 id);
    };

    FileType findFileTypeByDPath(QStringList dPath);
    std::optional <SingleStorage> findSingle(const IdObject &object);
    std::optional <SingleStorage> findSingle(const id::Global_bit& globalId);
    std::optional <SingleStorage> findSingle(const id::Account_bit& owner, const id::Object_bit& object);
    void removeListByFile(const QStringList dPath, FileType fileType);
    void registrateObject(const QStringList dPath, FileType fileType, const IdObject&     object);
    void registrateObject(const QStringList dPath, FileType fileType, const IdObjectPtr   object);
    void registrateObject(const QStringList dPath, FileType fileType, const IdObjectPtrV list);
    void addAccount(IdAccount_bit id);
    void setLastChangedTime(quint32 account, IdObject_bit id, QDateTime dTime);
    void addLocalShared(IdGlobal_bit sender, IdGlobal_bit whatsAddId);

    std::vector <AccountStorage> m_storageList;
};

typedef AFlib::Storage    AFStorage;
typedef AFlib::StoragePtr AFStoragePtr;

#endif
