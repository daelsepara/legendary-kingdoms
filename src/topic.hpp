#ifndef __TOPIC__HPP__
#define __TOPIC__HPP__

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

// JSON library
#include "nlohmann/json.hpp"

#include "book.hpp"

namespace Topics
{
    class Base
    {
    public:
        std::string Title = "";
        std::string Text = "";
        std::string Image = "";

        Base()
        {
        }

        Base(const char *title, const char *text)
        {
            Title = std::string(title);

            if (text)
            {
                Text = std::string(text);
            }
        }

        Base(const char *title, const char *text, const char *image)
        {
            Title = std::string(title);

            if (text)
            {
                Text = std::string(text);
            }

            if (image)
            {
                Image = std::string(image);
            }
        }
    };

    std::vector<Topics::Base> ALL = {};

    void LoadTopics(Book::Type book)
    {
        Topics::ALL.clear();

        std::string topic_file = "";

        if (book == Book::Type::BOOK1)
        {
            topic_file = "topics/book1.json";
        }

        std::ifstream ifs(topic_file);

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

                    std::string text = "";

                    std::string image = "";

                    if (!data["topics"][i]["text"].is_null())
                    {
                        text = std::string(data["topics"][i]["text"]);
                    }

                    if (!data["topics"][i]["image"].is_null())
                    {
                        image = std::string(data["topics"][i]["image"]);
                    }

                    if (text.length() > 0 && image.length() > 0)
                    {
                        Topics::ALL.push_back(Topics::Base(title.c_str(), text.c_str(), image.c_str()));
                    }
                    else if (text.length() > 0)
                    {
                        Topics::ALL.push_back(Topics::Base(title.c_str(), text.c_str()));
                    }
                    else if (image.length() > 0)
                    {
                        Topics::ALL.push_back(Topics::Base(title.c_str(), NULL, image.c_str()));
                    }
                }
            }
        }
    }
}
#endif