#include "afAccountGroup.h"
#include "afAccountStorage.h"
#include "AFbase/AfEnum"
#include <QtCore/QPair>

typedef QPair <quint32, quint8> AccountWithAccess;
Q_DECLARE_METATYPE(AccountWithAccess);

AccountWithAccess makeAccountWithAccess(AFIdObject_bit account, AFaccount::AccessType access)
{
    return AccountWithAccess(account.toUInt32(), fromAccessType(access));
}

using namespace AFaccount;

Group::Group(QObject *parent) : AFaccount::Info(parent)
{
    auto subUsers = getMultiAttribute(AFattribute::UserList);
    // TODO parse subUsers
}

Group::Group(const AFlib::IdObject &account, QObject *parent)
    : AFaccount::Info(account, parent)
{
    //
}

InfoPtrList Group::userList()      const { return getByType(AccessType::User);      }
InfoPtrList Group::ownerList()     const { return getByType(AccessType::Owner);     }
InfoPtrList Group::creatorList()   const { return getByType(AccessType::Creator);   }
InfoPtrList Group::moderatorList() const { return getByType(AccessType::Moderator); }

QStringList Group::userNameList()      const { return getNameByType(AccessType::User);      }
QStringList Group::ownerNameList()     const { return getNameByType(AccessType::Owner);     }
QStringList Group::creatorNameList()   const { return getNameByType(AccessType::Creator);   }
QStringList Group::moderatorNameList() const { return getNameByType(AccessType::Moderator); }

void Group::addAccount(InfoPtr account, AccessType access)
{
    addAccount(account->object_b(), access);
}

void Group::removeAccount(InfoPtr account, AccessType access)
{
    removeAccount(account->object_b(), access);
}

void Group::changeAccess(InfoPtr account, AccessType newAccess)
{
    changeAccess(account->object_b(), newAccess);
}

void Group::addAccount(AFIdObject_bit account, AccessType access)
{
    QVariant var = QVariant::fromValue(makeAccountWithAccess(account, access));
    setMultiAttribute(AFattribute::GroupMembers, var, AFlib::HIdType::AddIdLine);
}

void Group::removeAccount(AFIdObject_bit account, AccessType access)
{
    QVariant var = QVariant::fromValue(makeAccountWithAccess(account, access));
    setMultiAttribute(AFattribute::GroupMembers, var, AFlib::HIdType::RemoveIdLine);
}

void Group::changeAccess(AFIdObject_bit account, AccessType newAccess)
{
    QVariant var = QVariant::fromValue(makeAccountWithAccess(account, newAccess));
    setMultiAttribute(AFattribute::GroupMembers, var, AFlib::HIdType::EditIdLine);
}

InfoPtrList Group::getByType(AccessType type, bool inheritType) const
{
    InfoPtrList list;
    for (auto it = m_userList.begin(); it != m_userList.end(); ++it){
        if (type == it.value())
            list += it.key();
        else if (inheritType && uint(type) > uint(it.value()))
            list += it.key();
    }
    return list;
}

QStringList Group::getNameByType(AccessType type, bool inheritType) const
{
    QStringList list;
    for (auto it = m_userList.begin(); it != m_userList.end(); ++it){
        if (type == it.value())
            list += it .key()->name();
        else if (inheritType && uint(type) > uint(it.value()))
            list += it.key()->name();
    }
    return list;
}

AccessType AFaccount::toAccessType(const quint8 i)
{
    return static_cast <AccessType> (i + static_cast <uint> (AccessType::First));
}

quint8 AFaccount::fromAccessType(AccessType type)
{
    return static_cast <uint>(type) - static_cast <uint>(AccessType::First);
}

namespace AFaccount {
QDataStream &operator >>(QDataStream &stream, AFaccount::Group &group)
{
    int lenUserList;
    stream >> lenUserList;
    for (int i = 0; i < lenUserList; i++){
        int accessType;
        AFIdAccount id;
        stream >> id >> accessType;
        auto acc = AFaccount::storage()->getInfo(id);
        if (not acc.isNull())
            group.m_userList.insert(acc, AccessType(accessType));
    }
    stream >> static_cast <Info&> (group);
    return stream;
}

QDataStream &operator <<(QDataStream &stream, const AFaccount::Group &group)
{
    stream << group.m_userList.count();
    for (auto it = group.m_userList.begin(); it != group.m_userList.end(); ++it)
        stream << it.key()->owner() << int(it.value());
    stream << static_cast <const Info&> (group);
    return stream;
}
}

QDataStream &operator >>(QDataStream &stream, GPtrList &list)
{
    int count = 0;
    stream >> count;
    for (int i = 0; i < count; i++){
        GroupPtr temp = GroupPtr::create();
        stream >> *temp;
        list.push_back(temp);
    }
    return stream;
}

QDataStream &operator <<(QDataStream &stream, const GPtrList &list)
{
    stream << list.length();
    for (auto it : list)
        stream << it.data();
    return stream;
}
