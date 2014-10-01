#ifndef _TS_COMMON_DEF_H
#define _TS_COMMON_DEF_H

#define LIB_TS_DLL_OUTPUT 1

#if LIB_TS_DLL_OUTPUT

#define LIB_TINY_TS_API __declspec(dllexport)
#else
#define LIB_TINY_TS_API __declspec(dllimport)
#endif

#endif