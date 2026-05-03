#ifndef __khrplatform_h_
#define __khrplatform_h_

/*
** Copyright (c) 2008-2018 The Khronos Group Inc.
** SPDX-License-Identifier: MIT
*/

#include <stdint.h>

typedef int8_t   khronos_int8_t;
typedef uint8_t  khronos_uint8_t;
typedef int16_t  khronos_int16_t;
typedef uint16_t khronos_uint16_t;
typedef int32_t  khronos_int32_t;
typedef uint32_t khronos_uint32_t;
typedef int64_t  khronos_int64_t;
typedef uint64_t khronos_uint64_t;
typedef float    khronos_float_t;

#ifdef _WIN64
typedef signed   long long int khronos_ssize_t;
typedef unsigned long long int khronos_usize_t;
#else
typedef signed   long int khronos_ssize_t;
typedef unsigned long int khronos_usize_t;
#endif

typedef khronos_uint8_t khronos_ubyte_t;
typedef khronos_int8_t  khronos_byte_t;

#if defined(_WIN64)
typedef signed   long long int khronos_intptr_t;
typedef unsigned long long int khronos_uintptr_t;
#elif defined(_WIN32)
typedef signed   long int khronos_intptr_t;
typedef unsigned long int khronos_uintptr_t;
#else
#  include <stdint.h>
typedef intptr_t  khronos_intptr_t;
typedef uintptr_t khronos_uintptr_t;
#endif

#define KHRONOS_SUPPORT_INT64  1
#define KHRONOS_SUPPORT_FLOAT  1

#if defined(__GNUC__)
#  define KHRONOS_APICALL  __attribute__((visibility("default")))
#elif defined(_WIN32)
#  define KHRONOS_APICALL  __declspec(dllimport)
#else
#  define KHRONOS_APICALL
#endif

#if defined(_WIN32) && !defined(_WIN32_WCE) && !defined(__SCITECH_SNAP__)
#  define KHRONOS_APIENTRY __stdcall
#else
#  define KHRONOS_APIENTRY
#endif

#define KHRONOS_CALLBACK KHRONOS_APIENTRY

typedef void (*khronos_utime_nanoseconds_t)(void);
typedef void (*khronos_stime_nanoseconds_t)(void);

#endif /* __khrplatform_h_ */
