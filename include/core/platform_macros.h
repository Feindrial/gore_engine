#ifndef GORE_PLATFORM_MACROS_H
#define GORE_PLATFORM_MACROS_H

#if defined(_WIN32) || defined(__WIN64)
	#include <Windows.h>
	#define __GORE_WINDOWS
#elif defined(__linux__)
	#include <libgen.h>         
	#include <unistd.h>         
	#include <linux/limits.h> 
	#define __GORE_LINUX
#else 
	#define __GORE_MAC
#endif

#ifndef __GORE_DEBUG
	#if defined(__GNUC__)
		#define __FORCE_INLINE __attribute__((always_inline))
	#elif defined(_MSC_VER)
		#define __FORCE_INLINE __forceinline
	#endif
#else
	#define __FORCE_INLINE inline
#endif

#ifndef __NO_DISCARD
	#define __NO_DISCARD [[nodiscard]]
#endif

#ifndef __UNLIKELY
	#define __UNLIKELY [[unlikely]]
#endif

#ifndef __LIKELY
	#define __LIKELY [[likely]]
#endif

#ifndef __MAYBE_UNUSED
	#define __MAYBE_UNUSED [[maybe_unused]]
#endif

#ifndef __ASSUME
	#define __ASSUME [[assume]]
#endif

#ifndef __NO_EXCEPT
	#define __NO_EXCEPT noexcept
#endif


#endif