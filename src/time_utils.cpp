#include "time_utils.h"

// provided this is called once every ~47 days, it will correctly
// track the rollovers.




TimeUtils::TimeUtils() 
{

}

uint64_t TimeUtils::getTime() {
    update();
    
    uint64_t l = (uint64_t)low32;
    uint64_t h = ((uint64_t)high32) << 32;
    
    return (l | h);    
}

void TimeUtils::update() {
    uint32_t new_low32 = millis();
    uint32_t new_high32 = high32;

    if (new_low32 < low32) {
        new_high32++;
    }

    low32 = new_low32;
    high32 = new_high32;

}
