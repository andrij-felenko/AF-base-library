#ifndef LIB_ACCOUNT_ACCOUNT_H
#define LIB_ACCOUNT_ACCOUNT_H

#include <QtCore/QDir>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QJsonObject>

#include "accountInfo.h"
#include "lib_currencyAF/currencyType.h"

namespace AFaccount {
    class Account;
    typedef QSharedPointer <Account> AccountPtr;
    typedef QList <AccountPtr> AccountPtrList;
}

class AFaccount::Account : public AFaccount::Info
{
public:
    explicit Account(QObject* parent = nullptr);
    Account(QJsonObject obj, QObject* parent = nullptr);
    static AccountPtr me();

    bool check(QString password) const;
    QList <CurrencyEnum> currencyList() const;
    QStringList currencyStringList() const;
    InfoPtrList friendList() const;

    virtual QString login() const final;

    operator QJsonObject() const;
    virtual QJsonObject toJson() const override;

public slots:
    void setLogin(QString login);

protected:
    friend QDataStream & operator >> (QDataStream& stream,       Account &account);
    friend QDataStream & operator << (QDataStream& stream, const Account &account);

private:
    virtual QString passwordHash() const final;
    InfoPtrList m_friendList;
    QList <CurrencyEnum> m_currencyList;

    friend class Storage;
};

QDataStream& operator >> (QDataStream& stream,       AFaccount::AccountPtrList& list);
QDataStream& operator << (QDataStream& stream, const AFaccount::AccountPtrList& list);

#endif // LIB_ACCOUNT_ACCOUNT_H
