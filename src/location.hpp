#ifndef __LOCATION__HPP__
#define __LOCATION__HPP__

#include <map>

namespace Location
{
    enum class Type
    {
        NONE = -1,
        DESERT,
        JUNGLE,
        CLIFFTOP,
        SALTDAD,
        CURSUS,
        LUUTANESH,
        CAPE_PORTA,
        LONGPORT_BAY,
        LHASBREATH,
        MORDAIN,
        CHALICE,
        TUMBLESTONES,
        WITHERED_STEPPES,
        CAVES_OF_URANU,
        PALACE_OF_UNBRAAKI,
        SALTDAD_ARENA,
        SALT_MINES,
        WEST_ROAD,
        EAST_ROAD,
        BLACKWALL,
        SALT_PLAINS,
        CROSSROADS,
        LHASBREATH_OASIS,
        GRANITE_HILLS
    };

    std::map<Location::Type, const char *> Description = {
        {Location::Type::NONE, "Unknown"},
        {Location::Type::DESERT, "Desert"},
        {Location::Type::JUNGLE, "Jungle"},
        {Location::Type::CLIFFTOP, "Clifftop"},
        {Location::Type::SALTDAD, "Saltdad"},
        {Location::Type::CURSUS, "Cursus"},
        {Location::Type::LUUTANESH, "Luutanesh"},
        {Location::Type::CAPE_PORTA, "Cape Porta"},
        {Location::Type::LONGPORT_BAY, "Longport Bay"},
        {Location::Type::LHASBREATH, "Lhasbreath"},
        {Location::Type::MORDAIN, "Mordain"},
        {Location::Type::CHALICE, "Chalice"},
        {Location::Type::TUMBLESTONES, "Tumblestones"},
        {Location::Type::WITHERED_STEPPES, "Withered Steppes"},
        {Location::Type::CAVES_OF_URANU, "Caves of Uranu"},
        {Location::Type::PALACE_OF_UNBRAAKI, "Palace of Unbraaki"},
        {Location::Type::SALTDAD_ARENA, "Saltdad Arena"},
        {Location::Type::SALT_MINES, "Saltdad Salt Mines"},
        {Location::Type::WEST_ROAD, "Great West Road"},
        {Location::Type::EAST_ROAD, "Great East Road"},
        {Location::Type::BLACKWALL, "Blackwall"},
        {Location::Type::SALT_PLAINS, "Salt Plains"},
        {Location::Type::CROSSROADS, "Crossroads"},
        {Location::Type::LHASBREATH_OASIS, "Lhasbreath Oasis"},
        {Location::Type::GRANITE_HILLS, "Granite Hills"}};

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