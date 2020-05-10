#ifndef LIB_BASEAF_FUNCTION_H
#define LIB_BASEAF_FUNCTION_H

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QLibrary>
#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <random>

namespace AFlib {
class Function : public QObject
{
    Q_OBJECT
public:
    explicit Function(QObject *parent = nullptr);

    static QString libraryName(QString libraryName);
    static QPair <bool, QString> filePath(QString name, QDir dir = QDir::current());
    static QPair <bool, QString> libraryPath(QString name, QDir dir = QDir::current());
    static QSharedPointer <QLibrary> library(QString name, QDir dir = QDir::current(),
                                                QObject* parent = nullptr);
    static QSharedPointer <QPluginLoader> pluginLoader(QString name, QDir dir = QDir::current(),
                                                       QObject* parent = nullptr);

    // work with numbers
    static QString stringToDigit(QString text);
    static QString digitToString(double number, int symbolAfterPoint = 3);
    static int randomInt(int from, int to);

signals:
    //
public slots:
    //
};
}

#endif // LIB_BASEAF_FUNCTION_H
