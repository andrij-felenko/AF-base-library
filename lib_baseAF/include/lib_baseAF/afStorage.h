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
public:
    explicit Storage(QObject* parent = qApp);
    static QSharedPointer <Storage> init();

    void loadAllData();
    void setPath(QDir dir);

    bool addObjectId(const QString& key, const QByteArray& data);
    bool addObjectId(const QString& key, const IdObject& object);
    bool addObjectId(const QDir& dir, const QString& key, const QByteArray& data);
    bool addObjectId(const QDir& dir, const QString& key, const IdObject& object);

    bool removeObjectId(                 const QString& key, const AFIdObject_bit& object);
    bool removeObjectId(const QDir& dir, const QString& key, const AFIdObject_bit& object);

    bool updateFile(                 const QString& key, const QByteArray& data);
    bool updateFile(const QDir& dir, const QString& key, const QByteArray& data);

    bool addOperate(                 const QString& key, const AFIdObject_bit& object, const QByteArray& data);
    bool addOperate(const QDir& dir, const QString& key, const AFIdObject_bit& object, const QByteArray& data);
    bool addOperate(                 const QString& key, const AFIdObject_bit& object, const IdOperate& operate);
    bool addOperate(const QDir& dir, const QString& key, const AFIdObject_bit& object, const IdOperate& operate);
    bool addOperate(                 const QString& key, const AFIdObject_bit& object, const IdOperatePtr& operate);
    bool addOperate(const QDir& dir, const QString& key, const AFIdObject_bit& object, const IdOperatePtr& operate);

    bool addOperateList(                 const QString& key, const AFIdObject_bit& object, const QByteArray& data);
    bool addOperateList(const QDir& dir, const QString& key, const AFIdObject_bit& object, const QByteArray& data);
    bool addOperateList(                 const QString& key, const AFIdObject_bit& object, const IdOperatePtrList& list);
    bool addOperateList(const QDir& dir, const QString& key, const AFIdObject_bit& object, const IdOperatePtrList& list);

    IdObjectPtrList readObjectList(const QDir& dir, bool recurcion = true, CompressValue compress = CompressValue::AllActive);
    /*! Read object in directory without recurcive */
    IdObjectPtrList readObjectList(const QDir& dir, CompressValue compress = CompressValue::AllActive);
    IdObjectPtrList readObjectList(const QDir& dir, const QString& key, const QList <AFIdObject_bit> list, CompressValue value = CompressValue::Shortest);
    IdObjectPtrList readObjectList(                 const QString& key, const QList <AFIdObject_bit> list, CompressValue value = CompressValue::Shortest);
    IdObjectPtr     readObject(const QDir& dir, const QString& key, const AFIdObject_bit& object, CompressValue compress = CompressValue::Shortest);
    IdObjectPtr     readObject(const QString& key, const AFIdObject_bit& object, CompressValue compress = CompressValue::Shortest);

private:
    QDir getDir(const QString& name) const;
    QDir getDir(QStringList listOfId = {}) const;
    QDir getDir(QList <quint32> list) const;
    void loadFromDirectory(QDir& dir);

    // private storage functions
    bool p_addObjectId(QString fileName,      const QByteArray& data);
    bool p_addObjectId(QString fileName,      const IdObject& object);

    bool p_removeObjectId(QString fileName,      const AFIdObject_bit& object);

    bool p_updateFile(QString fileName,      const QByteArray& data);

    bool p_addOperate(QString fileName,      const AFIdObject_bit& object, const QByteArray& data);
    bool p_addOperate(QString fileName,      const AFIdObject_bit& object, const IdOperate& operate);
    bool p_addOperate(QString fileName,      const AFIdObject_bit& object, const IdOperatePtr& operate);

    bool p_addOperateList(QString fileName,      const AFIdObject_bit& object, const QByteArray& data);
    bool p_addOperateList(QString fileName,      const AFIdObject_bit& object, const IdOperatePtrList& list);
    IdObjectPtrList p_readObjectList(QString fileName, CompressValue compress = CompressValue::AllActive) const;
    IdObjectPtrList p_readObjectList(QString fileName, QList <AFIdObject_bit> list, CompressValue compress = CompressValue::Shortest) const;
    IdObjectPtr     p_readObject(QString fileName,      const AFIdObject_bit& object, CompressValue compress = CompressValue::Shortest) const;

    IdOperatePtrList p_getOperateListAfter(const QString& fileName, const AFIdObject_bit& object, const QDateTime& afterDate) const;

    // get file
    QString getFile(const QDir &dir, QString name) const;
    QString getFile(const QDir &dir, quint32   id) const;

    // all this struct we just need to update, it have all possible changes for every operate and account
    struct SingleStorage {
        QDateTime lastChange;
        quint16 uniqueId;
        quint8 typeOfSubject;
        QString filePath;
    };

    struct PluginStorage {
        quint8 pluginId;

        QList <SingleStorage> idList;
        void addSingle(QString filePath, QDateTime time, quint16 id, quint8 type);
    };

    struct AccountStorage {
        id::Account_bit accountBit;

        QList <PluginStorage> pluginList;
        void addPlugin(quint8 id);
    };

    void addOperateToStorageList(const QString &filePath, const IdObjectPtr object);
    void addAccount(IdAccount_bit id);
    void setLastChangedTime(quint32 account, quint8 plugin, quint8 typeOfSubject, quint16 uniqueId, QDateTime dTime);

    // TODO we need to have some list of storage folders, for wxample for account and for data
    static QDir m_storageDir;
    QList <AccountStorage> m_storageList;
};

typedef AFlib::Storage   AFStorage;
typedef AFlib::StoragePtr AFStoragePtr;

#endif
