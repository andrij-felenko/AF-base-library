#include "guiApplication.h"
#include "guiSize.h"
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickItem>
#include <AFaccount/AfAccountStorage>
#include <AFbase/AfStorage>

QUrl m_defaultQmlUrl("qrc:/AFquick/Base/App.qml");

AFquick::GuiApplication::GuiApplication(int &argc, char **argv)
    : QGuiApplication(argc, argv),
      m_useAFaccount(false), m_initializeAll(false), m_monomenu(false)
{
    // init default
    AFlib::afStorage();
    AFaccount::storage(this);

    m_menu = new MenuModel(this);

    m_view = new QQuickView();
    connect(m_view, &QQuickView::heightChanged, this, &GuiApplication::checkPortrait);
    connect(m_view, &QQuickView:: widthChanged, this, &GuiApplication::checkPortrait);

    setContextProperty("AFapp", this);
    setContextProperty("AFsize", afGuiSize().data());

    connect(m_menu, &MenuModel::getCount, [this](){ return m_pluginList.length(); });

    connect(m_menu, &MenuModel::getNameByIndex,
            [this](int index){ return m_pluginList[index]->name(); });

    connect(m_menu, &MenuModel::getIconByIndex,
            [this](int index){ return m_pluginList[index]->icon(); });

    connect(m_menu, &MenuModel::isCurrentByIndex,
            [this](int index){ return m_pluginList[index]->name() == m_currentPluginName; });
}

AFquick::GuiApplication::~GuiApplication()
{
    // TODO close all plugin step by step with saving data
    // TODO close main source
    m_view->setSource(QUrl(""));
    m_view->deleteLater();
}

void AFquick::GuiApplication::setAFaccountFlag(bool isUseAFaccount)
{
    if (m_useAFaccount == isUseAFaccount)
        return;

    m_useAFaccount = isUseAFaccount;
    emit accountChanged(account());
}

void AFquick::GuiApplication::initializePlugin()
{
    if (m_initializeAll)
        return;

    for (auto it : m_pluginList)
        it->initialize();
}

void AFquick::GuiApplication::initializePlugin(QString name)
{
    for (auto it : m_pluginList)
        if (it->name() == name){
            it->initialize();
            break;
        }
}

void AFquick::GuiApplication::show()
{
    if (m_pluginList.isEmpty())
        return;

    initializePlugin();
    checkPortrait();
    m_view->resize(700, 450);
    m_view->show();
}

void AFquick::GuiApplication::quit()
{
    // TODO
}

void AFquick::GuiApplication::setMainQmlUrl(QUrl url)
{
    m_view->setSource(url);
}

void AFquick::GuiApplication::setMainQml(QString path)
{
    setMainQmlUrl(QUrl(path));
}

void AFquick::GuiApplication::setMonomenu(QStringList list)
{
    Q_UNUSED(list)
    // TODO

    m_monomenu = true;
    emit monomenuChanged(true);
}

void AFquick::GuiApplication::setMonomenu(QHash<QString, QString> hash)
{
    Q_UNUSED(hash)
    // TODO

    m_monomenu = true;
    emit monomenuChanged(true);
}

QAbstractListModel *AFquick::GuiApplication::menuModel() const
{
    return static_cast <QAbstractListModel*> (m_menu);
}

bool AFquick::GuiApplication::addPlugin(GuiPluginPtr plugin)
{
    for (const auto& it : m_pluginList)
        if (it->name() == plugin->name())
            return false;

    m_initializeAll = false;
    m_pluginList.push_back(plugin);
    plugin->setAfApp(this);

    if (m_pluginList.length() == 1)
        setCurrentPlugin(plugin->name());

    if (m_pluginList.length() == 2)
        emit monoobjectChanged(monoobject());

    m_menu->updateData();

    return true;
}

void AFquick::GuiApplication::setContextProperty(QString name, QObject *object)
{
    m_view->rootContext()->setContextProperty(name, object);
}

void AFquick::GuiApplication::setContextProperty(QString name, QVariant variant)
{
    m_view->rootContext()->setContextProperty(name, variant);
}

bool AFquick::GuiApplication::setCurrentPlugin(const QString &name)
{
    for (const auto& it : m_pluginList)
        if (it->name() == name){
            if (setContentUrl(it->qmlUrl())){
                m_currentPluginName = name;
                break;
            }
            return false;
        }

    m_menu->updateData();
    return true;
}

bool AFquick::GuiApplication::account() const
{
    return m_useAFaccount;
}

bool AFquick::GuiApplication::monomenu() const
{
    return m_monomenu && monoobject();
}

bool AFquick::GuiApplication::monoobject() const
{
    return m_pluginList.length() == 1;
}

bool AFquick::GuiApplication::portrait() const
{
    return m_portrait;
}

void AFquick::GuiApplication::clickOnMenu(int index)
{
    if (monomenu())
        m_pluginList[0]->clickOnMenu(index);

    if (index < m_pluginList.length())
        setCurrentPlugin(m_pluginList[index]->name());
}

bool AFquick::GuiApplication::setContentUrl(QUrl url)
{
    // TODO
//    if (monoobject()){
//        qDebug() << "Mono mode not contains plugins, can't set different item.";
//        return false;
//    }

    if (m_view->filePath().isEmpty())
        setMainQmlUrl(m_defaultQmlUrl);

    QQuickItem* contentItem = m_view->rootObject()->findChild <QQuickItem*> ("contentItem");
    if (contentItem == nullptr){
        qDebug() << "Not found content item.";
        return false;
    }

    QQmlComponent component(m_view->engine(), url, this);
    auto newItem = qobject_cast <QQuickItem*> (component.create());
    if (component.isError()){
        qDebug() << "Plugin can't load: " << component.errorString();
        return false;
    }
    newItem->setParentItem(contentItem);
    return true;
}

void AFquick::GuiApplication::checkPortrait()
{
    bool portrait = m_view->height() > m_view->width();
    if (m_portrait == portrait)
        return;

    m_portrait = portrait;
    emit portraitChanged(portrait);
}

