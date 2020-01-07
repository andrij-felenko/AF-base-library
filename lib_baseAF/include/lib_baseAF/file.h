#ifndef LIB_BASEAF_DIR_H
#define LIB_BASEAF_DIR_H

#include <QtCore/QDir>
#include <QtCore/QFile>

namespace AFlib {
// TODO rewrite
class File
{
public:
    explicit File() = default;
    static bool rename(QDir dir, QString file, QString newName);
};
}

#endif // LIB_BASEAF_DIR_H
