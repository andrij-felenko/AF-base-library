#ifndef BASEAPPLICATIONAF_H
#define BASEAPPLICATIONAF_H

#include <QtCore/QObject>
#include <QtCore/QCoreApplication>
#include "userHandler.h"
#include "pluginHandler.h"
#include "origin.h"

namespace AFlib {
template <typename T>
class BaseApplicationAF;
}

template <typename T>
class AFlib::BaseApplicationAF final : public T
{
public:
    explicit BaseApplicationAF(int argc, char** argv);

    OriginPtr getOrigin() const;
    UserHandlerPtr getUserHandler() const;
    bool registratePlugin(Plugin* object);

signals:
    //

public slots:
    //

private:
    OriginPtr origin;
    PluginHandler* m_plugin;
    UserHandlerPtr userHandler;
};

typedef AFlib::BaseApplicationAF <QCoreApplication> CoreApplicationAF;

#endif // BASEAPPLICATIONAF_H
