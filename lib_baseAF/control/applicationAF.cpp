#include "applicationAF.h"

using namespace AFlib;

template<typename T>
BaseApplicationAF<T>::BaseApplicationAF(int argc, char **argv) : T(argc, argv)
{
    origin.create(this);
    userHandler.create(this);
    m_plugin = new PluginHandler(origin, this);
}

template<typename T>
UserHandlerPtr BaseApplicationAF<T>::getUserHandler() const
{
    return userHandler;
}

template<typename T>
bool BaseApplicationAF<T>::registratePlugin(Plugin *object)
{
    m_plugin->registratePlugin(object);
}

template<typename T>
OriginPtr BaseApplicationAF<T>::getOrigin() const
{
    return origin;
}
