#include <QtCore/QCoreApplication>
#include "dataServer.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);
    app.setOrganizationName("AF");
    app.setApplicationName("base_af_server");

    ushort port = 55323;
    for (auto it : app.arguments())
        if (it.left(5).toLower() == "port:"){
            port = it.right(it.length() - 5).toUShort();
            break;
        }

//    CurrencyAF::ByDate::ignoreUpdater = true;
//    CurrencyAF::Data::addCurrency(CurrencyAF::Type::allShort());

//    CurrencyHttpServer server(&app);
//    server.listen(QHostAddress::Any, port);
//    qDebug() << QCoreApplication::translate("CurrencyServer", "Running on http://127.0.0.1:%1/").arg(port);

    return app.exec();
}
