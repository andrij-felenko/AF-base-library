#include "quickAFlib.h"
#include "lib_baseAF/afSystem.h"

using namespace AFlib;

void QuickMunnin::registerTypes(const char *uri)
{
    Q_ASSERT(uri == QLatin1String("MunninQuickTypes"));
    qmlRegisterModule(uri, 1, 0);
    //        qmlRegisterUncreatableMetaObject(
    //          MQEnum::staticMetaObject, // static meta object
    //          "MQBase",                // import statement (can be any string)
    //          1, 0,                          // major and minor version of the import
    //          "MQEnum",                 // name in QML (does not have to match C++ name)
    //          "Error: only enums"            // error in case someone tries to create a MyNamespace object
    //        );
    qmlRegisterType <AFlib::System> ("MQBase", 1, 0, "MQSystem");
}
