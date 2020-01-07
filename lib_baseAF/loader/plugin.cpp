#include "plugin.h"
#include <QtCore/QDebug>

using namespace AFlib;

Plugin::Plugin(QObject* parent) : PluginMetadata(parent)
{
    m_loader = new QPluginLoader(this);
//    qDebug() << "METADATA:" << this->metaData();
//    m_pluginInfo = QSharedPointer <AFlib::PluginMetadata>::create(this->metaData().value("MetaData").toObject());
    //
}

Plugin::Plugin(const QJsonObject& obj, QObject* parent) : PluginMetadata(obj, parent)
{
    //
}

Plugin::Plugin(const PluginMetadata& c_data) : PluginMetadata(c_data)
{
    //
}

void Plugin::stop()
{
    m_initComplete = false;
}

void Plugin::initialize()
{
    emit initComplete();
    m_initComplete = true;
}

void Plugin::unloadPlugin()
{
    //
}

QPluginLoader *Plugin::loader() const
{
    return m_loader;
}

bool Plugin::isStatic() const
{
#ifdef _LIB_STATIC_
    return true;
#else
    return false;
#endif
}

bool Plugin::isGui() const
{
    return false;
}

bool Plugin::start()
{
    // TODO call warning must be reimplement
    m_readyToStart = false;
    return false;
}

void Plugin::readyToStart()
{
    if (m_initComplete)
        qInfo() << QString("Plugin %1 start: ").arg(name()) << start();
    m_readyToStart = true;
}

void Plugin::setOrigin(const QSharedPointer<Origin> origin)
{
    m_origin = origin;
}
