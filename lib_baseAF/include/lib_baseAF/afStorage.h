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

class AFlib::Storage : public QObject
{
    typedef IdObjectPtr     IdObj;
    typedef AFIdObject_bit  IdObj_b;
    typedef IdObjectPtrList IdObjList;

public:
    explicit Storage(QObject* parent = qApp);
    static QSharedPointer <Storage> init();

    // TODO add refresh object function, send object and read all new to it file
    void loadFromDirectory(const QStringList& dPath, Compress compress = Compress::AllActive);
    bool updateFile(const QStringList dPath, const QByteArray &data, AFlib::FileType type = AFlib::FileType::Data);
    bool contains(const IdObject& ptr) const;
    bool contains(const IdAccount_bit& account, const IdObjectU_bit& object) const;

    bool addOperate(const IdObject& object, const IdOperate& operate);
    bool addOperateList(const transfer::List &list);

    transfer::List getOperatesAfter(const QDateTime& dateTime);

    bool addObject(const QStringList dPath, const IdObject& object, AFlib::FileType type = AFlib::FileType::Data);

    bool addOperateList(transfer::List &operateList);

    IdObj     getObject    (const QStringList dPath, const IdObject_bit &object, Compress compress  = Compress::AllActive, AFlib::FileType type = AFlib::FileType::Data);
    IdObj     getObject    (const QStringList dPath, const IdObject_bit &object, AFlib::FileType type, Compress compress = Compress::AllActive);

    IdObjList getObjectList(const QStringList dPath,                             Compress compress  =  Compress::AllActive, AFlib::FileType type = AFlib::FileType::Data);
    IdObjList getObjectList(const QStringList dPath,                             AFlib::FileType type, Compress compress = Compress::AllActive);
    IdObjList getObjectList(QList <id::Account_bit> accList, quint8 plugin, Compress compress  =  Compress::AllActive);

    IdObjList getObjectList(const QStringList dPath, const IdObjectPtrList list, Compress compress  =  Compress::AllActive, AFlib::FileType type = AFlib::FileType::Data);
    IdObjList getObjectList(const QStringList dPath, const IdObjectPtrList list, AFlib::FileType type, Compress compress = Compress::AllActive);

    bool removeObject(const IdObject& object);

private:
    struct SingleStorage {
        QDateTime lastChange;
        IdObjectU_bit  id;
        QStringList dPath;
        FileType fileType;
    };

    struct PluginStorage {
        quint8 pluginId;

        std::vector <SingleStorage> idList;
        void addSingle(QStringList dPath, FileType fileType, QDateTime time, IdObjectU_bit id);
    };

    struct AccountStorage {
        id::Account_bit accountBit;

        std::vector <PluginStorage> pluginList;
        void addPlugin(quint8 id);
    };

    std::optional <SingleStorage> findSingle(const IdObject &object);
    std::optional <SingleStorage> findSingle(const IdAccount_bit& account, const IdObjectU_bit& object);
    void removeListByFile(const QStringList dPath, FileType fileType);
    void registrateObject(const QStringList dPath, FileType fileType, const IdObject&     object);
    void registrateObject(const QStringList dPath, FileType fileType, const IdObjectPtr   object);
    void registrateObject(const QStringList dPath, FileType fileType, const IdObjectPtrList list);
    void addAccount(IdAccount_bit id);
    void setLastChangedTime(quint32 account, IdObjectU_bit id, QDateTime dTime);

    std::vector <AccountStorage> m_storageList;
};

typedef AFlib::Storage    AFStorage;
typedef AFlib::StoragePtr AFStoragePtr;

#endif
