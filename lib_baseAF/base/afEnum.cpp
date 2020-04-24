#include "afEnum.h"

AFlib::HistoryIdType AFlib::toHistoryIdType(const quint8 i)
{
    return HistoryIdType(uint(HistoryIdType::AddIdLine) + i);
}

AFlib::SavedIdType AFlib::toSavedIdType(const quint8 i)
{
    return SavedIdType(uint(SavedIdType::TemperarySaved) + i);
}

bool operator ==(AFlib::AccountIdType f, AFlib::AccountIdType s){
    if (uint(f) == uint(s))
        return true;

    typedef AFlib::AccountIdType AIT;
    switch (f) {
    case AIT::User:               return uint(f) == uint(AIT::LocalUser)         || uint(f) == uint(AIT::GlobalUser);
    case AIT::Local:              return uint(f) == uint(AIT::LocalUser)         || uint(f) == uint(AIT::LocalOrganization);
    case AIT::Global:             return uint(f) == uint(AIT::GlobalUser)        || uint(f) == uint(AIT::GlobalOrganization);
    case AIT::LocalUser:          return uint(f) == uint(AIT::User)              || uint(f) == uint(AIT::Local);
    case AIT::GlobalUser:         return uint(f) == uint(AIT::User)              || uint(f) == uint(AIT::Global);
    case AIT::Organization:       return uint(f) == uint(AIT::LocalOrganization) || uint(f) == uint(AIT::GlobalOrganization);
    case AIT::LocalOrganization:  return uint(f) == uint(AIT::Local)             || uint(f) == uint(AIT::Organization);
    case AIT::GlobalOrganization: return uint(f) == uint(AIT::Global)            || uint(f) == uint(AIT::Organization);
    }
    return false;
}

quint8 AFlib::fromHisToInt(AFlib::HistoryIdType type)
{
    return quint8(type) % 0b1000;
}

quint8 AFlib::fromSaveToInt(AFlib::SavedIdType type)
{
    return quint8(type) % 0b100;
}

bool AFlib::isHIdEnable(AFlib::HistoryIdType type)
{
    if (type == HIdType::RemoveIdLine)
        return false;
    if (type == HIdType::InnactivateIdLine)
        return false;

    return true;
}

QDebug operator <<(QDebug d, AFlib::FileType type)
{
    switch (type){
    case AFlib::FileType::Data:    d << "FileType::Data";    break;
    case AFlib::FileType::Json:    d << "FileType::Json";    break;
    case AFlib::FileType::Config:  d << "FileType::Config";  break;
    case AFlib::FileType::Account: d << "FileType::Account"; break;
    case AFlib::FileType::Setting: d << "FileType::Setting"; break;
    }
    return d;
}

AFlib::AccountIdType AFlib::toAccountIdType(const quint8 i)
{
    return static_cast <AccountIdType> (i + static_cast <uint> (AccountIdType::First));
}

quint8 AFlib::fromAccountIdType(AFlib::AccountIdType type)
{
    return static_cast <uint>(type) - static_cast <uint>(AccountIdType::First);
}
