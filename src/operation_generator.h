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

    template <size_t MIN, size_t MAX>
    struct GeneratorRange
    {
        static constexpr unsigned min = MIN;
        static constexpr unsigned max = MAX;

        constexpr unsigned size()
        {
            return max - min;
        }
    };

    struct GeneratorData
    {
        Operations operations;
        OperWeightMap weights;
        unsigned max_value;
        unsigned min_value;
    };

    struct Answers
    {
        Operations operation;
        unsigned number;
    };

    struct GeneratorResult
    {
        // For now hardcoded to 3;
        Answers answers[3];
        unsigned result;
    };

    template <size_t N>
    class Generator
    {
    public:
        void init(const GeneratorData& gendata);
        unsigned get_next_num();
        OperationData get_next_operation();
        GeneratorResult next();

    private:
        GeneratorData gendata;
        unsigned current_value = 0;
        size_t id = 0;
        int numbs_array[N];
    };

    template <size_t N>
    void Generator<N>::init(const GeneratorData& gendata)
    {
        this->gendata = gendata;

        if constexpr(N < 1)
            return;

        // Fill array
        unsigned min = gendata.min_value;
        for(size_t i = 0; i < N; ++i) {
            numbs_array[i] = min++;
        }

        // Shuffle array
        srand(time(nullptr));
        for(size_t i = 0; i < N - 1; ++i) {
            size_t j = i + rand() / (RAND_MAX / (N - i) + 1);
            int t = numbs_array[j];
            numbs_array[j] = numbs_array[i];
            numbs_array[i] = t;
        }
    }

    template <size_t N>
    unsigned Generator<N>::get_next_num()
    {
        unsigned result = numbs_array[id % N];
        id++;
        return result;
    }

    template <size_t N>
    OperationData Generator<N>::get_next_operation()
    {
        return OperationData();
    }

    template <size_t N>
    GeneratorResult Generator<N>::next()
    {
        unsigned result = get_next_num();

    }
}
#endif // MATHOPERATIONS_OPERATION_GENERATOR_H
