#ifndef LIB_ACCOUNTAF_ACCOUNT_API_H
#define LIB_ACCOUNTAF_ACCOUNT_API_H

#include "accountStorage.h"

namespace AFaccount {
    class API;
    typedef QSharedPointer <API> AccountAPIPtr;
    typedef QList <AccountAPIPtr> AccountAPIPtrList;
}

class AFaccount::API : public QObject
{
public:
    explicit API(QObject* parent = nullptr);

    static std::optional <QString> login(const QString& login, const QString& password);

signals:
    void emitLogin(const QString&/* login */, const QString&/* password */, const QString&/* device name */);
    void emitRemind(const QString&/* email or login */);
    void emitLogout(const QString&/* login */, const QString&/* device name */);
    void emitCheckNew(const QDateTime&/* date time last update */);
    void emitRegistrate(const Account&/* account */);
//    void emitUpdateNew(const QList <Histories>&);
};

#endif // LIB_ACCOUNTAF_ACCOUNT_API_H
