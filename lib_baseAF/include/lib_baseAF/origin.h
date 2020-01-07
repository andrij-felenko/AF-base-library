#ifndef LIB_BASEAF_ORIGIN_H
#define LIB_BASEAF_ORIGIN_H

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>

#include "dir.h"
#include "setting.h"
#include "storage.h"
//#include "account.h"

namespace AFlib {
    class Origin;
    typedef QSharedPointer <Origin> OriginPtr;
}

class AFlib::Origin final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(DirPtr     dir     READ dir     CONSTANT)
    Q_PROPERTY(SettingPtr setting READ setting CONSTANT)
//    Q_PROPERTY(AccountPtr    user    READ user    CONSTANT)
public:
    explicit Origin(QObject *parent = nullptr);

    virtual DirPtr     dir()     const final;
    virtual SettingPtr setting() const final;
    virtual StoragePtr storage() const final;
//    virtual AccountPtr    user()    const final;

signals:
    void   loadLibrary(QString /* sender */);
    void unloadLibrary(QString /* sender */);

public slots:
    //

private:
    DirPtr m_dir;
    SettingPtr m_setting;
    StoragePtr m_storage;
//    AccountPtr m_user;

    friend class userHandler;
};

#endif // LIB_BASEAF_ORIGIN_H
