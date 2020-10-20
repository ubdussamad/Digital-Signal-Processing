#include <cstdio>

// Just replace LOGGER_NAME with anything you want
// Current its replaced with X
#ifndef DEBUG
#define DEBUG           0
#endif

#ifndef SHELL_BASH
#define SHELL_BASH      0
#endif

#ifndef ENABLE_WARNING
#define ENABLE_WARNING  0
#define ENABLE_INFO     0
#define ENABLE_ERROR    0
#define ENABLE_SPECIAL  0
#endif

#if SHELL_BASH
#define WARN_COLOR_YELLOW_BEGIN_  "\033[0;33m"
#define INFO_COLOR_GREEN_BEGIN_   "\033[0;32m"
#define ERROR_COLOR_RED_BEGIN_    "\033[0;31m"
#define SPECIAL_COLOR_BLUE_BEGIN_ "\033[0;34m"
#define CLEAR_COLOR_              "\033[0m"
#else
#define WARN_COLOR_YELLOW_BEGIN_  ""
#define INFO_COLOR_GREEN_BEGIN_   ""
#define ERROR_COLOR_RED_BEGIN_    ""
#define SPECIAL_COLOR_BLUE_BEGIN_ ""
#define CLEAR_COLOR_              ""
#endif

#if DEBUG | ENABLE_INFO
#define X_PRINTF(f_, ...) printf( (f_), __VA_ARGS__)
#else
#define X_PRINTF(f_, ...)
#endif

#if ENABLE_WARNING | DEBUG
#define X_WARN(x) printf( "\n%sWarning: %s%s" , WARN_COLOR_YELLOW_BEGIN_, x, CLEAR_COLOR_ )
#else
#define X_WARN(x) 
#endif

#if ENABLE_INFO | DEBUG
#define X_INFO(x) printf("\n%sInfo: %s%s" , INFO_COLOR_GREEN_BEGIN_, x, CLEAR_COLOR_ )
#else
#define X_INFO(x) 
#endif

#if ENABLE_ERROR | DEBUG
#define X_ERROR(x, ...) printf("\nError: %s",ERROR_COLOR_RED_BEGIN_); printf(x, ##__VA_ARGS__); printf("%s",CLEAR_COLOR_)
#else
#define X_ERROR(x, ...) 
#endif



#if ENABLE_SPECIAL | DEBUG
#define X_SPECIAL_LOG(x) printf("\n%sSpecial Log: %s%s\n" , SPECIAL_COLOR_BLUE_BEGIN_, x, CLEAR_COLOR_ )
#else
#define X_SPECIAL_LOG(x) 
#endif