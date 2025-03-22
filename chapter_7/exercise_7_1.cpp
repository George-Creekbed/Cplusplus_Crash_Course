#include <cstdio>
#include <cstdint>
#include <limits>
#include <cassert>

class UnsignedBigInteger {
public:
    constexpr static unsigned long long two_fifty_five{0xffULL}; // 0xff = 0d00000255 = 2^(8) - 1
    constexpr static unsigned long long max_ull{std::numeric_limits<unsigned long long int>::max()}; 
    constexpr static unsigned long long two_fifty_six{0x100ULL};

    UnsignedBigInteger() = default;
    UnsignedBigInteger(const unsigned long long value) {
        //printf("value = %llu\n", value);
        unsigned long long shift{two_fifty_five};
        for (size_t i=0; i!=len; ++i) {
            _value[i] = (value & shift) >> (8 * i);
            //printf("_value[%lu]: %llb - shift: %llb\n", i, _value[i], shift);
            shift <<= 8;
        };
        // which is equivalent to:
        // _value[0] = value & 0xff; // 0xff = 0d256 = 2^(8) 
        // _value[1] = value & (0xff << 8);
        // _value[2] = value & (0xff << 16); 
        // ... _value[6] = value & (0xff << 56);
    }

    operator unsigned long long int() const {
        unsigned long long int result{};
        for (size_t i=0; i!=len; ++i) {
            result += static_cast<unsigned>(_value[i]) << (8*i); //static_cast to int works as well
            //printf("get result[%lu] = %llu, or in binary %llb\n", i, result, result);
        }

        return result;
    }

    UnsignedBigInteger operator+(const UnsignedBigInteger& addend) const {
        //printf("addiction: %llu + %llu\n", *this, addend);

        UnsignedBigInteger result{};
        
        for (size_t i=0; i!=len; ++i)
            result._value[i] = _value[i] + addend._value[i];

        //printf("addiction result: %llu\n", result);

        return result;      
    }

    UnsignedBigInteger operator-(const UnsignedBigInteger& addend) const {
        //printf("subtraction: %llu - %llu\n", *this, addend);

        UnsignedBigInteger result{};
        
        for (size_t i=0; i!=len; ++i) {
            result._value[i] = (_value[i] - addend._value[i]) % two_fifty_six;
            *this < addend && result._value[i] == 0 ? result._value[i] = two_fifty_five : true;
            printf("subtraction result, buckets: _value[%lu]: %hhb\n", i, result._value[i]);
        }

        return result; 
    }
    // operator*
    // operator/
    // operator%
    // operator int
private:
    constexpr static size_t len = 7;
    uint8_t _value[len];
    //char _value[len];
};


int main() {
    constexpr static unsigned long long max_ull{std::numeric_limits<unsigned long long int>::max()}; 
    assert(max_ull == 18'446'744'073'709'551'615ULL);  // = 2^64 - 1 = billions of billions
    printf("ULLONG_MAX is set for 64bit arithmetics: %llu - binary: %llb\n", max_ull, max_ull);

    UnsignedBigInteger a{13ULL}, b{27ULL};
    //UnsignedBigInteger a{255ULL}, b{0ULL};
    printf("a + b = %llu\na - b = %llu\n", a + b, a - b);
    printf("In binary notation:\n");
    printf("a - b = %llb - %llb = %llb\n", a, b, a - b);

    unsigned int five = 5, seven = 7, zero = 0;
    unsigned int aa = five - seven; 
    printf("uint aa = 5 - 7 = %u\n", aa); 
    unsigned int ab = zero % seven; 
    printf("uint ab = 0 % 7 = %u\n", ab);
    printf("a = %llu\n", a); 
}