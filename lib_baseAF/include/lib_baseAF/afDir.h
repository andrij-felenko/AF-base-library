#ifndef LIB_BASEAF_DIR_H
#define LIB_BASEAF_DIR_H

#include <QtCore/QDir>
#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtCore/QSharedPointer>

namespace AFlib {
    class Dir;
    typedef QSharedPointer <Dir const> DirPtr;
    DirPtr afDir();
}

// TODO rewrite
class AFlib::Dir : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDir app        READ app        CONSTANT)
    Q_PROPERTY(QDir config     READ config     CONSTANT)
    Q_PROPERTY(QDir cookies    READ cookies    CONSTANT)
    Q_PROPERTY(QDir libraries  READ libraries  CONSTANT)
    Q_PROPERTY(QDir plugins    READ plugins    CONSTANT)
    Q_PROPERTY(QDir storage    READ storage    CONSTANT)
    Q_PROPERTY(QDir translates READ translates CONSTANT)
    Q_PROPERTY(QDir users      READ users      CONSTANT)
    // current user
    Q_PROPERTY(QDir userConfig READ userConfig NOTIFY userConfigChanged)
    Q_PROPERTY(QDir userData   READ userData   NOTIFY userDataChanged)
    Q_PROPERTY(QDir userDir    READ userDir    NOTIFY userDirChanged)

public:
    explicit Dir(QObject *parent = nullptr);
    void init(bool useCurrentFolder = false);

    static bool cdDirectory(QDir &dir, QString name);
    static bool cdDirectory(QDir &dir, quint32 name, uint size);
    static void cpDirectory(QString from, QString to);

    virtual QDir app()        const final { return m_app; }
    virtual QDir config()     const final { return m_config; }
    virtual QDir cookies()    const final { return m_cookies; }
    virtual QDir storage()    const final { return m_storage; }
    virtual QDir libraries()  const final { return m_libraries; }
    virtual QDir plugins()    const final { return m_plugins; }
    virtual QDir translates() const final { return m_translates; }
    virtual QDir users()      const final { return m_users; }
    virtual QDir userConfig() const final { return m_userConfig; }
    virtual QDir userData()   const final { return m_userData; }
    virtual QDir userDir()    const final { return m_userDir; }
    virtual QDir pluginData(QString plugin) const;

signals:
    void userConfigChanged(QDir userConfig);
    void userDataChanged(QDir userData);
    void userDirChanged(QDir userDir);

private slots:
    void switchToUser(QString name);

private:
    QDir m_app;
    QDir m_config;
    QDir m_cookies;
    QDir m_libraries;
    QDir m_plugins;
    QDir m_storage;
    QDir m_translates;
    QDir m_users;
    QDir m_userConfig;
    QDir m_userData;
    QDir m_userDir;

    void setUserConfig(QDir userConfig);
    void setUserData(QDir userData);
    void setUserDir(QDir userDir);

    friend class Origin;
};

typedef AFlib::Dir AFDir;

#endif // LIB_BASEAF_DIR_H
