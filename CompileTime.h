//+------------------------------------------------------------------+
//|                                              Compile time Random |
//+------------------------------------------------------------------+
#pragma once
#define IntToString(x) #x
template< size_t N >
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
constexpr size_t length(char const (&)[N])
  {
   return N-1;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
constexpr uint64_t fnv1impl(uint64_t h, const char* s)
  {
   return(*s==0) ? h :
   fnv1impl((h * 1099511628211ull) ^ static_cast<uint64_t>(*s), s+1);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
constexpr uint64_t fnv1(const char* s)
  {
   return fnv1impl(14695981039346656037ull, s);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
template <class T=uint64_t, uint32_t Bits=7>
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
constexpr T pack(const char *s, uint32_t off=0)
  {
   return(Bits*off>=CHAR_BIT*sizeof(T) || !s[off]) ? 0 :
   (((T)s[off] << (Bits*off)) | pack(s,off+1));
  }

template<int ...> struct Sequence { };
template<int N, int ...S> struct CreateSequence : CreateSequence<N-1, N-1, S...> { };
template<int ...S> struct CreateSequence<0, S...> { typedef Sequence<S...> Type; };

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
constexpr uint32_t UpdateHash1(uint32_t k)
  {
   return k * 0xcc9e2d51;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
constexpr uint32_t UpdateHash2(uint32_t k)
  {
   return(k << 15) | (k >> (32 - 15));
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
constexpr uint32_t UpdateHash3(uint32_t k)
  {
   return k * 0x1b873593;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
constexpr uint32_t UpdateHash4(uint32_t hash, uint32_t block)
  {
   return hash ^ block;
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
constexpr uint32_t UpdateHash5(uint32_t hash)
  {
   return((hash << 13) | (hash >> (32 - 13))) * 5 + 0xe6546b64;
  }

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
constexpr uint32_t UpdateHash(uint32_t hash, uint32_t block)
  {
   return UpdateHash5(UpdateHash4(hash, UpdateHash3(UpdateHash2(UpdateHash1(block)))));
  }

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
constexpr uint32_t UpdateLastHash(uint32_t hash, uint32_t block)
  {
   return UpdateHash4(hash, UpdateHash3(UpdateHash2(UpdateHash1(block))));
  }

template<typename... C> constexpr uint32_t CalculateHashRounds(uint32_t seed, C... c);

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
template<> constexpr uint32_t CalculateHashRounds(uint32_t seed)
  {
   return seed;
  }

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
template<> constexpr uint32_t CalculateHashRounds(uint32_t seed, char c0)
  {
   return UpdateLastHash(seed, uint8_t(c0));
  }

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
template<> constexpr uint32_t CalculateHashRounds(uint32_t seed, char c0, char c1)
  {
   return UpdateLastHash(seed, uint8_t(c0) | uint8_t(c1)<<8);
  }

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
template<> constexpr uint32_t CalculateHashRounds(uint32_t seed, char c0, char c1, char c2)
  {
   return UpdateLastHash(seed, uint8_t(c0) | uint8_t(c1)<<8 | uint8_t(c2)<<16);
  }

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
template<typename... C> constexpr uint32_t CalculateHashRounds(uint32_t seed, char c0, char c1, char c2, char c3, C... c)
  {
   return CalculateHashRounds(UpdateHash(seed, uint8_t(c0) | uint8_t(c1)<<8 | uint8_t(c2)<<16 | uint8_t(c3)<<24), c...);
  }

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
constexpr uint32_t CalculateFinalHash1(uint32_t h, uint32_t length)
  {
   return h ^ length;
  }

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
constexpr uint32_t CalculateFinalHash2(uint32_t h)
  {
   return h ^ (h >> 16);
  }

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
constexpr uint32_t CalculateFinalHash3(uint32_t h)
  {
   return h * 0x85ebca6b;
  }

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
constexpr uint32_t CalculateFinalHash4(uint32_t h)
  {
   return h ^ (h >> 13);
  }

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
constexpr uint32_t CalculateFinalHash5(uint32_t h)
  {
   return h * 0xc2b2ae35;
  }

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
constexpr uint32_t CalculateFinalHash6(uint32_t h)
  {
   return h ^ (h >> 16);
  }

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
constexpr uint32_t CalculateFinalHash(uint32_t h, uint32_t length)
  {
   return CalculateFinalHash6(
       CalculateFinalHash5(
       CalculateFinalHash4(
       CalculateFinalHash3(
       CalculateFinalHash2(
       CalculateFinalHash1(h, length))))));
  }

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
template<int N, int ...S> constexpr uint32_t Unpack(unsigned seed, const char (&s)[N], Sequence<S...>)
  {
   return CalculateHashRounds(seed, s[S]...);
  }

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
template<int N> constexpr uint32_t MURMUR3_32(const char (&s)[N], uint32_t seed=0)
  {
   return CalculateFinalHash(Unpack(seed, s, typename CreateSequence<N-1>::Type()), N-1);
  }
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
template<uint32_t S, uint32_t A=16807UL, uint32_t C=0UL, uint32_t M=(1UL<<31)-1>
  struct LinearGenerator {
   static const uint32_t state=((uint32_t)S * A + C)%M;
   static const uint32_t value=state;
   typedef LinearGenerator<state> next;
     struct Split {
      typedef LinearGenerator< state, A*A, 0, M> Gen1;
      typedef LinearGenerator<next::state, A*A, 0, M> Gen2;
     };
  };
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
template <uint64_t n> struct CompileTimeValue
  {
   enum : uint64_t
     {
      value=n
     };
  };

//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
#define SEED fnv1(__TIME__)
#define RNGSEED MURMUR3_32(__FILE__ __DATE__ __TIME__)
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
template<uint32_t N>
constexpr uint32_t Random(const uint64_t num=LinearGenerator<(N+RNGSEED),
	                      MURMUR3_32(IntToString(N+SEED),N)>::value,
                          const uint64_t num2=fnv1(IntToString(LinearGenerator<num>::value)))
{
 return uint32_t(num+num2);
}
//+------------------------------------------------------------------+
//|                                                                  |
//+------------------------------------------------------------------+
template<uint32_t N>
constexpr uint64_t Random64(const uint64_t num=LinearGenerator<(N + RNGSEED),
	                       MURMUR3_32(IntToString(N + SEED),N)>::value,
	                       const uint64_t num2=fnv1(IntToString(LinearGenerator<num>::value)))
{
 return (num*num2);
}

#define rValue                    uint32_t(CompileTimeValue<Random<__COUNTER__+1>()>::value)
#define rValue64                  uint64_t(CompileTimeValue<Random64<__COUNTER__+1>()>::value)