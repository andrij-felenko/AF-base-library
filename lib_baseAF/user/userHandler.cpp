#include "userHandler.h"
#include <QtCore/QDataStream>
#include <QtCore/QFile>
#include <QtCore/QCryptographicHash>
#include <QtCore/QDebug>

AFlib::UserHandler::UserHandler(OriginPtr origin, QObject *parent)
    : QObject(parent), m_origin(origin)
{
//    m_origin = origin;
    qDebug() << "fgfg";
    refresh();
}

QStringList AFlib::UserHandler::userList() const
{
    QStringList list;
    for (auto it : m_userList)
        list.push_back(it->login());
    return list;
}

void AFlib::UserHandler::refresh()
{
    m_userList.clear();
    QFile file(m_origin->dir()->users().absoluteFilePath("list.afud"));
    if (file.open(QIODevice::ReadWrite)){
        QDataStream stream(&file);
        while (!stream.atEnd()){
            auto user = QSharedPointer <User>::create();
            stream >> *user.data();
            m_userList.push_back(user);
        }
        file.close();
    }
    emit userListChanged(userList());
}

void AFlib::UserHandler::save()
{
    QFile file(m_origin->dir()->users().absoluteFilePath("list.afud"));
    file.remove();
    if (file.open(QIODevice::WriteOnly)){
        QDataStream stream(&file);
        for (auto it : m_userList)
            stream << *it.data();
        file.close();
    }
}

void AFlib::UserHandler::addUser(QString uid, QString login, QString password, QString icon)
{
    UserPtr user;
    user = UserPtr::create();
    user->m_uid = uid;
    user->m_icon = icon;
    user->m_login = login;
    user->m_password = password;
    m_userList.push_back(user);
    emit userListChanged(userList());
}

bool AFlib::UserHandler::check(QString login, QString password)
{
    for (auto it : m_userList)
        if (it->m_login == login)
            if (it->m_password == QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Md5).toHex())){
                m_origin->user() = it;
                return true;
            }
    return false;
}
