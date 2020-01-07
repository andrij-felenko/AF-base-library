#ifndef LIB_BASEAF_CATEGORY_H
#define LIB_BASEAF_CATEGORY_H

#include <QtCore/QObject>

namespace AFlib {
    QByteArray crypt(QByteArray array);
    QByteArray encrypt(QByteArray array);

    void toCrypt(uint &data);
    void toCrypt(quint64 &data);
    void toEncrypt(uint &data);
    void toEncrypt(quint64 &data);
    uint getCrypt(uint data);
    uint getEncrypt(uint data);
    quint64 getCrypt(quint64 data);
    quint64 getEncrypt(quint64 data);

    uint codeString(QString str, uint size);
}

#endif // LIB_BASEAF_SINGLECARD_H
