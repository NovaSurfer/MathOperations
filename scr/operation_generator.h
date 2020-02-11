//
// Created by novasurfer on 2/11/20.
//

#ifndef MATHOPERATIONS_OPERATION_GENERATOR_H
#define MATHOPERATIONS_OPERATION_GENERATOR_H

#include <cstdint>
#include <unordered_map>

namespace mathgens
{

    enum class Operations : unsigned
    {
        NONE = 0,
        PLUS = (1u << 0u),
        MINUS = (1u << 1u),
        MULTIPLY = (1u << 2u),
        DIVIDE = (1u << 3u),
        POW = (1u << 4u),
        SQRT = (1u << 5u),
        MOD = (1u << 6u),
        LOG = (1u << 7u)
    };

    using OperWeightMap = std::unordered_map<Operations, unsigned>;

    // This boilerplate code can be reduced with use of template + macro, the following method is
    // described here: http://blog.bitwigglers.org/using-enum-classes-as-type-safe-bitmasks/
    // but for now I want to keep things more obvious
    inline Operations operator|(Operations l, Operations r)
    {
        return static_cast<Operations>(static_cast<unsigned>(l) | static_cast<unsigned>(r));
    }

    inline Operations operator^(Operations l, Operations r)
    {
        return static_cast<Operations>(static_cast<unsigned>(l) ^ static_cast<unsigned>(r));
    }

    inline Operations operator&(Operations l, Operations r)
    {
        return static_cast<Operations>(static_cast<unsigned>(l) & static_cast<unsigned>(r));
    }

    inline Operations& operator|=(Operations& l, Operations r)
    {
        l = static_cast<Operations>(static_cast<unsigned>(l) | static_cast<unsigned>(r));
        return l;
    }

    inline Operations& operator^=(Operations& l, Operations r)
    {
        l = static_cast<Operations>(static_cast<unsigned>(l) ^ static_cast<unsigned>(r));
        return l;
    }

    inline Operations& operator&=(Operations& l, Operations r)
    {
        l = static_cast<Operations>(static_cast<unsigned>(l) & static_cast<unsigned>(r));
        return l;
    }

    inline Operations operator~(Operations oper)
    {
        return static_cast<Operations>(~static_cast<unsigned>(oper));
    }

    struct OperationData
    {
        Operations type;
        unsigned number;
    };

    struct GeneratorData
    {
        Operations operations;
        OperWeightMap weights;
        unsigned steps_count;
        unsigned max_value;
        unsigned min_value;
    };

    class Generator
    {
    public:
        void set_gendata(const GeneratorData& gendata)
        {
            this->gendata = gendata;
        }
        unsigned get_next_num();
        OperationData get_next_operation();

    private:
        GeneratorData gendata;
        unsigned number = 0;
    };

}
#endif // MATHOPERATIONS_OPERATION_GENERATOR_H
