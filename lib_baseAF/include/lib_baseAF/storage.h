#ifndef LIB_BASEAF_STORAGE
#define LIB_BASEAF_STORAGE

#include "dir.h"
#include <optional>

namespace AFlib {
class Storage;
typedef QSharedPointer <Storage> StoragePtr;
}

class AFlib::Storage : public QObject
{
public:
    Storage(DirPtr dir, QObject* parent = nullptr);

    static bool writeData(QDir dir, QString name, QString sufix, QByteArray data, bool crypt = false,
                          QString user = QString(), QString sub1 = QString(), QString sub2 = QString());
    bool writeData(QString name, QByteArray data, bool user = true, bool crypt = false);
    bool writeData(QString name, QByteArray data, QString sufix, bool user = true, bool crypt = false);
    bool writeData(QString plugin, QString name, QByteArray data, bool user = true, bool crypt = false);
    bool writeData(QString plugin, QString name, QByteArray data, QString sufix, bool user = true, bool crypt = false);

    static std::optional <QByteArray> readData(QString filePath);
    std::optional <QByteArray> readData(QDir dir, QString name, QString sufix = QString());
    std::optional <QByteArray> readData(QString plugin, QDir dir, QString name, QString sufix = QString());

    // TODO add move settings function from one folder to next folder using function from AFlib::Dir

private:
    DirPtr m_dir;
    QString m_subFolder;
    QString m_subSubFolder;
    QString m_sufix;
};

#endif
