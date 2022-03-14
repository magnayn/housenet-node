# HouseNetNode v3.0


## QuickStart

* Flash device
* Connect over wifi captive portal, http://192.168.4.1:8880
* Configure wifi
* Post some configuration
* (restart)


## Send some configuration

We need to set up what we want the node to do, and where to communicate with mqtt

Create some config - e.g
```
{
  "mqtt": "mqtt.nirima.com",
  "elements": [
    { "type": "state", "id":"main", "pin": 4  },
    { "type": "meter", "id":"leccy", "pin": 5, "debounce": 100 }
  ]
}
```

Post it to the device, e.g:

```
curl -X POST http://192.168.2.103/housenet/config -H "Content-Type: application/json" -d "@config.json"
```

(Restart the device)

You will now see pulling pin D4 to ground will result in MQTT messages:

```
/housenet/24-6F-28-AA-B4-D0/state/main/value 0
/housenet/24-6F-28-AA-B4-D0/state/main/value 1
```


## Element types

### State

Simplest possible monitor. Reports to mqtt 1 when the pin is pulled low (active) and 0 when not (inactive / floating)

#### Config
```
    { "type": "state", "id":"main", "pin": 4  },
```

#### MQTT
```
/housenet/24-6F-28-AA-B4-D0/state/main/value 0
/housenet/24-6F-28-AA-B4-D0/state/main/value 1
```

### Meter

Counts pulses on a pin. The pin is 'de-bounced' to prevent rising/falling edges reporting multiple counts. The MQTT output
will report a 'pulse count' and a 'meter value' if there is a value stored.

There will *only* be a reading returned if the element has been configured with a value.

The 'readings' are sent with persistence to the MQTT topic. When the node restarts, it receives this value in order to 
reset the meter value to whatever was previously stored before the restart (I.E: it uses MQTT for storage, not flash).

#### Config
```
        { "type": "meter", "id":"leccy", "pin": 5, "debounce": 100 }
```

* To set the current meter value, use an equals on the front of the setting so it may determine the difference
between a previously recorded value (or reflected from itself) and a "please set to this value" :

mosquitto_pub -d -h mqtt.nirima.com -t "/housenet/24-6F-28-AA-B4-D0/meter/leccy/reading" -m "=100"


#### MQTT
```
/housenet/24-6F-28-AA-B4-D0/meter/leccy/pulse 79
/housenet/24-6F-28-AA-B4-D0/meter/leccy/reading 114
```



## Concept

ESP is a HouseNetNode.
HouseNetNodes can have multiple elements - an element is an interface of some sort, usually relying on some hardware (onewire, I2c, serial, etc). 
These are identified by their element 'type'
An element can have multiple channels (e.g: onewire may have many things connected to it).

### Namespaces

#### MQTT

/housenet/{node_id}/{type}/{id}/{channel}

#### HTTP

/housenet/elements/{type}/{id}

  
 
