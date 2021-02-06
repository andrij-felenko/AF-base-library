#include "afEnum.h"

AFlib::HistoryIdType AFlib::toHistoryIdType(const quint8 i)
{
    return HistoryIdType(uint(HistoryIdType::AddLine) + i);
}

AFlib::SavedIdType AFlib::toSavedIdType(const quint8 i)
{
    return SavedIdType(uint(SavedIdType::Temporary) + i);
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
    if (type == HIdType::RemoveLine)
        return false;
    if (type == HIdType::InnactivateLine)
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

QDataStream &operator <<(QDataStream &s, const AFlib::FileType& type)
{
    return s << static_cast <uint> (type);
}

QDataStream &operator >>(QDataStream &s, AFlib::FileType& type)
{
    uint value;
    s >> value;
    type = static_cast <AFlib::FileType> (value);
    return s;
}

std::string AFlib::saveTypeToString(AFlib::SavedIdType type)
{
    using namespace AFlib;
    switch (type) {
    case SavedIdType::Local: return "local";
    case SavedIdType::Temporary: return "temporary";
    case SavedIdType::OnServer:  return "on_server";
    case SavedIdType::OnTheWayToServer: return "on_the_way_to_server";
    }
    return "";
}

std::string AFlib::historyToString(AFlib::HistoryIdType type)
{
    using namespace AFlib;
    switch (type) {
    case HistoryIdType::AddLine: return "add_line";
    case HistoryIdType::EditLine: return "edit_line";
    case HistoryIdType::RemoveLine: return "remove_line";
    case HistoryIdType::SavedChangeLine: return "saved_change_line";
    case HistoryIdType::InnactivateLine: return "innactivate_line";
    case HistoryIdType::ActivateLine: return "activate_line";
    }
    return "";
}

std::string AFlib::historyToString(quint8 type)
{
    return historyToString(toHistoryIdType(type));
}

std::string AFlib::saveTypeToString(quint8 type)
{
    return saveTypeToString(toSavedIdType(type));
}
