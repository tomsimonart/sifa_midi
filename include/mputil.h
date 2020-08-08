#ifndef MPUTIL_H
#define MPUTIL_H

#ifdef WIN32
    #include <windows.h>
    // Sleep for MS amount of milli seconds
    #define SLEEP(MS) Sleep(MS) 
#else
    #include <unistd.h>
    // Sleep for MS amount of milli seconds
    #define SLEEP(MS) usleep(MS*1000)
#endif

#endif