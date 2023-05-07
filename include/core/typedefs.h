#pragma once

#include <cstdint>
#include <stdfloat>

namespace gore
{
	typedef int8_t			i8;
	typedef int16_t			i16;
	typedef int32_t			i32;
	typedef int64_t			i64;

	typedef uint8_t			ui8;
	typedef uint16_t		ui16;
	typedef uint32_t		ui32;
	typedef uint64_t		ui64;

	typedef int_fast8_t		i8f;
	typedef int_fast16_t	i16f;
	typedef int_fast32_t	i32f;
	typedef int_fast64_t	i64f;

	typedef uint_fast8_t	ui8f;
	typedef uint_fast16_t	ui16f;
	typedef uint_fast32_t	ui32f;
	typedef uint_fast64_t	ui64f;

	typedef std::float16_t	f16;
	typedef std::float32_t  f32;
	typedef std::float64_t	f64;
	typedef std::float128_t f128;
	typedef std::bfloat16_t bf16;
}