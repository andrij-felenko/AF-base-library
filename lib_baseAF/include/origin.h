#ifndef MUNNIN_ORIGIN_H
#define MUNNIN_ORIGIN_H

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>

#include "dir.h"
#include "setting.h"
#include "user.h"

namespace AFlib {
    class Origin;
    typedef QSharedPointer <Origin> OriginPtr;
}

class AFlib::Origin final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(DirPtr     dir     READ dir     CONSTANT)
    Q_PROPERTY(SettingPtr setting READ setting CONSTANT)
    Q_PROPERTY(UserPtr    user    READ user    CONSTANT)
public:
    explicit Origin(QObject *parent = nullptr);

    virtual DirPtr     dir()     const final;
    virtual SettingPtr setting() const final;
    virtual UserPtr    user()    const final;

signals:
    void   loadLibrary(QString /* sender */);
    void unloadLibrary(QString /* sender */);

public slots:
    //

private:
    DirPtr m_dir;
    SettingPtr m_setting;
    UserPtr m_user;
};

#endif // MUNNIN_ORIGIN_H
