#ifndef PLUGINHANDLER_H
#define PLUGINHANDLER_H

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

    void parsePluginDirectory();
    bool registratePlugin(Plugin* object);
    std::optional <Plugin*> load(QString pluginName, bool isCritical = false);

    QList <const Plugin*> pluginList() const;

signals:
    void pluginListChanged(QList <const Plugin*> pluginList);

private:
    OriginPtr m_origin;
    // TODO fix it
    LibHandler *m_libraries;
};

#endif // PLUGINHANDLER_H
