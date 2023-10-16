#ifndef __BASECOUNTER_H__
#define __BASECOUNTER_H__
#include "time_utils.h"
#include "../utils/debouncer.h"

#include <functional>

class BaseSensor;

typedef std::function<void(const BaseSensor *item)> SensorChangeHandlerFunction;

void interruptFn(void *arg);

// A base sensor - look at some kind of pin, and raise events when it changes

class BaseSensor
{
public:
    BaseSensor(uint8_t pin, uint32_t debounce_time);
    ~BaseSensor();

    void OnChangeCall(SensorChangeHandlerFunction onRequest);

    void Process();

    virtual void Interrupt() = 0;
private:

protected:
    uint8_t   m_pin;
    DeBouncer m_deBouncer;

    SensorChangeHandlerFunction m_function;

    QueueHandle_t m_isrQueue;
};

#endif