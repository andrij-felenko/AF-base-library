#ifndef LIB_BASEAF_FUNCTION_H
#define LIB_BASEAF_FUNCTION_H

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QLibrary>
#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include "afFunction_st.h"

namespace AFlib {
class Function : public QObject, public FunctionST
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
    static std::string toString(uint number, uchar numeral_system = 1, char size = -1,
                                uchar separate = 4, bool showNS = true);
    static QString digitToString(double number, int symbolAfterPoint = 3);
    // TODO add template
    static QDateTime nullDateTime();

signals:
    //
public slots:
    //
};
}

typedef AFlib::Function AFfunction;

#endif // LIB_BASEAF_FUNCTION_H
