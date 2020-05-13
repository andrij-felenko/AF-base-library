#include <QtCore/QDebug>

#include "afFunction.h"
#include "afSystem.h"
#include <QtCore/QDateTime>

using namespace AFlib;

Function::Function(QObject *parent) : QObject(parent)
{
    //
}

QSharedPointer <QLibrary> Function::library(QString name, QDir dir, QObject* parent)
{
    auto file = Function::filePath(Function::libraryName(name), dir);
    if (not file.first)
        qWarning() << QString("Library %1 not found in %2 directory.").arg(name, dir.path());
    else
        return QSharedPointer <QLibrary>::create(file.second, parent);
    return QSharedPointer <QLibrary> ();
}

QSharedPointer <QPluginLoader> Function::pluginLoader(QString name, QDir dir, QObject* parent)
{
    auto file = Function::filePath(Function::libraryName(name), dir);
    if (not file.first)
        qWarning() << QString("Library %1 not found in %2 directory.").arg(name, dir.path());
    else
        return QSharedPointer <QPluginLoader>::create(file.second, parent);
    return QSharedPointer <QPluginLoader> ();
}

QString Function::stringToDigit(QString text)
{
    // convert to normal text
    QString validText("");
    bool pointPresent(false);
    int i(0);
    for (auto it : text){
        if (it.isDigit())
            validText += it;
        else if ((it == "." || it == ",") && not pointPresent){
            if (i == 0)
                validText += "0";
            validText += ".";
            pointPresent = true;
        }
        i++;
    }
    return validText;
}

QString Function::digitToString(double number, int symbolAfterPoint)
{
    QString ret = QString::number(number);
    auto list = ret.split('.');
    if (list.length() == 0)
        return QString("0");

    ret = list.first();
    if (list.length() > 1){
        int tail;
        if (symbolAfterPoint + 1 <= list.at(1).count()){
            tail = list.at(1).left(symbolAfterPoint + 1).toInt();
            tail = ((tail % 10 >= 5) ? 1 : 0) + tail / 10;
        }
        else
            tail = list.at(1).toInt();

        while (tail != 0 && tail % 10 == 0)
            tail /= 10;

        if (tail > 0){
            ret += ".";
            ret += QString::number(tail);
        }
    }
    return ret;
}

int Function::randomInt(int from, int to)
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(from, to);
    return dis(gen);
}

QDateTime Function::nullDateTime()
{
#if QT_VERSION >= 0x051300
    return QDateTime(QDate(1960, 1, 1).startOfDay());
#else
    return QDateTime(QDate(1960, 1, 1));
#endif
}

QString Function::libraryName(QString libraryName)
{
#if defined(Q_OS_WIN)
    return QString(libraryName + ".dll");
#elif defined(Q_OS_LINUX) || defined(Q_OS_ANDROID)
    return QString("lib" + libraryName + ".so");
#endif
    // TODO add Mac
}

QPair <bool, QString> Function::filePath(QString name, QDir dir)
{
    if (not dir.exists())
        qWarning() << QObject::tr("Directory %1 not found.").arg(dir.path());
    else if (not dir.exists(name))
        qWarning() << QObject::tr("Cannot found file %1 in %2 directory").arg(name, dir.path());
    else
        return QPair <bool, QString>(true, dir.absoluteFilePath(name));
    return QPair <bool, QString> (false, "");
}

QPair <bool, QString> Function::libraryPath(QString name, QDir dir)
{
    return Function::filePath(Function::libraryName(name), dir);
}
