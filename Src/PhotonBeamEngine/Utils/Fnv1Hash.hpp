#ifndef Fnv1Hash_h
#define Fnv1Hash_h

namespace Pht {
    namespace Hash {
        namespace Detail {
            constexpr uint32_t defaultOffsetBasis {0x811C9DC5};
            constexpr uint32_t prime {0x01000193};
        }
        
        // This should generate a compile-time FNV1a hash if called with a constexpr string.
        constexpr static inline uint32_t Fnv1a(char const* const str,
                                               const uint32_t val = Detail::defaultOffsetBasis) {
            return (str[0] == '\0') ? val : Fnv1a(&str[1], (val ^ uint32_t(str[0])) * Detail::prime);
        }
    }
}

#endif
