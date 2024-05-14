#include <Wifi_save.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <HTTPClient.h>
#include "esp_system.h"
#include <iostream>
#include "esp_wifi.h"
#include <string>
#include "root_ca.h"
#include "motion_cam.h"
#include "soc/soc.h"           // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems

// Domain Name with full URL Path for HTTP POST Request
String serverName = "https://rat-rave.databowie.com/api/";

String my_Api_key = "EnterYourApiKey";

// Time to sleep in microseconds to sleep
const uint64_t uS_TO_S_FACTOR = 1000000; // Conversion factor for micro seconds to seconds
// TODO: replace with NVRAM setting, allow setting rate through web interface
const uint64_t TIME_TO_SLEEP = 5 * 60; // Time ESP32 will go to sleep (in seconds)

// GPIO where the DS18B20 temp sensor is connected to
const int oneWireBus = 2;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

// REPLACE WITH YOUR TIMEZONE STRING: https://github.com/nayarsystems/posix_tz_db/blob/master/zones.csv
String myTimezone = "PST8PDT,M3.2.0,M11.1.0";

// Timing without delay:
unsigned long time_now = 0;

void getSensorData();
void sendHeartBeat();
void initTime(String timezone);
void setTimezone(String timezone);

void setup()
{
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // disable brownout detector
    time_now = millis();
    Serial.begin(115200);
    delay(2000);
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
    // Initialize time with timezone
    initTime(myTimezone); 
    sensors.begin();

    // Get sensor and/or perform other tasks. Then back to bed.
    getSensorData();
    sendHeartBeat();
    capturePicture();

    esp_deep_sleep_start();
    Serial.println("This will never be printed");
}

void getSensorData()
{
    sensors.requestTemperatures();
    float temperatureC = sensors.getTempCByIndex(0);
    float temperatureF = sensors.getTempFByIndex(0);
    Serial.print(temperatureC);
    Serial.println("ºC");
    Serial.print(temperatureF);
    Serial.println("ºF");
}

void sendHeartBeat()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("WiFi Disconnected");
        return;
    }

    WiFiClientSecure *client = new WiFiClientSecure;
    HTTPClient http;

    uint64_t macAddress = ESP.getEfuseMac();
    String fullRoute = serverName + "heartbeat";

    client->setCACert(root_ca_pem);
    // client->setInsecure();
    // Your Domain name with URL path or IP address with path
    http.begin(*client, fullRoute);

    // If you need Node-RED/server authentication, insert user and password below
    // http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

    // Specify content-type header
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    // Data to send with HTTP POST
    String httpRequestData = "api_key=" + my_Api_key + "&device_id=" + macAddress + "&status=ok";
    // Send HTTP POST request
    int httpResponseCode = http.POST(httpRequestData);
    Serial.println(httpRequestData);
    Serial.println(httpRequestData);
    // If you need an HTTP request with a content type: application/json, use the following:
    // http.addHeader("Content-Type", "application/json");
    // int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

    // If you need an HTTP request with a content type: text/plain
    // http.addHeader("Content-Type", "text/plain");
    // int httpResponseCode = http.POST("Hello, World!");

    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    Serial.println(http.getString());

    // Free resources
    http.end();
}

void loop()
{
    Serial.println("loop");
}

// Function to set timezone
void setTimezone(String timezone)
{
    Serial.printf("  Setting Timezone to %s\n", timezone.c_str());
    setenv("TZ", timezone.c_str(), 1); //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
    tzset();
}

// Connect to NTP server and adjust timezone
void initTime(String timezone)
{
    struct tm timeinfo;
    Serial.println("Setting up time");
    configTime(0, 0, "pool.ntp.org"); // First connect to NTP server, with 0 TZ offset
    if (!getLocalTime(&timeinfo))
    {
        Serial.println(" Failed to obtain time");
        return;
    }
    Serial.println("Got the time from NTP");
    // Now we can set the real timezone
    setTimezone(timezone);
}
