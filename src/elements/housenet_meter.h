#ifndef __HOUSENET_ELEMENT_METER_H__
#define __HOUSENET_ELEMENT_METER_H__
#pragma once

#include "housenet_node.h"
#include "utils/meter.h"
#include "sensor/pin/pulsecounter.h"

class HousenetMeterElement : public HousenetElement {
    public:
        static const String TYPE;

    public:
        HousenetMeterElement(HousenetNode *parent, String id, uint8_t pin, uint32_t debounce, int mode, bool pullup);
        virtual void Process();

        virtual String GetType() const {
            return TYPE;
        }

        virtual void OnMessage(String& topic, String& value);
        
        virtual String GetState( String channel );
        virtual void   SetState( String channel, String data );
    protected:
        void OnPulseCounterEvent(const BaseSensor* pm);


    private:
        PulseCounter*   m_pulse_counter   = nullptr;
        Meter           m_meter;
        bool            m_meter_updated   = false;
};


#endif