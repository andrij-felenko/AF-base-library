#include "lib_baseAF/afServerSettings.h"

AFlib::ServerSettingPtr p_serverSettingsPtr;

AFlib::ServerSetting::ServerSetting(QObject *parent) : QObject(parent)
{
    // TODO read settings from ini file or another text file, or read settings from account info file
}

bool AFlib::ServerSetting::addUrl(QString key, QString url)
{
    for (auto it = m_linkMap.begin(); it != m_linkMap.end(); ++it)
        if (it.key() == key)
            return false;

    m_linkMap.insert(key, url);
    return true;
}

QString AFlib::ServerSetting::getUrl(QString key, QString defaultValue) const
{
    return m_linkMap.value(key, defaultValue);
}

QDateTime AFlib::ServerSetting::lastStorageUpdate() const
{
    return m_lastStorageUpdate;
}

AFlib::ServerSettingPtr AFlib::afServerSettings(QObject *parent)
{
    if (p_serverSettingsPtr.isNull())
        p_serverSettingsPtr = ServerSettingPtr::create(parent);

    return p_serverSettingsPtr;
}

AFlib::ServerSettingPtr afServerSettings(QObject *parent)
{
    return AFlib::afServerSettings(parent);
}
