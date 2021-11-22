#include "guiPlugin.h"
#include "guiApplication.h"

AFquick::GuiPlugin::GuiPlugin(QString name, QObject *parent)
    : QObject(parent), m_name(name)
{
    //
}

QString AFquick::GuiPlugin::name() const
{
    return m_name;
}

QString AFquick::GuiPlugin::icon() const
{
    // for work need to override
    return "";
}

void AFquick::GuiPlugin::clickOnMenu(int index)
{
    Q_UNUSED(index)
    // NOTE need to override to use
}

void AFquick::GuiPlugin::setContextProperty(QString name, QObject *object)
{
    m_afApp->setContextProperty(name, object);
}

void AFquick::GuiPlugin::setContextProperty(QString name, QVariant variant)
{
    m_afApp->setContextProperty(name, variant);
}

void AFquick::GuiPlugin::setAfApp(GuiApplication *afApp)
{
    m_afApp = afApp;
}
