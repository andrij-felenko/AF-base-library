#ifndef LIB_ACCOUNT_ACCOUNT_H
#define LIB_ACCOUNT_ACCOUNT_H

#include <QtCore/QDir>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QJsonObject>
#include <QtCore/QSharedPointer>

#include "lib_baseAF/afAccountInfo.h"
//#include "lib_baseAF/afAPI.h"

namespace AFaccount {
    class Account;
    typedef QSharedPointer <Account> AccountPtr;
    typedef std::vector <AccountPtr> AccountPtrV;

    AccountPtr own();
}

namespace AFlib {
    class API;
}

class AFaccount::Account : public AFaccount::Info
{
public:
    explicit Account(QObject* parent = nullptr);
//    Account(QJsonObject obj, QObject* parent = nullptr);
    Account(AFlib::IdObjectPtr ptr, QObject* parent = nullptr);
    Account(const QByteArray& data, QObject* parent = nullptr);

    bool check(QString password) const;
//    InfoPtrList friendList() const;

    virtual QString login() const final;

    operator QJsonObject() const;
    virtual QJsonObject toJson() const override;

    QByteArray toByteArray() const;
    void reset();

    AFlib::id::AccList_b friendList() const;
    AFlib::id::AccList_b  groupList() const;

public slots:
    void setLogin(QString login);

private:
    virtual QString passwordHash() const final;
//    InfoPtrList m_friendList;

    void addAccount   (AFlib::IdObjectPtr account);
    void removeAccount(AFlib::IdObjectPtr account);

    // TODO group need to be public and we need to access it, maybe we need to public global group
    // but it not now
    void addGroup   (AFlib::IdObjectPtr account);
    void removeGroup(AFlib::IdObjectPtr account);

    friend class Storage;
    friend class AFlib::API;
};

#endif // LIB_ACCOUNT_ACCOUNT_H
