#include <QtCore/QStringList>
#include <QtCore/QDebug>

#include "version.h"

using namespace AFlib;

Version::Version(QString version)
{
    QStringList list = version.split(".");
    if (list.length() > 0){
        m_major = list.at(0).toUInt();
        if (list.length() > 1){
            m_minor = list.at(1).toUInt();
            if (list.length() > 2)
                m_build = list.at(2).toUInt();
        }
    }
    else
        qWarning() << "Cannot parse version: " << version;
}

Version::Version(const Version &version)
    : m_major(version.m_major), m_minor(version.m_minor), m_build(version.m_build)
{
    //
}

unsigned int Version::major_() const
{
    return m_major;
}

unsigned int Version::minor_() const
{
    return m_minor;
}

unsigned int Version::build() const
{
    return m_build;
}

QString Version::string() const
{
    return QString("%1.%2.%3").arg(m_major).arg(m_minor).arg(m_build);
}

bool Version::operator ==(const Version &version_b) const
{
    if (m_major != version_b.major_())
        return false;
    if (m_minor != version_b.minor_())
        return false;
    if (m_build != version_b.build())
        return false;
    return true;
}
