/*
 * (C) 2015 Augustin Cavalier <waddlesplash>
 * All rights reserved. Distributed under the terms of the MIT license.
 */
#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

#include "config.h"

#if defined(HAVE_STDBOOL_H) && !defined(USE_BUILTIN_TYPES)
#  include <stdbool.h>
#else
#  define bool char
#  define true (!false)
#  define false (0)
#endif

#if defined(HAVE_STDINT_H) && !defined(USE_BUILTIN_TYPES)
#  include <stdint.h>
#else
typedef signed char			    int8_t;
typedef short				    int16_t;
typedef int					    int32_t;
typedef long long int		    int64_t;
typedef unsigned char           uint8_t;
typedef unsigned short	        uint16_t;
typedef unsigned int	        uint32_t;
typedef unsigned long long int	uint64_t;
#endif

#endif // PLATFORM_TYPES_H
