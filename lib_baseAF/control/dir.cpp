#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>

#include "dir.h"

using namespace AFlib;
#define TEST_RELEASE false

Dir::Dir(QObject *parent) : QObject(parent)
{
    m_app = QDir::current();

    QDir cacheDir;
    QDir configDir;
    QDir localDir;
    cacheDir = configDir = localDir
#if TEST_RELEASE || defined(_RELEASE_)
            = QDir::home();
#else
            = m_app;
#endif

#if defined (Q_OS_WIN)
    cdDirectory(cacheDir, "AppData");
    cdDirectory(cacheDir, "Local");
#elif defined (Q_OS_MAC)
    cdDirectory(cacheDir, "Applications");
#elif defined (Q_OS_LINUX)
#if TEST_RELEASE
    cdDirectory(cacheDir, ".cache");
    cdDirectory(configDir, ".config");
    cdDirectory(localDir, ".local");
#else
    cdDirectory(cacheDir, "cache");
    cdDirectory(configDir, "config");
    cdDirectory(localDir, "local");
#endif
#endif

#if TEST_RELEASE || defined(_RELEASE_)
    QString appName(qApp->applicationName());
    QString organizationName(qApp->organizationName());

#if defined (_DEBUG_)
    appName.append("_d");
#elif defined (_RELEASE_)
#else
    qWarning() << "Error when try create constructor in AFlib::Dir";
    return;
#endif

#if defined (Q_OS_WIN) || defined (Q_OS_MAC)
    cdDirectory(cacheDir, organizationName);
    cdDirectory(cacheDir, appName);
    configDir = localDir = cacheDir;
#elif defined (Q_OS_LINUX)
    cdDirectory(cacheDir, organizationName);
    cdDirectory(cacheDir, appName);
    cdDirectory(configDir, organizationName);
    cdDirectory(configDir, appName);
    cdDirectory(localDir, organizationName);
    cdDirectory(localDir, appName);
#endif
#endif

    // config
    m_config = configDir;

    // cache
    QDir tempCacheDir(cacheDir);
    cdDirectory(tempCacheDir, "cookies");
    m_cookies = tempCacheDir;

    tempCacheDir = cacheDir;
    cdDirectory(tempCacheDir, "database");
    m_database = tempCacheDir;

    // local
    tempCacheDir = localDir;
    cdDirectory(tempCacheDir, "libraries");
    m_libraries = tempCacheDir;

    tempCacheDir = localDir;
    cdDirectory(tempCacheDir, "plugins");
    m_plugins = tempCacheDir;

    tempCacheDir = localDir;
    cdDirectory(tempCacheDir, "translates");
    m_translates = tempCacheDir;

    tempCacheDir = localDir;
    cdDirectory(tempCacheDir, "users");
    m_users = tempCacheDir;

    qInfo() << "Directory settings for application";
    qInfo() << "app" << m_app.path();
    qInfo() << "config" << m_config.path();
    qInfo() << "cookies" << m_cookies.path();
    qInfo() << "database" << m_database.path();
    qInfo() << "libraries" << m_libraries.path();
    qInfo() << "plugins" << m_plugins.path();
    qInfo() << "translates" << m_translates.path();
    qInfo() << "users" << m_users.path();
}

void Dir::switchToUser(QString name)
{
    QDir tempCacheDir(m_users);
    if (cdDirectory(tempCacheDir, name)){
        setUserDir(tempCacheDir);

        cdDirectory(tempCacheDir, "config");
        setUserConfig(tempCacheDir);

        tempCacheDir = m_userDir;
        cdDirectory(tempCacheDir, "data");
        setUserData(tempCacheDir);
    }
    else
        qWarning() << QString("User %1 directory not created").arg(name);
}

void Dir::setUserConfig(QDir userConfig)
{
    if (m_userConfig == userConfig)
        return;

    m_userConfig = userConfig;
    emit userConfigChanged(m_userConfig);
}

void Dir::setUserData(QDir userData)
{
    if (m_userData == userData)
        return;

    m_userData = userData;
    emit userDataChanged(m_userData);
}

void Dir::setUserDir(QDir userDir)
{
    if (m_userDir == userDir)
        return;

    m_userDir = userDir;
    emit userDirChanged(m_userDir);
}

bool Dir::cdDirectory(QDir &dir, QString name)
{
    if (! dir.cd(name)){
        dir.mkdir(name);
        if (! dir.cd(name)){
            qInfo() << QString("Cannot create Dir %1 in the %2").arg(dir.absolutePath(), name);
            return false;
        }
    }
    return true;
}

void Dir::cpDirectory(QString from, QString to)
{
    QDir dir(from);
    if (! dir.exists()){
        qDebug() << "directory " << from << " not found, end copy directory";
        return;
    }

    for (auto d : dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString dst_path = to + QDir::separator() + d;
        dir.mkpath(dst_path);
        cpDirectory(from+ QDir::separator() + d, dst_path);
    }

    for (auto f : dir.entryList(QDir::Files)) {
        QFile::copy(from + QDir::separator() + f, to + QDir::separator() + f);
    }
}
