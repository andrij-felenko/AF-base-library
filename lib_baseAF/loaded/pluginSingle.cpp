#include "pluginSingle.h"
#include <QtCore/QDebug>

using namespace AFlib;

PluginSingle::PluginSingle(QString path, QObject *parent)
    : QObject(parent), m_type(Type::Dynamic)
{
    m_loader.create(this);
    m_loader->setFileName(path);
}

PluginSingle::PluginSingle(QDir dir, QString name, QObject *parent)
    : PluginSingle(dir.absoluteFilePath(name), parent)
{
    //
}

PluginSingle::PluginSingle(Plugin* object, QObject *parent)
    : QObject(parent), m_type(Type::Static), m_plugin(object)
{
    //
}

PluginSingle::Type PluginSingle::type() const
{
    return m_type;
}

bool PluginSingle::exists() const
{
    switch (m_type){
    case Type::Static: return true;
    case Type::Dynamic: return QFile::exists(m_loader->fileName());
    default:;
    }
    return false;
}

bool PluginSingle::init()
{
    if (m_type == Type::Static){
        qDebug() << "Type static can`t init;";
        return false;
    }

    if (m_loader.isNull())
        m_loader.create(this);
    bool isLoad = m_loader->load();
    qDebug() << "Try load plugin: " << isLoad;
    return isLoad;
}

std::optional <const Plugin *> PluginSingle::instance()
{
    if (m_type == Type::Static && m_plugin)
        return m_plugin;

    if (m_type == Type::Dynamic)
        if (init())
            if (m_loader->isLoaded())
                return qobject_cast <const Plugin*> (m_loader->instance());

    return std::nullopt;
}
