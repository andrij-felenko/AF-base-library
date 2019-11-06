#ifndef QUICKPLUGINHANDLER_H
#define QUICKPLUGINHANDLER_H

#include <QtCore/QObject>
#include <QtQml/QQmlComponent>
#include <QtQml/QQmlContext>
#include <QtQuick/QQuickItem>
#include <QtQuick/QQuickView>

#include "pluginGui.h"
#include "pluginHandler.h"

namespace AFlib {
class PluginGuiHandler : public PluginHandler
{
    Q_OBJECT
    Q_PROPERTY(QString current READ current WRITE setCurrent NOTIFY currentChanged)
public:
    explicit PluginGuiHandler(QObject *parent = nullptr);

    QString current() const;

signals:
    void currentChanged(QString current);

public slots:
    void setCurrent(QString current);

private:
    QString m_current;
    QList <QSharedPointer <PluginGui>> m_pluginList;
    QSharedPointer <QQuickView> m_view;

    void setContextProperty(QString name, QObject* obj);
    void setContextProperty(QHash <QString, QObject*> hash);
};
}

#endif // QUICKPLUGINHANDLER_H
