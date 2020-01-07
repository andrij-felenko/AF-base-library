#include "enumAF.h"

AFlib::HistoryIdType AFlib::toHistoryIdType(const quint8 i)
{
    return HistoryIdType(uint(HistoryIdType::AddIdLine) + i);
}

AFlib::SavedIdType AFlib::toSavedIdType(const quint8 i)
{
    return SavedIdType(uint(SavedIdType::TemperarySaved) + i);
}

bool AFlib::operator ==(const AFlib::AccountIdType &f, const AFlib::AccountIdType &s){
    if (uint(f) == uint(s))
        return true;

    typedef AccountIdType AIT;
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
