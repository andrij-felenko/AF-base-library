#ifndef LIB_BASEAF_SERVER_SETTING_H
#define LIB_BASEAF_SERVER_SETTING_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QSharedPointer>
#include <QtCore/QDateTime>
#include <QtCore/QSettings>

namespace AFlib {
    class ServerSetting;
    typedef QSharedPointer <ServerSetting> ServerSettingPtr;

    ServerSettingPtr afServerSettings(QObject* parent = nullptr);
}

// TODO add read and write settings to some file, i don't know how now
class AFlib::ServerSetting : public QObject
{
    Q_OBJECT
public:
    explicit ServerSetting(QObject *parent = nullptr);

    bool addUrl(QString key, QString url);
    QString getUrl(QString key, QString defaultValue = "") const;
    // TODO it must be differebt for every users, cause we load only users depends updates
    QDateTime lastStorageUpdate() const;

private:
    QMap <QString, QString> m_linkMap;
    QDateTime m_lastStorageUpdate;
    QSettings m_settings;
};

AFlib::ServerSettingPtr afServerSettings(QObject* parent = nullptr);

#endif // LIB_BASEAF_SERVER_SETTING_H
