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
    Storage(QObject* parent = qApp);
    static QSharedPointer <Storage> init();

    void loadAllData();
    void setPath(QDir dir);

    bool addObjectId(QDir dir, QString key, const QByteArray& data);
    bool addObjectId(QString fileName,      const QByteArray& data);
    bool addObjectId(QDir dir, QString key, const IdObject& object);
    bool addObjectId(QString fileName,      const IdObject& object);

    bool removeObjectId(QDir dir, QString key, const AFIdObject_bit& object);
    bool removeObjectId(QString fileName,      const AFIdObject_bit& object);

    bool updateFile(QDir dir, QString key, const QByteArray& data);
    bool updateFile(QString fileName,      const QByteArray& data);

    bool addOperate(QDir dir, QString key, const AFIdObject_bit& object, const QByteArray& data);
    bool addOperate(QString fileName,      const AFIdObject_bit& object, const QByteArray& data);
    bool addOperate(QDir dir, QString key, const AFIdObject_bit& object, const IdOperate& operate);
    bool addOperate(QString fileName,      const AFIdObject_bit& object, const IdOperate& operate);
    bool addOperate(QDir dir, QString key, const AFIdObject_bit& object, const IdOperatePtr& operate);
    bool addOperate(QString fileName,      const AFIdObject_bit& object, const IdOperatePtr& operate);

    bool addOperateList(QDir dir, QString key, const AFIdObject_bit& object, const QByteArray& data);
    bool addOperateList(QString fileName,      const AFIdObject_bit& object, const QByteArray& data);
    bool addOperateList(QDir dir, QString key, const AFIdObject_bit& object, const IdOperatePtrList& list);
    bool addOperateList(QString fileName,      const AFIdObject_bit& object, const IdOperatePtrList& list);

    IdObjectPtrList readObjectList(QString fileName, CompressValue compress = CompressValue::AllActive);
    IdObjectPtr     readObject(QDir dir, QString key, const AFIdObject_bit& object, CompressValue compress = CompressValue::Shortest);
    IdObjectPtr     readObject(QString fileName,      const AFIdObject_bit& object, CompressValue compress = CompressValue::Shortest);

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
