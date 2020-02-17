#include "operation_generator.h"
#include <iostream>

int main()
{
    using namespace mathgens;

    // some testing..
    // TODO: add doctest for unit-testing
    Operations opers = Operations::MINUS | Operations::PLUS;
//    opers &= ~Operations::PLUS;

    OperWeightMap weights {
        {Operations::PLUS, 10},  {Operations::MINUS, 9}, {Operations::MULTIPLY, 8},
        {Operations::DIVIDE, 7}, {Operations::POW, 6},   {Operations::SQRT, 5},
        {Operations::MOD, 4},    {Operations::LOG, 3},   {opers, 88}};



    GeneratorData gendata_lvl1;
    GeneratorRange<9,25> range;
    gendata_lvl1.min_value = range.min;
    gendata_lvl1.max_value = range.max;
    const int sqrt_size_buff = 0;
    constexpr size_t rng_size = range.size();
    constexpr size_t sqrs_size = range.squares_size(sqrt_size_buff);
    Generator<rng_size, sqrs_size> generator_lvl1;
    generator_lvl1.init(gendata_lvl1);

//    GeneratorData gendata_lvl2;
//    GeneratorRange<64,128> range2;
//    gendata_lvl2.min_value = range2.min;
//    gendata_lvl2.max_value = range2.max;
//    Generator<range2.size()> generator_lvl2;
//    generator_lvl2.init(gendata_lvl2);

    printf("Operation type: %d\nSome weight: %d", static_cast<unsigned>(opers), weights[opers]);
    return 0;
}
