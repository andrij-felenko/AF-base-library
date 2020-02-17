#ifndef LIB_BASEAF_PLUGIN_SINGLE_H
#define LIB_BASEAF_PLUGIN_SINGLE_H

#include <QtCore/QObject>
#include "afPlugin.h"

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
    bool isLoad() const;
    std::optional<Plugin *> instance();

signals:

public slots:

private:
    typedef QSharedPointer <QPluginLoader> PluginPointer;
    PluginPointer m_loader;
    Type m_type;
    Plugin* m_plugin;
};

#endif // LIB_BASEAF_PLUGIN_SINGLE_H
