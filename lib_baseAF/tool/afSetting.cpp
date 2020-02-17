#include "afSetting.h"
#include <QtCore/QVariant>

AFlib::Setting::Setting(QObject *parent) : QObject(parent)
{
    //
}

std::optional <QVariant> AFlib::Setting::value(const QString &key)
{
    if (key == "currency_server")
        return "127.0.0.1";

    return std::nullopt;
}
