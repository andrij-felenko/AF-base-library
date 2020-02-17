#ifndef LIB_ACCOUNTAF_ACCOUNT_INFO_H
#define LIB_ACCOUNTAF_ACCOUNT_INFO_H

#include <QtCore/QDir>
#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

#include "lib_baseAF/afId.h"

namespace AFaccount {
    class Info;
    typedef QSharedPointer <Info> InfoPtr;
    typedef QList <InfoPtr> InfoPtrList;
}

class AFaccount::Info : public AFlib::id::Info,
                        public AFlib::id::History,
                        public AFlib::id::Account_bit
{
    Q_OBJECT
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QString mail READ mail WRITE setMail NOTIFY mailChanged)
public:
    explicit Info(QObject* parent = nullptr);
    Info(uint id, QObject* parent = nullptr);
    Info(QJsonObject obj, QObject* parent = nullptr);
    Info(AFlib::AccountIdType type, QObject* parent = nullptr);

    virtual QString icon() const final { return m_icon; }
    virtual QString mail() const final { return m_mail; }

    operator QJsonObject() const;
    QJsonObject toJson() const;

public slots:
    virtual void setIcon(QString icon) final;
    virtual void setMail(QString mail) final;

signals:
    void iconChanged(QString icon);
    void mailChanged(QString mail);

protected:
    friend QDataStream & operator >> (QDataStream& stream,       Info &info);
    friend QDataStream & operator << (QDataStream& stream, const Info &info);

private:
    QString m_icon;
    QString m_mail;
};

QDataStream& operator >> (QDataStream& stream,       AFaccount::InfoPtrList& list);
QDataStream& operator << (QDataStream& stream, const AFaccount::InfoPtrList& list);

#endif // LIB_ACCOUNTAF_ACCOUNT_INFO_H
