#ifndef MUNNIN_SYSTEM_H
#define MUNNIN_SYSTEM_H

#include <QtCore/QObject>
#include <QtCore/QString>

#include "enum.h"

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

#endif // MUNNIN_SYSTEM_H
