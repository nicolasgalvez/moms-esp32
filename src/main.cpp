#include "wifi_save.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

void setup()
{
    Serial.begin(115200);
    if (wifi_set_main())
    {
        Serial.println("Connect WIFI SUCCESS");
    }
    else
    {
        Serial.println("Connect WIFI FAULT");
    }
    sensors.begin();
}

void loop()
{
    sensors.requestTemperatures();
    float temperatureC = sensors.getTempCByIndex(0);
    float temperatureF = sensors.getTempFByIndex(0);
    Serial.print(temperatureC);
    Serial.println("ºC");
    Serial.print(temperatureF);
    Serial.println("ºF");
    delay(5000);

    Serial.println("loop");
}