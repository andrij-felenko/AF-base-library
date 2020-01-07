#ifndef LIB_BASEAF_ENUMAF_H
#define LIB_BASEAF_ENUMAF_H

#include <QtCore/QObject>

namespace AFlib {
    Q_NAMESPACE

    enum class Schedule {
        Once = 0,
        Daily,
        Weekly,
        Monthly,
        Yearly,
        Periodical,
        OnRequest,
    };
    
    enum class Continent {
        Europe = 0,
        Africa,
        North_America,
        South_America,
        Oceania,
        East_Asia,
        West_Asia,

        None = 0xF
    };

/// Id`s enum list --------------------------------------------------------------------------------
    enum class HistoryIdType {
        AddIdLine = 0xE0F100,
        EditIdLine,
        RemoveIdLine,
        //
        First = AddIdLine,
    };
    HistoryIdType toHistoryIdType(const quint8 i);

    enum class SavedIdType {
        TemperarySaved = 0xE0F200,
        LocaleSaved,
        SavedOnWayToServer,
        SavedOnServer,

        First = TemperarySaved,
    };
    SavedIdType toSavedIdType(const quint8 i);

    enum class AccountIdType {
        LocalUser = 0xE0F300,
        LocalOrganization,
        GlobalUser,
        GlobalOrganization,
        User,
        Organization,
        Local,
        Global,

        First = LocalUser,
    };
    bool operator == (const AccountIdType& f, const AccountIdType& s);
}

#endif // LIB_BASEAF_ENUMAF_H
