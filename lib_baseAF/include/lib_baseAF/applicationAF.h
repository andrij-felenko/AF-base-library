#ifndef LIB_BASEAF_APPLICATIONAF_H
#define LIB_BASEAF_APPLICATIONAF_H

#include <QtCore/QObject>
#include <QtCore/QCoreApplication>
#include "accountStorage.h"
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
//        m_userHandler = AccountStoragePtr::create(m_origin, this);
        m_plugin = new PluginHandler(m_origin, this);
    }

    bool registratePlugin(Plugin* object) { return m_plugin->registratePlugin(object); }
    OriginPtr      getOrigin()      const { return m_origin; }
//    AccountStoragePtr getUserHandler() const { return m_userHandler; }

private:
    OriginPtr m_origin;
    PluginHandler* m_plugin;
//    AccountStoragePtr m_userHandler;
};

typedef AFlib::BaseApplicationAF <QCoreApplication> CoreApplicationAF;

#endif // LIB_BASEAF_APPLICATIONAF_H
