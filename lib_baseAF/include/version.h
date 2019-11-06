#ifndef VERSION_H
#define VERSION_H

#include <QtCore/QString>

namespace AFlib {
class Version
{
public:
    Version(QString version = QString(""));
    Version(const Version& version);
    virtual ~Version() = default;

    virtual unsigned int major_() const final;
    virtual unsigned int minor_() const final;
    virtual unsigned int build () const final;

    QString string() const;

    bool operator == (const Version& version_b) const;

private:
    uint m_major :  9;
    uint m_minor :  9;
    uint m_build : 14;
};
}

#endif // VERSION_H
