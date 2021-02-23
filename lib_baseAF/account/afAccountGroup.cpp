#include "afAccountGroup.h"
#include "afAccountStorage.h"
#include "AFbase/AfEnum"
#include <QtCore/QPair>

typedef QPair <quint32, quint8> AccountWithAccess;
Q_DECLARE_METATYPE(AccountWithAccess);

AccountWithAccess makeAccountWithAccess(AFIdObject_bit account, AFaccount::AccessType access)
{
    return AccountWithAccess(account.id(), fromAccessType(access));
}

using namespace AFaccount;

Group::Group(QObject *parent) : AFaccount::Info(parent)
{
    auto subUsers = m_ptr->getMultiAttribute(AFattribute::UserList);
    Q_UNUSED(subUsers)
    // TODO parse subUsers
}

Group::Group(AFlib::IdObjectPtr ptr, QObject *parent)
    : AFaccount::Info(ptr, parent)
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
    addAccount(account->afObject()->object_b(), access);
}

void Group::removeAccount(InfoPtr account, AccessType access)
{
    removeAccount(account->afObject()->object_b(), access);
}

void Group::changeAccess(InfoPtr account, AccessType newAccess)
{
    changeAccess(account->afObject()->object_b(), newAccess);
}

void Group::addAccount(AFIdObject_bit account, AccessType access)
{
    QVariant var = QVariant::fromValue(makeAccountWithAccess(account, access));
    m_ptr->setMultiAttribute(AFattribute::GroupMembers, var, AFlib::HIdType::AddLine);
}

void Group::removeAccount(AFIdObject_bit account, AccessType access)
{
    QVariant var = QVariant::fromValue(makeAccountWithAccess(account, access));
    m_ptr->setMultiAttribute(AFattribute::GroupMembers, var, AFlib::HIdType::RemoveLine);
}

void Group::changeAccess(AFIdObject_bit account, AccessType newAccess)
{
    QVariant var = QVariant::fromValue(makeAccountWithAccess(account, newAccess));
    m_ptr->setMultiAttribute(AFattribute::GroupMembers, var, AFlib::HIdType::EditLine);
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
            list += it .key()->afObject()->name();
        else if (inheritType && uint(type) > uint(it.value()))
            list += it.key()->afObject()->name();
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
