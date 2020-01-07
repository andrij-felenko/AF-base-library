#ifndef LIB_ACCOUNTAF_ACCOUNT_INFO_H
#define LIB_ACCOUNTAF_ACCOUNT_INFO_H

#include <QtCore/QDir>
#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

#include "lib_baseAF/id.h"

namespace AFaccount {
    class Info;
    typedef QSharedPointer <Info> InfoPtr;
    typedef QList <InfoPtr> InfoPtrList;
}

class AFaccount::Info : public AFlib::HistoryId, public AFlib::AccountId
{
    Q_OBJECT
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QString mail READ mail WRITE setMail NOTIFY mailChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit Info(QObject* parent = nullptr);
    Info(uint id, QObject* parent = nullptr);
    Info(QJsonObject obj, QObject* parent = nullptr);
    Info(AFlib::AccountIdType type, QObject* parent = nullptr);

    virtual QString icon() const final { return m_icon; }
    virtual QString mail() const final { return m_mail; }
    virtual QString name() const final { return m_name; }

    operator QJsonObject() const;

public slots:
    virtual void setIcon(QString icon) final;
    virtual void setMail(QString mail) final;
    virtual void setName(QString name) final;

signals:
    void iconChanged(QString icon);
    void mailChanged(QString mail);
    void nameChanged(QString name);

protected:
    friend QDataStream & operator >> (QDataStream& stream,       Info &info);
    friend QDataStream & operator << (QDataStream& stream, const Info &info);

private:
    QString m_icon;
    QString m_mail;
    QString m_name;
};

QDataStream& operator >> (QDataStream& stream,       AFaccount::InfoPtrList& list);
QDataStream& operator << (QDataStream& stream, const AFaccount::InfoPtrList& list);

#endif // LIB_ACCOUNTAF_ACCOUNT_INFO_H
