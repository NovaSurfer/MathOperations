//
// Created by novasurfer on 2/11/20.
//

#ifndef MATHOPERATIONS_OPERATION_GENERATOR_H
#define MATHOPERATIONS_OPERATION_GENERATOR_H

#include <cmath>
#include <time.h>
#include <unordered_map>

#define DLL_EXPORT __declspec(dllexport)

namespace mathcore
{

    namespace utils
    {
        using num_check_func = bool (*)(unsigned);

        inline static void split_numbs_by_type(num_check_func is_type,
                                               const unsigned* const src_array,
                                               unsigned* dest_array, size_t src_length)
        {
            size_t i = 0;
            for(size_t j = 0; j < src_length; ++j) {
                unsigned value = src_array[j];
                if(is_type(value)) {
                    dest_array[i] = value;
                    ++i;
                }
            }
        }

        inline static bool is_perfect_square(unsigned n)
        {
            const double root = sqrt(n);
            return ((root - floor(root)) == 0.0);
        }

        inline static constexpr bool is_power_of_two(unsigned n)
        {
            return (n != 0) && ((n & (n - 1)) == 0);
        }

        inline static constexpr bool is_prime(unsigned n)
        {
            if(n == 0 || n == 1)
                return false;

            for(unsigned i = 2; i < n; ++i)
                if(n % i == 0)
                    return false;

            return true;
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

    //using OperWeightMap = std::unordered_map<Operations, unsigned>;
    typedef DLL_EXPORT std::unordered_map<Operations, unsigned> OperWeightMap;

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
        static constexpr unsigned size()
        {
            return (max - min) + 1;
        }

        /*
         * Returns number of perfect squares between [min, max]
         */
        static constexpr size_t squares_size()
        {
            static size_t sqrts_size = 0;
            for(size_t i = MIN; i <= MAX; ++i) {
                for(size_t j = 1; j * j <= i; ++j) {
                    if(j * j == i)
                        ++sqrts_size;
                }
            }
            return sqrts_size;
        }

        static constexpr size_t powsof2_size()
        {
            static size_t powsof2_size = 0;
            for(size_t i = MIN; i <= MAX; ++i) {
                if(utils::is_power_of_two(i))
                    ++powsof2_size;
            }
            return powsof2_size;
        }

        static constexpr size_t primes_size()
        {
            static size_t primes_size = 0;
            for(size_t i = MIN; i <= MAX; ++i) {
                if(utils::is_prime(i))
                    ++primes_size;
            }
            return primes_size;
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
        unsigned min_value;
        unsigned max_value;
    };

    struct DLL_EXPORT GenRes
    {
        static constexpr size_t CORRECT_ID = 0;
        // For now hardcoded to 3;
        OperationData answers[3];
        unsigned result;
    };

    enum gen_data_args
    {
        N,
        N_SQRTS,
        N_POWS2,
        N_PRIMES,
        THE_END
    };

    template <const size_t* SIZE>
    class DLL_EXPORT Generator 
    {
        using arg = gen_data_args;

    public:
        //Generator();
        void init(const GeneratorData& gendata);
        OperationData get_next_operation();
        GenRes next();
        unsigned get_current_result() const
        {
            return current_result;
        }

        unsigned get_previous_result() const
        {
            return prev_result;
        }

    private:
        unsigned get_next_num();
        void split_numbers();
        void fill_incorrect_data(OperationData* answers);
        Operations pick_operation();

        GeneratorData gendata;
        unsigned current_result = 0;
        unsigned prev_result = 0;
        unsigned numbs_array[SIZE[arg::N]];
        unsigned numbs_sqrts_array[SIZE[arg::N_SQRTS]];
        unsigned numbs_pow2_array[SIZE[arg::N_POWS2]];
        unsigned numbs_non_prime_array[SIZE[arg::N] - SIZE[arg::N_PRIMES]];
    };

    template <size_t MIN, size_t MAX>
    class DLL_EXPORT GeneratorDataProvider
    {
    public:
        constexpr GeneratorDataProvider(const OperWeightMap& weights)
        {
            gendata.weights = weights;
            gendata.min_value = MIN;
            gendata.max_value = MAX;
        }

        static constexpr GeneratorRange<MIN, MAX> range {};
        static constexpr size_t args[gen_data_args::THE_END] {
            range.size(), range.squares_size(), range.powsof2_size(), range.primes_size()};

        const GeneratorData& get_gendata() const
        {
            return gendata;
        }

    private:
        GeneratorData gendata;
    };

    template <const size_t* SIZE>
    void Generator<SIZE>::init(const GeneratorData& gendata)
    {
        this->gendata = gendata;
        constexpr size_t length = SIZE[arg::N];

        if constexpr(length < 1)
            return;

        // Fill array
        unsigned min = gendata.min_value;
        for(size_t i = 0; i < length; ++i) {
            numbs_array[i] = min++;
        }

        // Shuffle array
        srand(time(nullptr));
        size_t k = 0;
        for(size_t i = 0; i < length; ++i) {
            size_t j = i + rand() / (RAND_MAX / (length - i) + 1);
            int t = numbs_array[j];
            numbs_array[j] = numbs_array[i];
            numbs_array[i] = t;
            // Fill non-prime array
            if(!utils::is_prime(numbs_array[i])) {
                numbs_non_prime_array[k] = numbs_array[i];
                ++k;
            }
        }

        split_numbers();
        current_result = get_next_num();
    }

    template <const size_t* SIZE>
    unsigned Generator<SIZE>::get_next_num()
    {
        static size_t id;
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
        // Index 0 is used for the first 'current_result'
        static size_t regular_id = 1;
        static size_t sqrts_id;
        static size_t pows2_id;
        unsigned correct_number = 0;
        GenRes genres;

        Operations oper = pick_operation();

        switch(oper) {
        case Operations::PLUS:
            genres.result = numbs_array[regular_id % SIZE[arg::N]];
            if(genres.result < current_result) {
                genres.answers[GenRes::CORRECT_ID].type = Operations::MINUS;
                correct_number = current_result - genres.result;
            } else {
                genres.answers[GenRes::CORRECT_ID].type = Operations::PLUS;
                correct_number = genres.result - current_result;
            }
            genres.answers[GenRes::CORRECT_ID].number = correct_number;
            prev_result = current_result;
            current_result = genres.result;
            fill_incorrect_data(genres.answers);
            ++regular_id;
            break;
        case Operations::MINUS:
            genres.result = numbs_array[regular_id % SIZE[arg::N]];
            if(genres.result > current_result) {
                genres.answers[GenRes::CORRECT_ID].type = Operations::PLUS;
                correct_number = genres.result - current_result;
            } else {
                genres.answers[GenRes::CORRECT_ID].type = Operations::MINUS;
                correct_number = current_result - genres.result;
            }
            genres.answers[GenRes::CORRECT_ID].number = correct_number;
            prev_result = current_result;
            current_result = genres.result;
            fill_incorrect_data(genres.answers);
            ++regular_id;
            break;
            //        case Operations::MULTIPLY:
            //
            //            break;
            //        case Operations::DIVIDE:
            //            genres.result = numbs_array[regular_id % SIZE[arg::N]];
            //            break;
            //        case Operations::POW:
            //            genres.result = numbs_pow2_array[regular_id % SIZE[arg::N_POWS2]];
            //            break;
            //        case Operations::SQRT:
            //            genres.result = numbs_sqrts_array[regular_id % SIZE[arg::N_SQRTS]];
            //            break;
            //        case Operations::MOD:
            //            break;
            //        case Operations::LOG:
            //            break;
        }
        return genres;
    }

    // TODO: replace hard-coded values
    template <const size_t* SIZE>
    void Generator<SIZE>::fill_incorrect_data(OperationData* answers)
    {
        unsigned correct_answer = answers[GenRes::CORRECT_ID].number;
        unsigned incorrect_answers[3];
        size_t j = 0;

        for(size_t i = correct_answer + 1; i <= correct_answer + 3; ++i, ++j) {
            incorrect_answers[j] = i;
        }

        // Shuffle array
        // TODO: Replace copy-pasting
        srand(time(nullptr));
        for(size_t i = 0; i < 3; ++i) {
            size_t k = i + rand() / (RAND_MAX / (3 - i) + 1);
            int t = incorrect_answers[k];
            incorrect_answers[k] = incorrect_answers[i];
            incorrect_answers[i] = t;
        }

        for(size_t i = 0; i < 3; ++i) {
            if(i != GenRes::CORRECT_ID) {
                unsigned value = incorrect_answers[i];
                answers[i].number = value;
                // TODO: redo
                answers[i].type = pick_operation();
            }
        }
    }

    template <const size_t* SIZE>
    void Generator<SIZE>::split_numbers()
    {
        constexpr size_t length = SIZE[arg::N];
        utils::split_numbs_by_type(utils::is_perfect_square, numbs_non_prime_array,
                                   numbs_sqrts_array, length);
        utils::split_numbs_by_type(utils::is_power_of_two, numbs_non_prime_array, numbs_pow2_array,
                                   length);

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
