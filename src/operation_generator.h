//
// Created by novasurfer on 2/11/20.
//

#ifndef MATHOPERATIONS_OPERATION_GENERATOR_H
#define MATHOPERATIONS_OPERATION_GENERATOR_H

#include <cmath>
#include <cstdint>
#include <unordered_map>

namespace mathgens
{

    namespace utils
    {
        inline static bool is_perfect_square(unsigned n)
        {
            const double root = sqrt(n);
            return ((root - floor(root)) == 0.0);
        }
    }

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

        /*
         * Returns range size [min, max]
         */
        constexpr unsigned size()
        {
            return (max - min) + 1;
        }

        /*
         * Returns number of perfect squares between [min, max]
         */
        constexpr unsigned squares_size(size_t sqrts_size)
        {
            for(size_t i = MIN; i <= MAX; ++i) {
                for(size_t j = 1; j * j <= i; ++j) {
                    if(j * j == i)
                        sqrts_size++;
                }
            }
            return sqrts_size;
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

    template <size_t N, size_t N_SQRTS>
    class Generator
    {
    public:
        void init(const GeneratorData& gendata);
        unsigned get_next_num();
        OperationData get_next_operation();
        GeneratorResult next();

    private:
        void split_numbers();

        GeneratorData gendata;
        unsigned current_value = 0;
        size_t id = 0;
        unsigned numbs_array[N];
        unsigned numbs_sqrts_array[N_SQRTS];
    };

    template <size_t N, size_t N_SQRTS>
    void Generator<N, N_SQRTS>::init(const GeneratorData& gendata)
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

        split_numbers();
    }

    template <size_t N, size_t N_SQRTS>
    unsigned Generator<N, N_SQRTS>::get_next_num()
    {
        unsigned result = numbs_array[id % N];
        id++;
        return result;
    }

    template <size_t N, size_t N_SQRTS>
    OperationData Generator<N, N_SQRTS>::get_next_operation()
    {
        return OperationData();
    }

    template <size_t N, size_t N_SQRTS>
    GeneratorResult Generator<N, N_SQRTS>::next()
    {
        unsigned result = get_next_num();
    }

    template <size_t N, size_t N_SQRTS>
    void Generator<N, N_SQRTS>::split_numbers()
    {
        // Split perfect squares
        {
            size_t i = 0;
            for(size_t j = 0; j < N; ++j) {
                unsigned value = numbs_array[j];
                if(utils::is_perfect_square(value)) {
                    numbs_sqrts_array[i] = value;
                    ++i;
                }
            }
        }
        // TODO: Split numbers with power of 2
        // TODO: Split other arrays
    }
}
#endif // MATHOPERATIONS_OPERATION_GENERATOR_H
