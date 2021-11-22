#include "afTestClass.h"
#include <lib_baseAF/afIdObject.h>
#include <lib_baseAF/afFunction.h>
#include <lib_baseAF/afIdObject.h>
#include <QtCore/QBuffer>
#include <QDebug>

//void AFtestClass::condition()
//{
//    QVERIFY(true); // check that a condition is satisfied
//    QCOMPARE(2, 1); // compare two values
//}

void AFtestClass::saveObjectBit()
{
    AFlib::id::Object_bit first(0b11110111101110000101111111110000);
    qDebug() << "0c " << QString::number(first.toUInt32(), 8);
    qDebug() << "0b " << QString::number(first.toUInt32(), 2);
    qDebug() << first;
    qDebug() << "after first:";
    QByteArray dat;
    QBuffer buf(&dat);
    buf.open(QIODevice::WriteOnly);
    QDataStream s(&buf);
    s << first;
    buf.close();

    QBuffer buf2(&dat);
    buf.open(QIODevice::ReadOnly);
    QDataStream s2(&buf2);
    AFlib::id::Object_bit last;
    s >> last;
    buf2.close();

    qDebug() << "dat: " << dat << "\n" << last << first;

    QCOMPARE(first, last);
}
