#ifndef MUNNIN_DIR_H
#define MUNNIN_DIR_H

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

#endif // MUNNIN_DIR_H
