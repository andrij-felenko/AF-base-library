#include "pluginGui.h"

using namespace AFlib;

PluginGui::PluginGui(QObject* parent) : Plugin(parent)
{
    //
}

PluginGui::PluginGui(const QJsonObject& obj, QObject* parent) : Plugin(obj, parent)
{
    //
}

PluginGui::PluginGui(const PluginMetadata& c_data) : Plugin(c_data)
{
    //
}

bool PluginGui::isGui() const
{
    return true;
}
