#ifndef LIB_BASEAF_PLUGIN_METADATA_H
#define LIB_BASEAF_PLUGIN_METADATA_H

#include <QtCore/QObject>
#include <QtCore/QJsonObject>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QtCore/QUrl>

#include "version.h"

namespace AFlib {
class PluginMetadata : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl    icon READ icon CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)

public:
    explicit PluginMetadata(QObject* parent = nullptr);
    PluginMetadata(const QJsonObject& obj, QObject* parent = nullptr);
    PluginMetadata(const PluginMetadata& c_data);

    virtual QString name()           const final { return m_name; }
    virtual Version version()        const final { return m_version; }
    virtual QVariantHash libraries() const final { return m_libraries; }
    virtual QString license()        const final { return m_license; }
    virtual QUrl icon()              const final { return m_icon; }
    virtual bool isEmpty();
    void setJson(const QJsonObject& obj);

    static QSharedPointer<PluginMetadata> parseMetadata(const QJsonObject& obj);

private:
    QString m_name;
    Version m_version;
    QVariantHash m_libraries;
    QString m_license;
    QUrl m_icon;
    bool m_isEmpty;

    void setName(QString name);
    void setVersion(Version version);
    void setLibraries(QVariantHash libraries);
    void setIcon(QUrl url);
    void setIcon(QString url);
    void setLicense(QString license);
};
}

#endif // LIB_BASEAF_PLUGIN_METADATA_H
