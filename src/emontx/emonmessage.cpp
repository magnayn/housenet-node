//#include <Arduino.h>
#include "emontx.h"
#include "../housenet_node.h"
#include "debugging.h"

extern HousenetNode *node;

bool EmonMessage::parse(const String &message)
{
	if (!message.startsWith("MSG:"))
		return false;

	Debugging.println(".. starts ok");

	int l1 = message.length();
	int l2 = strlen(message.c_str());

	Serial.printf("L1 %d\n L2 %d\n", l1, l2);

	char *cstr = new char[message.length() + 1];
	strcpy(cstr, message.c_str());
	char *saveptr;
	char *token = strtok_r(cstr, ",", &saveptr);

	Debugging.println(".. starts ok");

	while (token != NULL)
	{
		Debugging.println("TOKEN! ");
		if (strncmp("MSG:", token, 4) == 0)
		{
			msg_seq = atoi(token + 4);
		}
		else if (strncmp("Vrms:", token, 5) == 0)
		{
			v = atof(token + 5);
		}
		else if (token[0] == 'P')
		{
			int idx = (int)(token[1] - '1');
			power[idx] = atoi(token + 3);
		}
		else if (token[0] == 'E')
		{
			int idx = (int)(token[1] - '1'); // P1 to P4, E1 to E4 so '1' indexed.

			energy_in[idx] = atoi(token + 3);
			char *eout = strchr(token + 3, ' ');
			if (eout != NULL)
			{
				energy_out[idx] = atoi(eout + 1);
			}
		}

		token = strtok_r(NULL, ",", &saveptr);
	}

	delete[] cstr;
	Debugging.println(".. done");
	// Make sure V > 200V as a test.
	if (v < 200 || msg_seq == 0)
		return false;
    Debugging.println(".. ok");
	return true;
}

DynamicJsonDocument EmonMessage::ToJson() const
{
	DynamicJsonDocument doc(512);
	doc["seq"] = msg_seq;
	doc["v"] = v;
	JsonArray channels = doc.createNestedArray("channels");

	for (int i = 0; i < 4; i++)
	{
		JsonObject channel = channels.createNestedObject();
		channel["power"] = power[i];
		channel["in"] = energy_in[i];
		channel["out"] = energy_out[i];
	}

	return doc;
}