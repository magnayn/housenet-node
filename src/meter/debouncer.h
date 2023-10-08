#pragma once
#include "time_utils.h"

class DeBouncer {
    public:
        DeBouncer(uint32_t debounce_time);

        bool debounceFilterPasses();
        void reset();
        
    private:
        uint32_t debounce_time;
        // For Debounce
        volatile uint64_t last_time = 0;

        TimeUtils timeUtils;
};