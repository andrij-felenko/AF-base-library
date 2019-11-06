#ifndef PLUGINSINGLE_H
#define PLUGINSINGLE_H

#include <QtCore/QObject>
#include "plugin.h"

namespace AFlib {
    class PluginSingle;
    typedef QSharedPointer <PluginSingle> PluginSinglePtr;
}

class AFlib::PluginSingle : public QObject
{
    Q_OBJECT
public:
    PluginSingle(QString path, QObject *parent = nullptr);
    PluginSingle(QDir dir, QString name, QObject *parent = nullptr);
    PluginSingle(Plugin* object, QObject *parent = nullptr);

    enum Type {
        Static = 0,
        Dynamic,
        Undeclarate = 0xFF
    };

    Type type() const;
    bool exists() const; // only for dynamic
    bool init(); // load plugin
    std::optional <const Plugin*> instance();

signals:

public slots:

private:
    typedef QSharedPointer <QPluginLoader> PluginPointer;
    PluginPointer m_loader;
    Type m_type;
    Plugin* m_plugin;
};

#endif // PLUGINSINGLE_H
