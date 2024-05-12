#include "wifi_save.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Time to sleep in microseconds to sleep
const uint64_t uS_TO_S_FACTOR = 1000000;  // Conversion factor for micro seconds to seconds
// TODO: replace with NVRAM setting, allow setting rate through web interface
const uint64_t TIME_TO_SLEEP = 15;  // Time ESP32 will go to sleep (in seconds)

// GPIO where the DS18B20 temp sensor is connected to
const int oneWireBus = 2;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

void getSensorData();


void setup()
{
    Serial.begin(115200);
    Serial.println("Attempt to connect to wifi");
    // Set wakeup conditions

    // If there is motion detected:
    // esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 0);

    // Every X minutes, wakeup and check the sensors
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);

    // Set up WIFI
    
    if (wifi_set_main())
    {
        Serial.println("Connect WIFI SUCCESS");
    }
    else
    {
        Serial.println("Connect WIFI FAULT");
    }
    sensors.begin();

    // Get sensor and/or perform other tasks. Then back to bed.
    getSensorData();

    esp_deep_sleep_start();
    Serial.println("This will never be printed");

}

void getSensorData() {
    sensors.requestTemperatures();
    float temperatureC = sensors.getTempCByIndex(0);
    float temperatureF = sensors.getTempFByIndex(0);
    Serial.print(temperatureC);
    Serial.println("ºC");
    Serial.print(temperatureF);
    Serial.println("ºF");
}

void loop()
{
    Serial.println("loop");
}