#include <iostream>
#include <libmath/sum.h>
#include <cxxopts/cxxopts.hpp>

int main(int argc, char** argv)
{
    sum(argv[1], argv[2]);
    return 0;
}
