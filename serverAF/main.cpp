#include <QtCore/QCoreApplication>
#include "server.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName("AF");
    app.setApplicationName("base_af_server");

    ushort port = 55323;
    for (const auto &it : app.arguments())
        if (it.left(5).toLower() == "port:"){
            port = it.right(it.length() - 5).toUShort();
            break;
        }

    Server server;
    server.listen(QHostAddress::Any, port);
    qDebug() << QCoreApplication::translate("AFServer", "Running on http://127.0.0.1:%1/").arg(port);

    return app.exec();
}
