/*

Author  : Vincent
Create  : 2021/2/5

if (ESP32 boot check SSID == NULL) {
    // Enter AP mode and access 192.168.4.1 to modify and obtain SSID and PASSWORD
    // Store them in FLASH using NVS
    // Restart
} else {
    // Connect to WiFi using SSID and PASSWORD from FLASH
}

while (within five seconds of restart && reset button is pressed) {
    // Set NVS to NULL
    // Restart
}

user_program();

*/


#include <Arduino.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"


#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>

#define SSID_LENGTH 40

#define WIFI_SET_PIN 21

int record_rst_time();
void nvs_test();
void record_wifi(char *ssid, char *password);
void check_wifi(char *ssid, char *password);

void ap_init();
int wifi_config_server();
void set_wifi_from_url(String get_url);
int wifi_set_main();