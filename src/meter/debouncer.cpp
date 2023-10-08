#include "debouncer.h"

DeBouncer::DeBouncer(uint32_t t ) : debounce_time(t)
{

}

IRAM_ATTR bool DeBouncer::debounceFilterPasses()
{
    uint64_t time = timeUtils.getTime();

    // prevent noise
    if (time < (last_time + debounce_time))
        return false;

    last_time = time;
    return true;
}

IRAM_ATTR void DeBouncer::reset()
{
    last_time = 0;
}