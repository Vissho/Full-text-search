#include <cxxopts.hpp>
#include <fmt/core.h>
#include <fts/parser.hpp>
#include <iostream>

int main(int argc, char** argv)
{
    cxxopts::Options options(
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
            sum(result["first"].as<double>(), result["second"].as<double>()));

    return 0;
}
