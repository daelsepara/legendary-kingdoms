#ifndef __TOPIC__HPP__
#define __TOPIC__HPP__

#include <vector>

namespace Topic
{
    class Base
    {
    public:
        const char *Title = NULL;
        const char *Text = NULL;

        Base()
        {
        }

        Base(const char *title, const char *text)
        {
            Title = title;

            Text = text;
        }
    };

    std::vector<Topic::Base> ALL = {};
}
#endif