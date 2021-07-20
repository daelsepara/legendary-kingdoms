#ifndef __LOCATION__HPP__
#define __LOCATION__HPP__

#include <map>

namespace Location
{
    enum class Type
    {
        NONE = -1,
        CLIFFTOP,
        SALTDAD,
        CURSUS,
        LUUTANESH,
        CAPE_PORTA,
        LONGPORT_BAY
    };

    std::map<Location::Type, const char *> Description = {
        {Location::Type::NONE, "Unknown"},
        {Location::Type::CLIFFTOP, "Clifftop"},
        {Location::Type::SALTDAD, "Saltdad"},
        {Location::Type::CURSUS, "Cursus"},
        {Location::Type::LUUTANESH, "Luutanesh"},
        {Location::Type::CAPE_PORTA, "Cape Porta"},
        {Location::Type::LONGPORT_BAY, "Longport Bay"}};

    enum class BattleField
    {
        NONE = -1,
        LEFT_FLANK_FRONT,
        LEFT_FLANK_SUPPORT,
        CENTRE_FRONT,
        CENTRE_SUPPORT,
        RIGHT_FLANK_FRONT,
        RIGHT_FLANK_SUPPORT
    };

    std::map<Location::BattleField, const char *> BattleFieldDescription = {
        {Location::BattleField::NONE, "(None)"},
        {Location::BattleField::LEFT_FLANK_FRONT, "Left Flank Front"},
        {Location::BattleField::LEFT_FLANK_SUPPORT, "Left Flank Support"},
        {Location::BattleField::CENTRE_FRONT, "Centre Front"},
        {Location::BattleField::CENTRE_SUPPORT, "Center Support"},
        {Location::BattleField::RIGHT_FLANK_FRONT, "Right Flank Front"},
        {Location::BattleField::RIGHT_FLANK_SUPPORT, "Right Flank Support"}};
}

#endif