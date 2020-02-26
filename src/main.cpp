#include "operation_generator.h"
#include <iostream>

int main()
{
    using namespace mathcore;

    // some testing..
    // TODO: add doctest for unit-testing
    Operations opers = Operations::MINUS | Operations::PLUS;
    //    opers &= ~Operations::PLUS;

    OperWeightMap weights {{Operations::PLUS, 5}, {Operations::MINUS, 5}};

    GeneratorDataProvider<9, 25> dataprovider(weights);
    Generator<dataprovider.args> sdfs;
    sdfs.init(dataprovider.get_gendata());
   // sdfs.next();

	size_t i = 0;
    while(true) {
       GenRes result = sdfs.next();
        printf("%d) Result: %d, correct: %d, wrong: %d, wrong: %d\n", i, result.result, result.answers[0].number, result.answers[1].number, result.answers[2].number);
        ++i;
	}
    printf("Operation type: %d\nSome weight: %d", static_cast<unsigned>(opers), weights[opers]);
    return 0;
}
