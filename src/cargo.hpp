#ifndef __CARGO__HPP__
#define __CARGO__HPP__

#include <map>

namespace Cargo
{
    enum class Type
    {
        NONE = -1,
        SALT,
        CROPS,
        STEEL,
        SPICES,
        WINE,
        SLAVES
    };

    std::map<Cargo::Type, const char *> Description = {
        {Cargo::Type::NONE, "(None)"},
        {Cargo::Type::SALT, "Salt"},
        {Cargo::Type::CROPS, "Crops"},
        {Cargo::Type::STEEL, "Steel"},
        {Cargo::Type::SPICES, "Spices"},
        {Cargo::Type::WINE, "Wine"},
        {Cargo::Type::SLAVES, "Slaves"}};
}

#endif
