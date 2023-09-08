#include <cxxopts.hpp>
#include <fts/sum.h>
#include <iostream>

int sum(int argc, char** argv)
{
    cxxopts::options options("test", "A brief description");

  options.add_options()
    ("f,first", "First value", cxxopts::value<int>())
    ("s,second", "Second value", cxxopts::value<int>())
    ("h,help", "Print usage");

  auto result = options.parse(argc, argv);

  std::cout << result["first"].as<int>() + result["second"].as<int>() << std::endl;

    return 0;
}
