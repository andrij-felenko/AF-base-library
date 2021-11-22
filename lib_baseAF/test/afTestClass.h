#ifndef AFTESTCLASS_H
#define AFTESTCLASS_H

#include <QtTest/QTest>

class AFtestClass : public QObject
{
    Q_OBJECT
private slots:
    void saveObjectBit();
//    void condition();
};

#endif // AFTESTCLASS_H
