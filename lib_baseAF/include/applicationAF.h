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
    explicit BaseApplicationAF(int argc, char** argv) : T(argc, argv)
    {
        m_origin = OriginPtr::create(this);
        m_userHandler = UserHandlerPtr::create(m_origin, this);
        m_plugin = new PluginHandler(m_origin, this);
    }

    bool registratePlugin(Plugin* object) { return m_plugin->registratePlugin(object); }
    OriginPtr      getOrigin()      const { return m_origin; }
    UserHandlerPtr getUserHandler() const { return m_userHandler; }

private:
    OriginPtr m_origin;
    PluginHandler* m_plugin;
    UserHandlerPtr m_userHandler;
};

typedef AFlib::BaseApplicationAF <QCoreApplication> CoreApplicationAF;

#endif // BASEAPPLICATIONAF_H
