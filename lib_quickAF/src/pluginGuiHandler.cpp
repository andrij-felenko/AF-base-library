#include "pluginGuiHandler.h"

using namespace AFlib;

PluginGuiHandler::PluginGuiHandler(OriginPtr origin, QObject *parent) : PluginHandler(origin, parent)
{
    m_view = QSharedPointer <QQuickView>::create();

    setContextProperty("AFlibApp", this);
    qmlRegisterType <Plugin> ("AFlibApp", 1, 0, "AFlib_PluginSingle");

    m_view->setSource(QUrl("qrc:/App/qtquick/main.qml"));
    m_view->resize(360, 480);
    m_view->show();
}

QString PluginGuiHandler::current() const
{
    return m_current;
}

void PluginGuiHandler::setCurrent(QString current)
{
    if (m_current == current)
        return;

    // TODO rewrite all
//    for (auto it = begin(); it != end(); ++it)
//        if (it->get()->name() == current){
//            auto plugin = load(current);
//            if (plugin) {
//                m_current = current;
//                emit currentChanged(m_current);
//                // here we need to do smth if current plugin not have gui
//                auto guiPlugin = static_cast <AFlib::PluginGui*> (plugin.value());
//                if (guiPlugin){
//                    QQmlComponent component(m_view->engine(), guiPlugin->source());
//                    QQuickItem* pluginContent = m_view->rootObject()->findChild <QQuickItem*> ("pluginContent");
//                    qobject_cast <QQuickItem*> (component.create())->setParentItem(pluginContent);
//                }
//            }
//            else
//                qDebug() << "not create" << it->get()->loader()->errorString();
//        }
}

void PluginGuiHandler::setContextProperty(QString name, QObject *obj)
{
    m_view->rootContext()->setContextProperty(name, obj);
}

void PluginGuiHandler::setContextProperty(QHash<QString, QObject *> hash)
{
    for (auto it = hash.begin(); it != hash.end(); ++it)
        setContextProperty(it.key(), it.value());
}
