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

    printf("Operation type: %d\nSome weight: %d", static_cast<unsigned>(opers), weights[opers]);
    return 0;
}
