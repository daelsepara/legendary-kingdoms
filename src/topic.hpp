#ifndef __TOPIC__HPP__
#define __TOPIC__HPP__

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

// JSON library
#include "nlohmann/json.hpp"

namespace Topics
{
    class Base
    {
    public:
        std::string Title = "";
        std::string Text = "";

        Base()
        {
        }

        Base(const char *title, const char *text)
        {
            Title = std::string(title);

            Text = std::string(text);
        }
    };

    std::vector<Topics::Base> ALL = {};

    void LoadTopics()
    {
        Topics::ALL.clear();

        std::ifstream ifs("topics-book1.json");

        if (ifs.good())
        {
            auto data = nlohmann::json::parse(ifs);

            ifs.close();

            auto topics = data["topics"];

            if (!topics.is_null() && topics.is_array())
            {
                for (auto i = 0; i < (int)data["topics"].size(); i++)
                {
                    std::string title = std::string(data["topics"][i]["title"]);

                    std::string text = std::string(data["topics"][i]["text"]);

                    Topics::ALL.push_back(Topics::Base(title.c_str(), text.c_str()));
                }
            }
        }
    }
}
#endif