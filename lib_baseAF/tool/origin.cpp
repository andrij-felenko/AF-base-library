#include "origin.h"

using namespace AFlib;

Origin::Origin(QObject *parent) : QObject(parent)
{
    m_dir = DirPtr::create(this);
    m_storage = StoragePtr::create(m_dir, this);
    m_setting = SettingPtr::create(this);
//    m_user.create(this);

//    QObject::connect(m_user.data(), &Account::loginChanged, m_dir.data(), &Dir::switchToUser);
}

DirPtr Origin::dir() const
{
    return m_dir;
}

SettingPtr Origin::setting() const
{
    return m_setting;
}

StoragePtr Origin::storage() const
{
    return m_storage;
}

//AccountPtr Origin::user() const
//{
//    return m_user;
//}
