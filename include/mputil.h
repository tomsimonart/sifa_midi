#ifndef MPUTIL_H
#define MPUTIL_H

#include <stdlib.h>

#ifdef WIN32
    #include <windows.h>
    // Sleep for MS amount of milli seconds
    #define SLEEP(MS) Sleep(MS) 
    #define CLEAR() system("cls")
    #define UNREF_PARAM(PARAM) UNREFERENCED_PARAMETER(PARAM)
#else
    #include <unistd.h>
    // Sleep for MS amount of milli seconds
    #define SLEEP(MS) usleep(MS*1000)
    #define CLEAR() system("clear")
#endif

#endif