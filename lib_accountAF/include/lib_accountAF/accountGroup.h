#ifndef LIB_ACCOUNTAF_ACCOUNT_GROUP_H
#define LIB_ACCOUNTAF_ACCOUNT_GROUP_H

#include <QtCore/QDir>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QString>

#include "accountInfo.h"

namespace AFaccount {
    class Group;
    typedef QSharedPointer <Group> GroupPtr;
    typedef QList <GroupPtr> GroupPtrList;
    typedef GroupPtrList GPtrList;

    enum class AccessType {
        Owner = 0xE0E100,
        Moderator,
        Creator,
        User,

        Default = User,
    };
}

class AFaccount::Group : public Info
{
    Q_OBJECT
public:
    explicit Group(QObject* parent = nullptr);
    Group(const AFlib::IdObject& account, QObject* parent = nullptr);

    InfoPtrList userList() const;
    InfoPtrList ownerList() const;
    InfoPtrList creatorList() const;
    InfoPtrList moderatorList() const;

    QStringList userNameList() const;
    QStringList ownerNameList() const;
    QStringList creatorNameList() const;
    QStringList moderatorNameList() const;

    void addAccount  (InfoPtr account, AccessType access);
    void removeAcount(InfoPtr account, AccessType access);
    void changeAccess(InfoPtr account, AccessType newAccess);

    void addAccount  (AFIdObject_bit account, AccessType access);
    void removeAcount(AFIdObject_bit account, AccessType access);
    void changeAccess(AFIdObject_bit account, AccessType newAccess);

    //! Call update InfoPtr for all user from list to get new values
    void updateInfoList();

protected:
    friend QDataStream & operator >> (QDataStream& stream,       Group &group);
    friend QDataStream & operator << (QDataStream& stream, const Group &group);

private:
    QMap <InfoPtr, AccessType> m_userList;

    InfoPtrList getByType(AccessType type, bool inheritType = true) const;
    QStringList getNameByType(AccessType type, bool inheritType = true) const;
};

QDataStream& operator >> (QDataStream& stream,       AFaccount::GPtrList& list);
QDataStream& operator << (QDataStream& stream, const AFaccount::GPtrList& list);

#endif // LIB_ACCOUNTAF_ACCOUNT_GROUP_H
