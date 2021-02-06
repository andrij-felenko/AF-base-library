#ifndef LIB_BASEAF_ENUMAF_H
#define LIB_BASEAF_ENUMAF_H

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QDataStream>

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
    
    //! \brief The Continent enum
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

    enum class FileType {
        Data = 0xE0F0F0,
        Account,
        Config,
        Setting,
        Json,
    };

    enum class HistoryIdType { // 3 bit`s
        AddLine = 0xE0F100,
        EditLine,
        RemoveLine,
        SavedChangeLine,
        InnactivateLine,
        ActivateLine,
        //
        First = AddLine,
    };
    HistoryIdType toHistoryIdType(const quint8 i);
    quint8 fromHisToInt(HistoryIdType type);
    typedef HistoryIdType HIdType;
    bool isHIdEnable(HistoryIdType type);
    std::string historyToString(HistoryIdType type);
    std::string historyToString(quint8 type);

    enum class SavedIdType { // 2 bit`s
        Temporary = 0xE0F200,
        Local,
        OnTheWayToServer,
        OnServer,

        First = Temporary,
    };
    SavedIdType toSavedIdType(const quint8 i);
    quint8 fromSaveToInt(SavedIdType type);
    typedef SavedIdType SIdType;
    std::string saveTypeToString(SavedIdType type);
    std::string saveTypeToString(quint8 type);

    /// Account id type enum
    //! \details default AccountIdType::LocalUser
    enum class AccountIdType {
        LocalUser = 0xE0F300, ///< User, that not accept from server yet, can't send information to global server.
        LocalOrganization,    ///< Local organization, use only on current device.
        GlobalUser,           ///< User, that registrate on server.
        GlobalOrganization,   ///< Organization, registrate on server, always have owner user.
        User,                 ///< User enum, unite both LocalUser and GlobalUser.
        Organization,         ///< Organization enum, unite LocalOrganization and GlobalOrganization
        Local,                ///< Use for show that enum is local.
        Global,               ///< Use for Global enum.

        First = LocalUser,
    };
    AccountIdType toAccountIdType(const quint8 i);
    quint8 fromAccountIdType(AccountIdType type);
}

bool operator == (AFlib::AccountIdType f, AFlib::AccountIdType s);
QDebug operator << (QDebug d, AFlib::FileType type);

QDataStream& operator << (QDataStream& s, const AFlib::FileType& type);
QDataStream& operator >> (QDataStream& s,       AFlib::FileType& type);

#endif // LIB_BASEAF_ENUMAF_H
