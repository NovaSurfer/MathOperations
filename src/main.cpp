#include "operation_generator.h"
#include <iostream>

int main()
{
    using namespace mathcore;

    // some testing..
    // TODO: add doctest for unit-testing
    Operations opers = Operations::MINUS | Operations::PLUS;
    OperWeightMap weights {{Operations::PLUS, 5}, {Operations::MINUS, 5}};

    GeneratorDataProvider<9, 25> dataprovider(weights);
    Generator<dataprovider.get_args()> sdfs;
    sdfs.init(dataprovider.get_gendata());

	size_t i = 0;
    while(true) {
       GenRes result = sdfs.next();
       printf("%d) Result: %d, correct: %d, wrong: %d, wrong: %d\n", i, result.result, result.answers[0].number, result.answers[1].number, result.answers[2].number);
       ++i;
	}

    return 0;
}
