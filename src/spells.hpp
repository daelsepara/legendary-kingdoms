#ifndef __SPELLS__HPP__
#define __SPELLS__HPP__

namespace Spells
{
    enum class Type
    {
        NONE = -1
    };

    class Base
    {
    public:

        const char* Name = NULL;

        Spells::Type Type = Spells::Type::NONE;

        bool Charged = false;

        int Recharge = 0;

        virtual void Effect() {};
        
        Base(const char* name, Spells::Type type, int recharge)
        {
            Name = 0;

            Type = type;

            Recharge = recharge;
        }
    };
}

#endif