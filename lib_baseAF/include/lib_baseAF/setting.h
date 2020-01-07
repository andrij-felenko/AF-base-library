#ifndef LIB_BASEAF_SETTING_H
#define LIB_BASEAF_SETTING_H

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

    static std::optional <QVariant> value(const QString& key);

signals:
    //

public slots:
    //

private:
    //
};

#endif // LIB_BASEAF_SETTING_H
