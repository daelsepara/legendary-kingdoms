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
        LONGPORT_BAY,
        LHASBREATH,
        MORDAIN,
        CHALICE,
        TUMBLESTONES
    };

    std::map<Location::Type, const char *> Description = {
        {Location::Type::NONE, "Unknown"},
        {Location::Type::CLIFFTOP, "Clifftop"},
        {Location::Type::SALTDAD, "Saltdad"},
        {Location::Type::CURSUS, "Cursus"},
        {Location::Type::LUUTANESH, "Luutanesh"},
        {Location::Type::CAPE_PORTA, "Cape Porta"},
        {Location::Type::LONGPORT_BAY, "Longport Bay"},
        {Location::Type::LHASBREATH, "Lhasbreath"},
        {Location::Type::MORDAIN, "Mordain"},
        {Location::Type::CHALICE, "Chalice"},
        {Location::Type::TUMBLESTONES, "Tumblestones"}};

    enum class BattleField
    {
        NONE = -1,
        LEFT_FLANK_FRONT,
        LEFT_FLANK_SUPPORT,
        CENTER_FRONT,
        CENTER_SUPPORT,
        RIGHT_FLANK_FRONT,
        RIGHT_FLANK_SUPPORT
    };

    enum class Zone
    {
        NONE = -1,
        LEFT_FLANK,
        CENTER,
        RIGHT_FLANK
    };

    std::map<Location::BattleField, const char *> BattleFieldDescription = {
        {Location::BattleField::NONE, "(None)"},
        {Location::BattleField::LEFT_FLANK_FRONT, "Left Flank Front"},
        {Location::BattleField::LEFT_FLANK_SUPPORT, "Left Flank Support"},
        {Location::BattleField::CENTER_FRONT, "Centre Front"},
        {Location::BattleField::CENTER_SUPPORT, "Center Support"},
        {Location::BattleField::RIGHT_FLANK_FRONT, "Right Flank Front"},
        {Location::BattleField::RIGHT_FLANK_SUPPORT, "Right Flank Support"}};
}

#endif