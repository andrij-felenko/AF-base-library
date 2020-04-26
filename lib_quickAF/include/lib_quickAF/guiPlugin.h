#ifndef LIB_QUICK_AF_GUI_PLUGIN_H
#define LIB_QUICK_AF_GUI_PLUGIN_H

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QUrl>

namespace AFquick {
    class GuiPlugin;
    class GuiApplication;
    typedef QSharedPointer <GuiPlugin> GuiPluginPtr;
}

class AFquick::GuiPlugin : public QObject
{
    Q_OBJECT
public:
    explicit GuiPlugin(QString name, QObject* parent = nullptr);
    virtual ~GuiPlugin() = default;

    virtual QString name() const final;
    virtual QUrl qmlUrl() const  = 0;

    virtual void initialize() = 0;

    void clickOnMenu(int index);

protected:
    virtual void setContextProperty(QString name, QObject* object) final;
    virtual void setContextProperty(QString name, QVariant variant) final;

private:
    QString m_name;

    friend class GuiApplication;
    GuiApplication* m_afApp;
    virtual void setAfApp(GuiApplication* afApp) final;
};

typedef AFquick::GuiPluginPtr AFguiPluginPtr;

#endif
