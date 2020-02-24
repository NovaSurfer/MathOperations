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

    GeneratorRange<9, 25> range;
    OperationPair op_pair[OPERATIONS_SIZE] {
        {Operations::PLUS, 10},  {Operations::MINUS, 9}, {Operations::MULTIPLY, 8},
        {Operations::DIVIDE, 7}, {Operations::POW, 6},   {Operations::SQRT, 5},
        {Operations::MOD, 4},    {Operations::LOG, 3},
    };

    GeneratorData gendata_lvl1;
    gendata_lvl1.min_value = range.min;
    gendata_lvl1.max_value = range.max;
    gendata_lvl1.weights = weights;
    const int sqrt_size_buff = 0;
    const int pows2_size_buff = 0;
    constexpr size_t rng_size = range.size();
    constexpr size_t sqrs_size = range.squares_size(sqrt_size_buff);
    constexpr size_t pows2_size = range.powsof2_size(pows2_size_buff);
//    constexpr size_t oper_pairs_size = gendata_lvl1.oper_pairs_size();
    constexpr static size_t lvl1_args[3] = {rng_size, sqrs_size, pows2_size};
    Generator<lvl1_args> sdfs;
    sdfs.init(gendata_lvl1);

    printf("Operation type: %d\nSome weight: %d", static_cast<unsigned>(opers), weights[opers]);
    return 0;
}
