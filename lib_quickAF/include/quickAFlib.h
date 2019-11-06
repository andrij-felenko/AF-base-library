#ifndef QUICKMUNNIN_H
#define QUICKMUNNIN_H

#include <QtCore/QObject>
#include <QtQml/QQmlExtensionPlugin>
#include <QtQml/qqml.h>

namespace AFlib {
class QuickMunnin : public QQmlExtensionPlugin
{
    Q_OBJECT
//    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override;
};
}

#endif // QUICKMUNNIN_H
