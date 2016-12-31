#ifndef CHAOS_MACROS_HH
#define CHAOS_MACROS_HH

#include <inttypes.h>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace chaos { //::chaos ////////////////////////////////////////////////////

// double from signed

#define CHAOS_DOUBLE_S08(x) ((double)((int8_t)x) / (double)INT8_MAX)
#define CHAOS_DOUBLE_S16(x) ((double)((int16_t)x) / (double)INT16_MAX)
#define CHAOS_DOUBLE_S32(x) ((double)((int32_t)x) / (double)INT32_MAX)
#define CHAOS_DOUBLE_S64(x) ((double)((int64_t)x) / (double)INT64_MAX)

// double from unsigned

#define CHAOS_DOUBLE_U08(x) ((double)((uint8_t)x) / (double)UINT8_MAX)
#define CHAOS_DOUBLE_U16(x) ((double)((uint16_t)x) / (double)UINT16_MAX)
#define CHAOS_DOUBLE_U32(x) ((double)((uint32_t)x) / (double)UINT32_MAX)
#define CHAOS_DOUBLE_U64(x) ((double)((uint64_t)x) / (double)UINT64_MAX)

// rotation: left

#define CHAOS_RO_L08(n, r)                                                     \
	((uint8_t)((uint8_t)(n) << (r)) | (uint8_t)((uint8_t)(n) >> (8 - (r))))
#define CHAOS_RO_L16(n, r)                                                     \
	((uint16_t)((uint16_t)(n) << (r)) | (uint16_t)((uint16_t)(n) >> (16 - (r))))
#define CHAOS_RO_L32(n, r)                                                     \
	((uint32_t)((uint32_t)(n) << (r)) | (uint32_t)((uint32_t)(n) >> (32 - (r))))
#define CHAOS_RO_L64(n, r)                                                     \
	((uint64_t)((uint64_t)(n) << (r)) | (uint64_t)((uint64_t)(n) >> (64 - (r))))

// rotation: right

#define CHAOS_RO_R08(n, r)                                                     \
	((uint8_t)((uint8_t)(n) >> (r)) | (uint8_t)((uint8_t)(n) << (8 - (r))))
#define CHAOS_RO_R16(n, r)                                                     \
	((uint16_t)((uint16_t)(n) >> (r)) | (uint16_t)((uint16_t)(n) << (16 - (r))))
#define CHAOS_RO_R32(n, r)                                                     \
	((uint32_t)((uint32_t)(n) >> (r)) | (uint32_t)((uint32_t)(n) << (32 - (r))))
#define CHAOS_RO_R64(n, r)                                                     \
	((uint64_t)((uint64_t)(n) >> (r)) | (uint64_t)((uint64_t)(n) << (64 - (r))))

// string to chars

#define CHAOS_STRING_TO_CHAR(str)                                              \
	({                                                                           \
		char *str_copy = new char[str.size() + 1];                                 \
		std::copy(str.begin(), str.end(), str_copy);                               \
		str_copy[str.size()] = '\0';                                               \
		str_copy;                                                                  \
	})

// register prng

#define CHAOS_GENERATOR_REGISTER(a, b) typedef chaos::prng<b> CHAOS_PRNG_##a;

// register engine

#define CHAOS_ENGINE_REGISTER(a, b) typedef chaos::machine<b> CHAOS_MACHINE_##a;

// utils for meta

#define CHAOS_META_DEFINE(a, b) std::string name = a, authors = b;

#define CHAOS_META_NAME(module) ({ module inst; inst.name; })
#define CHAOS_META_AUTHORS(module) ({ module inst; inst.authors; })

/// special ////////////////////////////////////////////////////////////////////

#define __chaos_note(S, ...)                                                   \
	fprintf(stderr,                                                              \
	        "\x1b[1m(%s:%d, %s)\x1b[0m\n  \x1b[1m\x1b[90mnote:\x1b[0m " S "\n",  \
	        __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

#define __chaos_warn(S, ...)                                                   \
	fprintf(stderr,                                                              \
	        "\x1b[1m(%s:%d, %s)\x1b[0m\n  \x1b[1m\x1b[33mwarning:\x1b[0m " S     \
	        "\n",                                                                \
	        __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__)

#define __chaos_errn(S, ...)                                                   \
	do {                                                                         \
		fprintf(stderr,                                                            \
		        "\x1b[1m(%s:%d, %s)\x1b[0m\n  \x1b[1m\x1b[31merror:\x1b[0m " S     \
		        "\n",                                                              \
		        __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__);                  \
		exit(1);                                                                   \
	} while (0)

} //::chaos ////////////////////////////////////////////////////////////////////

#endif // CHAOS_MACROS_HH
