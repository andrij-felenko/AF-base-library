#ifndef AFLIB_PLUGIN_APPLICATION
#define AFLIB_PLUGIN_APPLICATION

#include <memory>
#include <QtCore/QUrl>
#include <QtCore/QPluginLoader>
#include <QtCore/QSharedPointer>
#include <QtCore/QJsonObject>

#include "dir.h"
#include "origin.h"
#include "version.h"
#include "pluginMetadata.h"

namespace AFlib { class Plugin; }
class AFlib::Plugin : public PluginMetadata
{
    Q_OBJECT
    Q_PROPERTY(QString name    READ name    CONSTANT)
    Q_PROPERTY(Version version READ version CONSTANT)
public:
    explicit Plugin(QObject* parent = nullptr);
    Plugin(const QJsonObject& obj, QObject* parent = nullptr);
    Plugin(const PluginMetadata& c_data);
    virtual ~Plugin() = default;

    virtual void stop();
    virtual void initialize();
    virtual void unloadPlugin();
    virtual QPluginLoader* loader() const final;

    virtual bool isStatic() const final;
    virtual bool isGui() const;
    virtual bool start();
    virtual void setOrigin(const QSharedPointer <AFlib::Origin> origin) final;

signals:
    // two signal must be calling in Plugin::start function
    void loadProgress(double /* from 0 to 1 */);
    void loadComplete();

protected:
    QPluginLoader* m_loader;
    QSharedPointer <const AFlib::Origin> m_origin;
};

#define AFlibPlugin_iid "org.andrij_felenko.base_plugin"
Q_DECLARE_INTERFACE(AFlib::Plugin, AFlibPlugin_iid)

#endif //AFLIB_PLUGIN_APPLICATION
