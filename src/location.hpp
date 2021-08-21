#ifndef __LOCATION__HPP__
#define __LOCATION__HPP__

#include <map>

namespace Location
{
    enum class Type
    {
        NONE = -1,
        DESERT,
        LHASBREATH_JUNGLE,
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
        NORTH_ROAD,
        WEST_ROAD,
        EAST_ROAD,
        BLACKWALL,
        SALT_PLAINS,
        CROSSROADS,
        LHASBREATH_OASIS,
        GRANITE_HILLS,
        UNDERGROUND_TUNNELS,
        NORTHWESTCOAST_VALLEY_OF_BONES,
        SOUTHCOAST_VALLEY_OF_BONES,
        SHADED_GATE,
        BEYOND_SHADED_GATE,
        SHADOW_LANDS,
        VIAAN_ISLAND,
        BANDO_VILLAGE,
        AZURE_DEEP,
        COLD_RIVER,
        TEMPLE_OFTHE_UNBROKEN,
        MORDAIN_EXCAVATED_DUNGEONS,
        LHASBREATH_RAPIDS,
        LHASBREATH_PYRAMID,
        TOMB_OF_IMOPPOSH,
        CHALICE_HILLSIDE,
        LHASBREATH_CLIFFSIDE
    };

    std::map<Location::Type, const char *> Description = {
        {Location::Type::NONE, "Unknown"},
        {Location::Type::DESERT, "Desert"},
        {Location::Type::LHASBREATH_JUNGLE, "Lhastbreath Jungle"},
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
        {Location::Type::NORTH_ROAD, "North Road"},
        {Location::Type::WEST_ROAD, "Great West Road"},
        {Location::Type::EAST_ROAD, "Great East Road"},
        {Location::Type::BLACKWALL, "Blackwall"},
        {Location::Type::SALT_PLAINS, "Salt Plains"},
        {Location::Type::CROSSROADS, "Crossroads"},
        {Location::Type::LHASBREATH_OASIS, "Lhasbreath Oasis"},
        {Location::Type::GRANITE_HILLS, "Granite Hills"},
        {Location::Type::UNDERGROUND_TUNNELS, "Underground Tunnels"},
        {Location::Type::NORTHWESTCOAST_VALLEY_OF_BONES, "Northwest coast, Valley of Bones"},
        {Location::Type::SOUTHCOAST_VALLEY_OF_BONES, "South coast, Valley of Bones"},
        {Location::Type::SHADED_GATE, "Shaded Gate"},
        {Location::Type::BEYOND_SHADED_GATE, "Land beyond the Shaded Gate"},
        {Location::Type::SHADOW_LANDS, "Shadow Lands"},
        {Location::Type::VIAAN_ISLAND, "Viaan Island"},
        {Location::Type::BANDO_VILLAGE, "Bando Village"},
        {Location::Type::AZURE_DEEP, "Azure Deep"},
        {Location::Type::COLD_RIVER, "Cold River"},
        {Location::Type::TEMPLE_OFTHE_UNBROKEN, "Temple of the Unbroken"},
        {Location::Type::MORDAIN_EXCAVATED_DUNGEONS, "Mordain, Excavated Dungeons"},
        {Location::Type::LHASBREATH_RAPIDS, "Lhasbreath, The Rapids"},
        {Location::Type::LHASBREATH_PYRAMID, "Pyramid beyond Lhasbreath"},
        {Location::Type::TOMB_OF_IMOPPOSH, "Tomb of Imopposh"},
        {Location::Type::CHALICE_HILLSIDE, "Hillside"},
        {Location::Type::LHASBREATH_CLIFFSIDE, "Cliffside"}};

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