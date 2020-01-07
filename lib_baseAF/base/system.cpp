#include "system.h"

using namespace AFlib;

System::System(QObject *parent) : QObject(parent)
{
    //
}

System::SystemOS System::OS()
{
#if defined(Q_OS_ANDROID)
    return SystemOS::Android;
#elif defined(Q_OS_LINUX)
    return SystemOS::Linux;
#elif defined(Q_OS_WIN)
    return SystemOS::Windows;
#elif defined(Q_OS_IOS)
    return SystemOS::iOS;
#elif defined(Q_OS_MAC)
    return SystemOS::Mac;
#endif
}
