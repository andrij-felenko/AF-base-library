#ifndef LIB_QUICK_AF_GUI_APPLICATION_H
#define LIB_QUICK_AF_GUI_APPLICATION_H

#include <QtGui/QGuiApplication>
#include <QtQuick/QQuickView>
#include "guiPlugin.h"

namespace AFquick {
    class GuiApplication;
}

class AFquick::GuiApplication : public QGuiApplication
{
    Q_OBJECT
    Q_PROPERTY(bool account    READ account    NOTIFY accountChanged)
    Q_PROPERTY(bool monomenu   READ monomenu   NOTIFY monomenuChanged)
    Q_PROPERTY(bool monoobject READ monoobject NOTIFY monoobjectChanged)
    Q_PROPERTY(bool portrait   READ portrait   NOTIFY portraitChanged)
public:
    explicit GuiApplication(int &argc, char **argv);

    void initializePlugin();
    void initializePlugin(QString name);
    void setAFaccountFlag(bool isUseAFaccount);

    void show();
    void quit();

    // global set
    void setMainQmlUrl(QUrl url);
    void setMainQml(QString path);

    void setMonomenu(QStringList list);
    void setMonomenu(QHash <QString/*icon*/, QString/*name*/> hash);

    bool addPlugin(GuiPluginPtr plugin);

    void setContextProperty(QString name, QObject* object);
    void setContextProperty(QString name, QVariant variant);

    bool setCurrentPlugin(const QString& name);

    virtual bool account()    const final;
    virtual bool monomenu()   const final;
    virtual bool monoobject() const final;
    virtual bool portrait()   const final;

public slots:
    void clickOnMenu(int index);

signals:
    void accountChanged   (bool account);
    void monomenuChanged  (bool monomenu);
    void monoobjectChanged(bool monoobject);
    void portraitChanged  (bool portrait);

private:
    bool m_useAFaccount;
    bool m_portrait;
    bool m_initializeAll;
    bool m_monomenu;
    QString m_currentPluginName;

    QQuickView* m_view;
    QList <GuiPluginPtr> m_pluginList;

    void checkPortrait();
    bool setContentUrl(QUrl url);
};

typedef AFquick::GuiApplication AFguiApplication;

#endif
