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

        inline static constexpr bool is_power_of_two(unsigned n)
        {
            return (n != 0) && ((n & (n - 1)) == 0);
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

    constexpr unsigned OPERATIONS_SIZE = 8;

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
        constexpr size_t squares_size(size_t sqrts_size) const
        {
            for(size_t i = MIN; i <= MAX; ++i) {
                for(size_t j = 1; j * j <= i; ++j) {
                    if(j * j == i)
                        sqrts_size++;
                }
            }
            return sqrts_size;
        }

        constexpr size_t powsof2_size(size_t powsof2_size) const
        {
            for(size_t i = MIN; i <= MAX; ++i) {
                if(utils::is_power_of_two(i))
                    powsof2_size++;
            }
            return powsof2_size;
        }
    };

    struct OperationData
    {
        Operations type;
        unsigned number;
    };

    // Struct for pair [operation, weight]
    using OperationPair = OperationData;

    struct GeneratorData
    {
        OperWeightMap weights;
        unsigned max_value;
        unsigned min_value;
    };

    struct GenRes
    {
        static constexpr size_t CORRECT_INDEX = 0;
        // For now hardcoded to 3;
        OperationData answers[3];
        unsigned result;
    };

    enum gen_data_args
    {
        N,
        N_SQRTS,
        N_POWS2,
        THE_END
    };

    template <const size_t* SIZE>
    class Generator
    {
        using arg = gen_data_args;

    public:
        void init(const GeneratorData& gendata);
        unsigned get_next_num();
        OperationData get_next_operation();
        GenRes next();

    private:
        void split_numbers();
        Operations pick_operation();

        GeneratorData gendata;
        unsigned current_value = 0;
        size_t id = 0;
        unsigned numbs_array[SIZE[arg::N]];
        unsigned numbs_sqrts_array[SIZE[arg::N_SQRTS]];
        unsigned numbs_pow2_array[SIZE[arg::N_POWS2]];
    };

    template <const size_t* SIZE>
    void Generator<SIZE>::init(const GeneratorData& gendata)
    {
        this->gendata = gendata;
        constexpr size_t lenght = SIZE[arg::N];

        if constexpr(lenght < 1)
            return;

        // Fill array
        unsigned min = gendata.min_value;
        for(size_t i = 0; i < SIZE[arg::N]; ++i) {
            numbs_array[i] = min++;
        }

        // Shuffle array
        srand(time(nullptr));
        for(size_t i = 0; i < lenght - 1; ++i) {
            size_t j = i + rand() / (RAND_MAX / (lenght - i) + 1);
            int t = numbs_array[j];
            numbs_array[j] = numbs_array[i];
            numbs_array[i] = t;
        }

        split_numbers();
    }

    template <const size_t* SIZE>
    unsigned Generator<SIZE>::get_next_num()
    {
        unsigned result = numbs_array[id % SIZE[arg::N]];
        id++;
        return result;
    }

    template <const size_t* SIZE>
    OperationData Generator<SIZE>::get_next_operation()
    {
        return OperationData();
    }

    template <const size_t* SIZE>
    GenRes Generator<SIZE>::next()
    {
        GenRes genres;
        static size_t regular_id;
        static size_t sqrts_id;
        static size_t pows2_id;

        Operations oper = pick_operation();

        switch(oper) {
        case Operations::PLUS:
        case Operations::MINUS:
        case Operations::MULTIPLY:
        case Operations::DIVIDE:
            genres.result = numbs_array[regular_id % SIZE[arg::N]];
            break;
        case Operations::POW:
            genres.result = numbs_pow2_array[regular_id % SIZE[arg::N_POWS2]];
            break;
        case Operations::SQRT:
            genres.result = numbs_sqrts_array[regular_id % SIZE[arg::N_SQRTS]];
            break;
        case Operations::MOD:
            break;
        case Operations::LOG:
            break;
        }
    }

    template <const size_t* SIZE>
    void Generator<SIZE>::split_numbers()
    {
        constexpr size_t length = SIZE[arg::N];
        // Split perfect squares
        {
            size_t i = 0;
            for(size_t j = 0; j < length; ++j) {
                unsigned value = numbs_array[j];
                if(utils::is_perfect_square(value)) {
                    numbs_sqrts_array[i] = value;
                    ++i;
                }
            }
        }
        // Split numbers that are power of 2
        {
            size_t i = 0;
            for(size_t j = 0; j < length; ++j) {
                unsigned value = numbs_array[j];
                if(utils::is_power_of_two(value)) {
                    numbs_pow2_array[i] = value;
                    ++i;
                }
            }
        }
        // TODO: Split other arrays
    }
    template <const size_t* SIZE>
    Operations Generator<SIZE>::pick_operation()
    {
        size_t sum_of_weight = 0;

        for(const auto [key, val] : gendata.weights) {
            sum_of_weight += val;
        }

        srand(time(nullptr));
        size_t rnd = rand() % sum_of_weight;
        for(const auto [key, val] : gendata.weights) {
            if(rnd < val)
                return key;
            rnd -= val;
        }
    }
}
#endif // MATHOPERATIONS_OPERATION_GENERATOR_H
