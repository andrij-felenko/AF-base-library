#ifndef LIB_BASEAF_STORAGE
#define LIB_BASEAF_STORAGE

#include <QtCore/QDateTime>
#include <optional>
#include "afDir.h"
#include "afId.h"

namespace AFlib {
    class Storage;
    typedef QSharedPointer <Storage> StoragePtr;

    StoragePtr afStorage();
}

class AFlib::Storage : public QObject
{
public:
    static StoragePtr init();

    // work with accounts
    void loadAccount();
    void writeAccountsData(quint32 account, QByteArray data);
    void writeAccountOperate(quint32 account, quint8 plugin, id::Operate_bit data);
    void writeAccountOperateList(quint32 account, quint8 plugin, QList <id::Operate_bit> list);

    // work with objects
    void loadData(quint32 account = 0, quint8 plugin = 255);
    void writeData(quint32 account, quint8 plugin, quint32 subjId, QByteArray data);
    void writeOperate(quint32 account, quint8 plugin, id::Operate_bit data);
    void writeOperateList(quint32 account, quint8 plugin, QList <id::Operate_bit> list);

    QByteArray readData(quint32 account, quint8 plugin, quint32 objId, bool fullOperate = false) const;

private:
    Storage();
    QDir getDestinationDir(quint32 account, quint8 plugin) const;

    struct SingleStorage {
        QDateTime lastChange;
        quint16 uniqueId;
        quint8 typeOfSubject;
    };

    struct ModuleStorage {
        quint8 pluginId;

        QList <SingleStorage> idList;
    };

    struct AccountStorage {
        QString acccountName;
        id::Account_bit accountBit;

        QList <ModuleStorage> moduleList;
    };

    QList <AccountStorage> m_storageList;
};

typedef AFlib::Storage   AFStorage;
typedef AFlib::StoragePtr AFStoragePtr;

#endif
