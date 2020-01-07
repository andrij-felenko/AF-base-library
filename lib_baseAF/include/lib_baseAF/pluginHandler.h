#ifndef LIB_BASEAF_PLUGIN_HANDLER_H
#define LIB_BASEAF_PLUGIN_HANDLER_H

#include <QtCore/QObject>
#include "plugin.h"
#include "libHandler.h"
#include "origin.h"
#include "pluginSingle.h"

namespace AFlib { class PluginHandler; }
class AFlib::PluginHandler : public QObject, protected QList <PluginSinglePtr>
{
    Q_OBJECT
    Q_PROPERTY(QList <const Plugin*> pluginList READ pluginList NOTIFY pluginListChanged)
public:
    explicit PluginHandler(OriginPtr origin, QObject *parent = nullptr);

    // for dynamic
    void parsePluginDirectory();
    std::optional <Plugin*> load(QString pluginName, bool isCritical = false);

    // for static
    void initStaticPlugin();
    void startStaticPlugin();
    bool registratePlugin(Plugin* object);

    QList <const Plugin*> pluginList() const;

signals:
    void pluginListChanged(QList <const Plugin*> pluginList);

private:
    OriginPtr m_origin;
    // TODO fix it
    LibHandler *m_libraries;
};

#endif // LIB_BASEAF_PLUGIN_HANDLER_H
