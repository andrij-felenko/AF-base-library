#ifndef LIB_BASEAF_FUNCTION_H
#define LIB_BASEAF_FUNCTION_H

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QLibrary>
#include <QtCore/QPluginLoader>
#include <QtCore/QDir>
#include <random>
#include <charconv>

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
    static std::string toString(uint number, uchar numeral_system = 1, char size = -1,
                                uchar separate = 4, bool showNS = true);
    static QString digitToString(double number, int symbolAfterPoint = 3);
    // TODO add template
    static int randomInt(int from, int to);
    static QDateTime nullDateTime();

    template <typename Return_arg, unsigned size>
    static std::pair <unsigned, Return_arg> findMaxElement(std::array <Return_arg, size> array)
    {
        auto maxValue = array[0];
        unsigned maxIndex = 0;
        for (unsigned i = 1; i< array.size(); i++)
            if (array[i] > maxValue){
                maxIndex = i;
                maxValue = array[i];
            }
        return std::make_pair (maxIndex, maxValue);
    }

    template <typename Return_arg, unsigned size>
    static std::pair <unsigned, Return_arg> findMinElement(std::array <Return_arg, size> array)
    {
        auto maxValue = array[0];
        unsigned maxIndex = 0;
        for (unsigned i = 1; i< array.size(); i++)
            if (array[i] < maxValue){
                maxIndex = i;
                maxValue = array[i];
            }
        return std::make_pair (maxIndex, maxValue);
    }

    template <typename Return_arg, unsigned size>
    static std::list <unsigned> findMaxIndexList(std::array <Return_arg, size> array)
    {
        auto maxValue = array[0];
        for (unsigned i = 1; i < array.size(); i++)
            if (array[i] > maxValue)
                maxValue = array[i];
        std::list <unsigned> list;
        for (unsigned i = 0; i < array.size(); i++)
            if (array[i] == maxValue)
                list.push_back(i);
        return list;
    }

    template <typename Return_arg, unsigned size>
    static std::list <unsigned> findMinIndexList(std::array <Return_arg, size> array)
    {
        auto minValue = array[0];
        for (unsigned i = 1; i < array.size(); i++)
            if (array[i] < minValue)
                minValue = array[i];
        std::list <unsigned> list;
        for (unsigned i = 0; i < array.size(); i++)
            if (array[i] == minValue)
                list.push_back(i);
        return list;
    }

signals:
    //
public slots:
    //
};
}

typedef AFlib::Function AFfunction;

#endif // LIB_BASEAF_FUNCTION_H
