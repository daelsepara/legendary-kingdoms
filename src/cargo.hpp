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
        {Cargo::Type::SALT, "SALT"},
        {Cargo::Type::CROPS, "CROPS"},
        {Cargo::Type::STEEL, "STEEL"},
        {Cargo::Type::SPICES, "SPICES"},
        {Cargo::Type::WINE, "WINE"},
        {Cargo::Type::SLAVES, "SLAVES"}};
}

#endif