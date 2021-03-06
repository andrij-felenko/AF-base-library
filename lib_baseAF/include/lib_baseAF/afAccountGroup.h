#ifndef LIB_ACCOUNTAF_ACCOUNT_GROUP_H
#define LIB_ACCOUNTAF_ACCOUNT_GROUP_H

#include <QtCore/QDir>
#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QString>

#include "lib_baseAF/afAccountInfo.h"

namespace AFaccount {
    class Group;
    typedef QSharedPointer <Group> GroupPtr;
    typedef std::vector <GroupPtr> GroupPtrV;

    enum class AccessType {
        Owner = 0xE0E100,
        Moderator,
        Creator,
        User,

        Default = User,
        First = Owner,
    };

    AccessType toAccessType(const quint8 i);
    quint8 fromAccessType(AccessType type);
}

class AFaccount::Group : public Info
{
    Q_OBJECT
public:
    explicit Group(QObject* parent = nullptr);
    Group(AFlib::IdObjectPtr ptr, QObject* parent = nullptr);

    InfoPtrList userList() const;
    InfoPtrList ownerList() const;
    InfoPtrList creatorList() const;
    InfoPtrList moderatorList() const;

    QStringList userNameList() const;
    QStringList ownerNameList() const;
    QStringList creatorNameList() const;
    QStringList moderatorNameList() const;

    void addAccount   (InfoPtr account, AccessType access);
    void removeAccount(InfoPtr account, AccessType access);
    void changeAccess (InfoPtr account, AccessType newAccess);

    void addAccount   (AFIdObject_bit account, AccessType access);
    void removeAccount(AFIdObject_bit account, AccessType access);
    void changeAccess (AFIdObject_bit account, AccessType newAccess);

private:
    QMap <InfoPtr, AccessType> m_userList;

    InfoPtrList getByType(AccessType type, bool inheritType = true) const;
    QStringList getNameByType(AccessType type, bool inheritType = true) const;
};

#endif // LIB_ACCOUNTAF_ACCOUNT_GROUP_H
