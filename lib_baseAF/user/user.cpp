#include "user.h"

AFlib::User::User(QObject *parent) : QObject(parent)
{
    //
}

QString AFlib::User::login() const
{
    return m_login;
}

QString AFlib::User::uid() const
{
    return m_uid;
}

QString AFlib::User::icon() const
{
    return m_icon;
}

void AFlib::User::setLogin(QString login)
{
    if (m_login == login)
        return;

    m_login = login;
    emit loginChanged(m_login);
}

void AFlib::User::setPassword(QString password)
{
    m_password = password;
}

void AFlib::User::setUid(QString uid)
{
    if (m_uid == uid)
        return;

    m_uid = uid;
    emit uidChanged(m_uid);
}

void AFlib::User::setIcon(QString icon)
{
    if (m_icon == icon)
        return;

    m_icon = icon;
    emit iconChanged(m_icon);
}

namespace AFlib {
QDataStream & operator >>(QDataStream &stream, User& user)
{
    return stream >> user.m_uid >> user.m_icon >> user.m_login >> user.m_password;
}

QDataStream & operator <<(QDataStream &stream, const User& user)
{
    return stream << user.m_uid << user.m_icon << user.m_login << user.m_password;
}
}
