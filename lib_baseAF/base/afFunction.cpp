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
        else if ((it == QChar('.') || it == QChar(',')) && not pointPresent){
            if (i == 0)
                validText += "0";
            validText += ".";
            pointPresent = true;
        }
        i++;
    }
    return validText;
}

// TODO make more universality
//!
//! \brief Function::toString
//! \param number - само число
//! \param numeral_system - система чисел в яку перетворюємо
//! \param size - розмір майбутнього рядка, за замовчуванням -1
//! \param separate - роздільник числа, за замовчуванням 4
//! \param showNS - флаг чи треба показувати спереду систему чисел
//! \return повертаемо рядок виду   [ 2] 1110
//!
std::string Function::toString(uint number, uchar numeral_system, char size, uchar separate, bool showNS)
{
    if (size != 2 && size > 0)
        size = std::ceil(size / std::log2(numeral_system));

    std::string str("");
    if (size != -1)
        str = std::string(size, '0');

    uchar i = 0;
    for (; number != 0; number /= numeral_system, i++){
        uchar n = number % numeral_system;
        char c = n > 9 ? 'A' + n - 10 : '0' + n;

        if (size == -1)
            str.insert(0, 1, c);
        else
            str[size - i - 1] = c;
    }

    uchar t_size = uchar(str.size());
    if (t_size % separate)
        str.insert(0, separate - t_size % separate, '.');
    t_size = uchar(str.size());

    for (char i = t_size - separate; i > 0; i -= separate)
        str.insert(i, 1, ' ');

    if (showNS){
        std::string ns = "[" + std::to_string(numeral_system) + "]";
        if (ns.size() == 3)
            ns.insert(1, 1, ' ');
        str.insert(0, ns + ' ');
    }

    return str;
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
