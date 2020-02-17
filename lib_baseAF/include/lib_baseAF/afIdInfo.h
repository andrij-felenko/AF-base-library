#ifndef LIB_BASEAF_ID_INFO_H
#define LIB_BASEAF_ID_INFO_H

#include <QtCore/QObject>

namespace AFlib::id {
    class Info;
}

class AFlib::id::Info : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name        READ name        WRITE setName        NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
public:
    explicit Info(QObject* parent = nullptr);
    Info(QString name, QString descr, QObject* parent = nullptr);

    virtual QString name()        const final { return m_name; }
    virtual QString description() const final { return m_description; }

public slots:
    void setName(QString name);
    void setDescription(QString description);

protected:
    QString m_name;
    QString m_description;

    friend QDataStream &operator << (QDataStream& stream, const Info& data);
    friend QDataStream &operator >> (QDataStream& stream,       Info& data);

signals:
    void nameChanged(QString name);
    void descriptionChanged(QString description);
};

#endif // LIB_BASEAF_ID_INFO_H
