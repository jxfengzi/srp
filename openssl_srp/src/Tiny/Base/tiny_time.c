/**
 * Copyright (C) 2013-2015
 *
 * @author jxfengzi@gmail.com
 * @date   2013-11-19
 *
 * @file   tiny_time.c
 *
 * @remark
 *      set tabstop=4
 *      set shiftwidth=4
 *      set expandtab
 */

#include "tiny_time.h"

#ifdef _WIN32
int gettimeofday(struct timeval *tv, void *tz)
{
    time_t clock;
    struct tm tm;
    SYSTEMTIME wtm;

    GetLocalTime(&wtm);
    tm.tm_year     = wtm.wYear - 1900;
    tm.tm_mon      = wtm.wMonth - 1;
    tm.tm_mday     = wtm.wDay;
    tm.tm_hour     = wtm.wHour;
    tm.tm_min      = wtm.wMinute;
    tm.tm_sec      = wtm.wSecond;
    tm.tm_isdst    = -1;
    clock = mktime(&tm);
    tv->tv_sec = (long)clock;
    tv->tv_usec = wtm.wMilliseconds * 1000;

    return (0);
}
#endif /* _WIN32 */

#ifdef _WIN32
void tiny_sleep(int second)
{
    Sleep(second * 1000);
}

void tiny_usleep(int usecond)
{
    Sleep(usecond);
}
#else
void tiny_sleep(int second)
{
    sleep(second);
}

void tiny_usleep(int usecond)
{
    usleep(usecond);
}
#endif // _WIN32

#define BEIJINGTIME     8
#define DAY             (60*60*24)
#define YEARFIRST       2001
#define YEARSTART       (365*(YEARFIRST-1970) + 8)
#define YEAR400         (365*4*100 + (4*(100/4 - 1) + 1))
#define YEAR100         (365*100 + (100/4 - 1))
#define YEAR004         (365*4 + 1)
#define YEAR001         365

int tiny_getstrtime(char buf[], int len)
{
    struct timeval tv;
    long sec = 0;
    int usec = 0;
    int yy = 0;
    int mm = 0;
    int dd = 0;
    int hh = 0;
    int mi = 0;
    int ss = 0;
    int ms = 0;
    int ad = 0;
    int y400 = 0;
    int y100 = 0;
    int y004 = 0;
    int y001 = 0;
    int m[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int i = 0;

    memset(&tv, 0, sizeof(struct timeval));
    gettimeofday(&tv, NULL);
    sec = tv.tv_sec;
    usec = tv.tv_usec;
    sec = sec + (60 * 60)*BEIJINGTIME;
    ad = sec / DAY;
    ad = ad - YEARSTART;
    y400 = ad / YEAR400;
    y100 = (ad - y400*YEAR400) / YEAR100;
    y004 = (ad - y400*YEAR400 - y100*YEAR100) / YEAR004;
    y001 = (ad - y400*YEAR400 - y100*YEAR100 - y004*YEAR004) / YEAR001;
    yy = y400 * 4 * 100 + y100 * 100 + y004 * 4 + y001 * 1 + YEARFIRST;
    dd = (ad - y400*YEAR400 - y100*YEAR100 - y004*YEAR004) % YEAR001;

    // month & day
    if (0 == yy % 1000)
    {
        if (0 == (yy / 1000) % 4)
        {
            m[1] = 29;
        }
    }
    else
    {
        if (0 == yy % 4)
        {
            m[1] = 29;
        }
    }

    for (i = 1; i <= 12; i++)
    {
        if (dd - m[i] < 0)
        {
            break;
        }
        else
        {
            dd = dd - m[i];
        }
    }

    mm = i;
    
    // hour
    hh = sec / (60 * 60) % 24;
    
    // minute
    mi = sec / 60 - sec / (60 * 60) * 60;
    
    // second
    ss = sec - sec / 60 * 60;

    // ms
    ms = (usec / 1000) % 1000;

    tiny_snprintf(buf, len, "%d-%02d-%02d %02d:%02d:%02d.%03d", yy, mm, dd, hh, mi, ss, ms);

    return 0;
}
