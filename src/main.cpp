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
        {Operations::PLUS, 5},  {Operations::MINUS, 5}
    };

    GeneratorDataProvider<9, 25> dataprovider(weights);
    Generator<dataprovider.args> sdfs;
    sdfs.init(dataprovider.get_gendata());
    //sdfs.next();

    printf("Operation type: %d\nSome weight: %d", static_cast<unsigned>(opers), weights[opers]);
    return 0;
}
