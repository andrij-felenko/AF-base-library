#ifndef LIB_ACCOUNTAF_ACCOUNT_INFO_H
#define LIB_ACCOUNTAF_ACCOUNT_INFO_H

#include <QtCore/QDir>
#include <QtCore/QJsonObject>
#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QString>

#include <AFbase/AfId>

namespace AFaccount {
    class Info;
    typedef QSharedPointer <Info> InfoPtr;
    typedef QList <InfoPtr> InfoPtrList;
}

class AFaccount::Info : public QObject, public AFlib::id::Object
{
    Q_OBJECT
//    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QString mail READ mail WRITE setMail NOTIFY mailChanged)
public:
    explicit Info(QObject* parent = nullptr);
    Info(uint id, QObject* parent = nullptr);
    Info(const AFlib::IdObject &obj, QObject* parent = nullptr);
    Info(QJsonObject obj, QObject* parent = nullptr);
    Info(AFlib::AccountIdType type, QObject* parent = nullptr);

    virtual QByteArray icon() const final;
    virtual QString mail() const final;

    operator QJsonObject() const;
    virtual QJsonObject toJson() const;

public slots:
    virtual void setIcon(const QByteArray& icon) final;
    virtual void setMail(const QString& mail) final;

signals:
    void iconChanged(QString icon);
    void mailChanged(QString mail);

protected:
    friend QDataStream & operator >> (QDataStream& stream,       Info &info);
    friend QDataStream & operator << (QDataStream& stream, const Info &info);
};

typedef QSharedPointer <AFaccount::Info> AccInfoPtr;
typedef QList <AccInfoPtr> AccInfoPtrList;

QDataStream& operator >> (QDataStream& stream,       AFaccount::InfoPtrList& list);
QDataStream& operator << (QDataStream& stream, const AFaccount::InfoPtrList& list);

#endif // LIB_ACCOUNTAF_ACCOUNT_INFO_H
