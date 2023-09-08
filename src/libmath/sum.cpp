#define FMT_HEADER_ONLY
#include <core.h>
#include <cxxopts.hpp>
#include <fts/sum.h>
#include <iostream>

int sum(int argc, char** argv)
{
    cxxopts::options options(
            "Summation",
            "A simple program for summing two floating point numbers");

    options.add_options()("f,first", "First value", cxxopts::value<double>())(
            "s,second", "Second value", cxxopts::value<double>())(
            "h,help", "Print usage");

    auto result = options.parse(argc, argv);

    if (result.count("help") != 0) {
        fmt::print(options.help());
        fmt::print("\n");
        return (0);
    }

    if (result.count("first") == 0) {
        fmt::print(
                "You have to enter the first value. For more details, "
                "type --help.\n");
        return (-1);
    }

    if (result.count("second") == 0) {
        fmt::print(
                "You have to enter the second value. For more details, "
                "type --help.\n");
        return (-1);
    }

    fmt::print(
            "Your answer: {}\n",
            result["first"].as<double>() + result["second"].as<double>());

    return 0;
}
