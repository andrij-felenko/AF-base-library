#pragma once
#include <optional>
#include <QtCore/QDir>
#include <QtCore/QObject>
#include <QtCore/QString>

namespace AFlib {
    class User;
    typedef QSharedPointer <const User> UserPtr;
    typedef QList <UserPtr> UserPtrList;
}

class AFlib::User final : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString uid   READ uid   WRITE setUid   NOTIFY uidChanged)
    Q_PROPERTY(QString icon  READ icon  WRITE setIcon  NOTIFY iconChanged)
    Q_PROPERTY(QString login READ login WRITE setLogin NOTIFY loginChanged)
public:
    explicit User(QObject* parent = nullptr);
    virtual ~User() = default;

    QString uid() const;
    QString icon() const;
    QString login() const;

    friend QDataStream & operator >> (QDataStream& stream,       User &user);
    friend QDataStream & operator << (QDataStream& stream, const User &user);

public slots:
    void setUid(QString uid);
    void setIcon(QString icon);
    void setLogin(QString login);

signals:
    void uidChanged(QString uid);
    void iconChanged(QString icon);
    void loginChanged(QString login);

private:
    QString m_uid;
    QString m_icon;
    QString m_login;
    QString m_password;

    QString password() const;
    void setPassword(QString password);
    friend class UserHandler;
};
