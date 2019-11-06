#ifndef AFlib_USER_MANAGER
#define AFlib_USER_MANAGER

#include "origin.h"

namespace AFlib {
    class UserHandler;
    typedef QSharedPointer <UserHandler> UserHandlerPtr;
}

class AFlib::UserHandler final : public QObject
{
    Q_OBJECT
public:
    explicit UserHandler(QObject* parent = nullptr);
    virtual ~UserHandler() = default;

    QStringList userList() const;
    void refresh();
    void save();
    void addUser(QString uid, QString login, QString password, QString icon = "");
    bool check(QString login, QString password);

signals:
    void userListChanged(QStringList);

private:
    OriginPtr m_origin;
    UserPtrList m_userList;
};

#endif // AFlib_USER_MANAGER
