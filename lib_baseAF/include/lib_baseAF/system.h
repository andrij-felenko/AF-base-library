#ifndef LIB_BASEAF_SYSTEM_H
#define LIB_BASEAF_SYSTEM_H

#include <QtCore/QObject>
#include <QtCore/QString>

#include "enumAF.h"

namespace AFlib {
class System : public QObject
{
    Q_OBJECT
public:
    explicit System(QObject *parent = nullptr);

    enum class SystemOS {
        Android,
        Linux,
        Windows,
        Mac,
        iOS,
    }; Q_ENUMS(SystemOS)
    static SystemOS OS();

    enum class Anchor {
        Top,
        Left,
        Right,
        Bottom,
        Undefined,
    }; Q_ENUMS(Anchor)

signals:
    //

public slots:
    //
};
}

#endif // LIB_BASEAF_SYSTEM_H
