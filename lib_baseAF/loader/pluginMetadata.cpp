#include <QtCore/QJsonArray>
#include <QSharedPointer>

#include "pluginMetadata.h"

AFlib::PluginMetadata::PluginMetadata(QObject *parent) : QObject(parent), m_isEmpty(true)
{
    //
}

AFlib::PluginMetadata::PluginMetadata(const QJsonObject &obj, QObject *parent)
    : AFlib::PluginMetadata(parent)
{
    setJson(obj);
}

AFlib::PluginMetadata::PluginMetadata(const AFlib::PluginMetadata &c_data)
    : QObject(c_data.parent()), m_name(c_data.m_name), m_version(c_data.m_version),
      m_libraries(c_data.m_libraries), m_icon(c_data.m_icon), m_isEmpty(c_data.m_isEmpty)
{
    //
}

bool AFlib::PluginMetadata::isEmpty()
{
    return m_isEmpty;
}

void AFlib::PluginMetadata::setJson(const QJsonObject &obj)
{
    if (obj.keys().contains("name") && obj.keys().contains("libraries") && obj.keys().contains("version")){
        setName(obj.value("name").toString());
        QVariantHash hash;
        for (auto it : obj.value("libraries").toArray())
            hash.insert(it.toObject().value("name").toString(), it.toObject().value("version").toString());
        setLibraries(hash);
        setVersion(Version(obj.value("version").toString()));

        // optional
        if (obj.keys().contains("icon"))
            setIcon(obj.value("icon").toString());
        if (obj.keys().contains("license"))
            setLicense(obj.value("license").toString());
        m_isEmpty = false;
    }
    else {
        m_isEmpty = true;
    }
}

QSharedPointer<AFlib::PluginMetadata> AFlib::PluginMetadata::parseMetadata(const QJsonObject &obj)
{
    QSharedPointer<AFlib::PluginMetadata> metadata = QSharedPointer <AFlib::PluginMetadata>::create();
    metadata->setJson(obj);
    return metadata;
}

void AFlib::PluginMetadata::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
//    emit nameChanged(m_name);
    m_isEmpty = false;
}

void AFlib::PluginMetadata::setVersion(AFlib::Version version)
{
    if (m_version == version)
        return;

    m_version = version;
//    emit versionChanged(m_version);
    m_isEmpty = false;
}

void AFlib::PluginMetadata::setLibraries(QVariantHash libraries)
{
    if (m_libraries == libraries)
        return;

    m_libraries = libraries;
//    emit librariesChanged(m_libraries);
    m_isEmpty = false;
}

void AFlib::PluginMetadata::setIcon(QUrl url)
{
    if (url == m_icon)
        return;

    m_icon = url;
}

void AFlib::PluginMetadata::setIcon(QString url)
{
    setIcon(QUrl(url));
}

void AFlib::PluginMetadata::setLicense(QString license)
{
    if (m_license == license)
        return;

    m_license = license;
}
