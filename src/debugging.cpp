#include "debugging.h"

_Debugging Debugging;

_Debugging::_Debugging()
{
}

void _Debugging::print(const String &data)
{
    Serial.printf("%s", data.c_str());
    if (websocket != NULL)
    {
        websocket->text(data);
    }
}
void _Debugging::println(const String &data)
{
    Serial.printf("%s", data.c_str());
    Serial.printf("\n");
    if (websocket != NULL)
    {
        websocket->text(data);
        websocket->text("\n");
    }
}
