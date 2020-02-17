#ifndef PLUGINQML_H
#define PLUGINQML_H

#include "lib_baseAF/afPlugin.h"

namespace AFlib {
class PluginGui : public Plugin
{
    Q_OBJECT
public:
    explicit PluginGui(QObject* parent = nullptr);
    PluginGui(const QJsonObject& obj, QObject* parent = nullptr);
    PluginGui(const PluginMetadata& c_data);

    virtual QUrl source() = 0;

signals:
    //

public slots:
    //

private:
    virtual bool isGui() const override;
};

typedef QSharedPointer <PluginGui> PluginQmlPointer;
}

#define QuickAFlibPlugin_iid "org.andrij_felenko.quick_plugin"
Q_DECLARE_INTERFACE(AFlib::PluginGui, QuickAFlibPlugin_iid)

#endif // PLUGINQML_H
