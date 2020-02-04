#include "accountGroup.h"
#include "accountStorage.h"

using namespace AFaccount;

Group::Group(QObject *parent) : AFaccount::Info(parent)
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

namespace AFaccount {
QDataStream &operator >>(QDataStream &stream, AFaccount::Group &group)
{
    int lenUserList;
    stream >> lenUserList;
    for (int i = 0; i < lenUserList; i++){
        int accessType;
        AFlib::id::Account_bit id;
        stream >> id >> accessType;
        auto acc = accountStorage()->getInfo(id);
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
        stream << static_cast <AFlib::id::Account_bit> (*it.key()) << int(it.value());
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
