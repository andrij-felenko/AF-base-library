#include "origin.h"

using namespace AFlib;

Origin::Origin(QObject *parent) : QObject(parent)
{
    m_dir.create(this);
    m_setting.create(this);
    m_user.create(this);
}

DirPtr Origin::dir() const
{
    return m_dir;
}

SettingPtr Origin::setting() const
{
    return m_setting;
}

UserPtr Origin::user() const
{
    return m_user;
}
