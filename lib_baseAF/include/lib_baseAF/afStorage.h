#ifndef LIB_BASEAF_STORAGE
#define LIB_BASEAF_STORAGE

#include <QtCore/QDateTime>
#include <optional>
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
public:
    Storage(QObject* parent = qApp);
    static QSharedPointer <Storage> init();

    void loadAllData();
    void setPath(QDir dir);

    bool writeData(quint32 obj_id, QByteArray data);
    bool writeData(QString f_path, QByteArray data);
    bool writeData(QStringList listOfId, QString object, QByteArray data);
    bool writeData(QList <quint32> list, quint32 obj_id, QByteArray data);

    bool writeData(quint32 obj_id, IdOperate& data);
    bool writeData(QString f_path, IdOperate& data);
    bool writeData(QStringList listOfId, QString object, IdOperate& data);
    bool writeData(QList <quint32> list, quint32 obj_id, IdOperate& data);

    bool writeData(quint32 obj_id, IdOperatePtr data);
    bool writeData(QString f_path, IdOperatePtr data);
    bool writeData(QStringList listOfId, QString object, IdOperatePtr data);
    bool writeData(QList <quint32> list, quint32 obj_id, IdOperatePtr data);

    bool writeData(quint32 obj_id, IdOperatePtrList data);
    bool writeData(QString f_path, IdOperatePtrList data);
    bool writeData(QStringList listOfId, QString object, IdOperatePtrList data);
    bool writeData(QList <quint32> list, quint32 obj_id, IdOperatePtrList data);

    // FIXME maybe add short and not short version?
    QByteArray readData(quint32 obj_id) const;
    QByteArray readData(QString f_path) const;
    QByteArray readData(QStringList listOfId, QString object) const;
    QByteArray readData(QList <quint32> list, quint32 obj_id) const;

private:
    QDir getDir(QStringList listOfId = {}) const;
    QDir getDir(QList <quint32> list) const;
    void loadFromDirectory(QDir& dir);

    QString getFile(const QDir &dir, QString name) const;
    QString getFile(const QDir &dir, quint32   id) const;

    struct SingleStorage {
        QDateTime lastChange;
        quint16 uniqueId;
        quint8 typeOfSubject;
    };

    struct PluginStorage {
        quint8 pluginId;

        QList <SingleStorage> idList;
        void addSingle(QDateTime time, quint16 id, quint8 type);
    };

    struct AccountStorage {
        id::Account_bit accountBit;

        QList <PluginStorage> pluginList;
        void addPlugin(quint8 id);
    };

    void addAccount(IdAccount_bit id);
    void setLastChangedTime(quint32 account, quint8 plugin, quint8 typeOfSubject, quint16 uniqueId, QDateTime dTime);

    QDir m_storageDir;
    QList <AccountStorage> m_storageList;
};

typedef AFlib::Storage   AFStorage;
typedef AFlib::StoragePtr AFStoragePtr;

#endif
