#include <QtCore/QDebug>
#include <QtQml/QQmlContext>
#include <QtQml/QQmlEngine>
#include <QtQuick/QQuickItem>

#include "pluginHandler.h"
#include "function.h"
#include "pluginMetadata.h"

using namespace AFlib;

PluginHandler::PluginHandler(OriginPtr origin, QObject *parent)
    : QObject(parent), m_origin(origin)
{
    // FIXME it
    m_libraries = new LibHandler(this);
//    m_libraries->load(m_origin->dir()->libraries(), Function::libraryName("munnin_base_1.0.0"), "main");
//    m_libraries->load(m_origin->dir()->libraries(), Function::libraryName("munnin_icons_1.0.0"), "main");
//    m_libraries->load(m_origin->dir()->libraries(), Function::libraryName("munnin_quick_types_1.0.0"), "main");
}

void PluginHandler::parsePluginDirectory()
{
    // here we parse directory, when user changes, its need to make actual list of plugins, unused plugin destroyed when logout

    for (auto it = begin(); it != end(); ++it)
        if (it->data()->isLoad()){
            auto plugin = it->get()->instance();
            if (plugin.has_value())
                plugin.value()->stop();
        }

    // parse folder again (use default values after logout
    // TODO [Settings] for load dynamic plugins



    // TODO rewrite it all
//    QStringList pluginList;
//    for (auto it : m_origin->dir()->plugins().entryList(QDir::Files))
//        if (QLibrary::isLibrary(it))
//            pluginList.push_back(it);

//    // remove file from this.list if it remove from directory
//    auto removeFiles = std::remove_if(begin(), end(), [this, pluginList](const QSharedPointer<Plugin> &it) {
//        if (!pluginList.contains(it->loader()->fileName())){
//            m_libraries->unload(it->loader()->fileName());
//            // FIXME call unload depends library
//            it->loader()->unload();
//            return true;
//        }
//        return false;
//    });
//    erase(removeFiles, end());

//    for (auto pluginName : pluginList){
//        bool found = false;
//        for (auto it = begin(); it != end(); ++it)
//            if (it->get()->loader()->fileName() == pluginName){
//                found = true;
//                break;
//            }
//        if (not found){
//            // TODO add new plugin, need parse metadata and write it
//            auto single = QSharedPointer <Plugin>::create(/*m_origin->dir()->plugins().absoluteFilePath(pluginName), */this);
//            if (not single->isEmpty()){
//                // FIXME we have only need`t library name and version, version can be modern
//                push_back(single);
//            }
//        }
//    }
//    emit pluginListChanged(this->pluginList());
    //    qDebug() << "PLUGINLIST" << this->pluginList();
}

bool PluginHandler::registratePlugin(Plugin *object)
{
    if (object){
        this->push_back(PluginSinglePtr::create(object, this));
        return true;
    }
    return false;
}

std::optional <AFlib::Plugin*> PluginHandler::load(QString pluginName, bool isCritical)
{
    // TODO [Settings] rewrite it all
    Q_UNUSED(pluginName)
    Q_UNUSED(isCritical)
//    for (auto it = begin(); it != end(); ++it)
//        if (pluginName == it->get()->name()){
//            if (it->get() ->loader()->isLoaded()){
//                qDebug() << QString("Plugin %1 already loaded").arg(pluginName);
//                break;
//            }
//            else {
//                m_libraries->load(m_origin->dir()->libraries(), it->get()->libraries(), pluginName);
//                auto object = it->get()->loader()->instance();
//                if (object){
//                    return qobject_cast <AFlib::Plugin*> (object);
//                }
//                else {
//                    m_libraries->unload(pluginName);
//                    // TODO set Critical error from this if it happened
//                    if (isCritical)
//                        qCritical() << QString("Plugin %1 are not loaded").arg(pluginName);
//                    break;
//                }
//            }
//        }

    return std::nullopt;
}

void PluginHandler::initStaticPlugin()
{
    for (auto it = begin(); it != end(); ++it){
        auto plugin = it->get()->instance();
        if (plugin.has_value())
            plugin.value()->initialize();
    }
}

void PluginHandler::startStaticPlugin()
{
    for (auto it = begin(); it != end(); ++it){
        auto plugin = it->get()->instance();
        if (plugin.has_value())
            plugin.value()->readyToStart();
    }
}

QList <const Plugin *> PluginHandler::pluginList() const
{
    QList <const Plugin*> list;
    for (auto it = begin(); it != end(); ++it)
        if (it->get()->instance())
            list.push_back(it->get()->instance().value());
    return list;
}
