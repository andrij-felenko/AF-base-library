#ifndef SETTING_H
#define SETTING_H

#include <QtCore/QObject>

namespace AFlib {
    class Setting;
    typedef QSharedPointer <Setting> SettingPtr;
}

class AFlib::Setting : public QObject
{
    Q_OBJECT
public:
    explicit Setting(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SETTING_H
