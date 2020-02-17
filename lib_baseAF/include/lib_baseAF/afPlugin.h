#ifndef LIB_BASEAF_PLUGIN
#define LIB_BASEAF_PLUGIN

#include <memory>
#include <QtCore/QUrl>
#include <QtCore/QPluginLoader>
#include <QtCore/QSharedPointer>
#include <QtCore/QJsonObject>

#include "afDir.h"
#include "afVersion.h"
#include "afPluginMetadata.h"

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

    virtual void stop(); // stop work all, need to set default all settings and data inside
    virtual void initialize();
    virtual void unloadPlugin();
    virtual QPluginLoader* loader() const final;

    virtual bool isStatic() const final;
    virtual bool isGui() const;
    virtual void readyToStart();

signals:
    // two signal must be calling in Plugin::start function
    void loadProgress(double /* from 0 to 1 */);
    void loadComplete();
    void initComplete();

protected:
    bool m_initComplete = false;
    bool m_readyToStart = false;
    QPluginLoader* m_loader;

    virtual bool start();
};

#define AFlibPlugin_iid "org.andrij_felenko.base_plugin"
Q_DECLARE_INTERFACE(AFlib::Plugin, AFlibPlugin_iid)

#endif // LIB_BASEAF_PLUGIN
