#ifndef __TIME_UTILS_H__
#define __TIME_UTILS_H__

#include <Arduino.h> // uint types



class TimeUtils
{
public:
    TimeUtils();

    uint64_t getTime();
    void update();

private:
    uint32_t low32 = 0;
    uint32_t high32 = 0;
};



#endif