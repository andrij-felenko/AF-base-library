#ifndef MUNNIN_ENUM_H
#define MUNNIN_ENUM_H

#include <QObject> 

namespace AFlib {
namespace Enum {
    Q_NAMESPACE

    enum class Continent {
        Europe = 0,
        Africa,
        North_America,
        South_America,
        Oceania,
        East_Asia,
        West_Asia,

        None = 0xF
    };
}
}

typedef AFlib::Enum::Continent AFcontinent;

#endif // MUNNIN_ENUM_H
