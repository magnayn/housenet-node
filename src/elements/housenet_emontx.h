#ifndef __HOUSENET_EMONTX_H__
#define __HOUSENET_EMONTX_H__

//#include <Arduino.h>
#include "housenet_node.h"


class HousenetEmonTXElement : public HousenetElement {
    public:
        static const String TYPE;

    public:
        HousenetEmonTXElement(HousenetNode *parent, String id);
        virtual void Process();
        virtual String GetType() const {
            return TYPE;
        }
        virtual String GetState( String channel );
    protected:
        void processEmon(const EmonMessage &msg);
        void publish_emon(const EmonMessage &msg, AsyncWebSocketClient *client);
    
    private:
        EmonTX emontx;
        EmonMessage last_emon_message;
};

#endif