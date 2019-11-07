#include "origin.h"

using namespace AFlib;

Origin::Origin(QObject *parent) : QObject(parent)
{
    m_dir = DirPtr::create(this);
    m_setting = SettingPtr::create(this);
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
