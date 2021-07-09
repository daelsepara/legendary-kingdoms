#ifndef __BOOK1__HPP__
#define __BOOK1__HPP__

#include "story.hpp"

namespace Book1
{
    auto Stories = std::vector<Story::Base *>();

    void InitializeStories()
    {
        Book1::Stories = {};
    }
}
#endif
