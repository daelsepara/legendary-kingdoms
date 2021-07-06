// Standard IO
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

#include "engine.hpp"

int main(int argc, char **argv)
{
    int count = 1;

    auto random = Random();

    random.UniformIntDistribution(1, 6);

    if (argc > 1)
    {
        count = std::atoi(argv[1]);
    }

    auto results = Engine::ROLL_DICE(count);

    std::cerr << "Rolling " << count << " die." << std::endl;

    for (auto i = 0; i < results.size(); i++)
    {
        if (i > 0)
        {
            std::cerr << ", ";
        }

        std::cerr << results[i];
    }

    std::cerr << std::endl;

    return 0;
}