#ifndef __SPDATABASE_H
#define __SPDATABASE_H
#ifdef _WIN32
    #ifdef DLL_EXPORT
    #define SPDLLEXPORT extern __declspec(dllexport)
    #else
    #define SPDLLEXPORT extern __declspec(dllimport)
    #endif
#else
    #define SPDLLEXPORT
#endif
#include "message.h"
#include "spmodeldatabase.h"
#include "spenginedatabase.h"
#include"sprecorederdatabase.h"
#endif