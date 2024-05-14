// Motion activated ES32-CAM
// Makerguides.com

#include "esp_camera.h"
#include "driver/rtc_io.h"
#include "SD_MMC.h"
#include "EEPROM.h"

// CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

void configCamera()
{
  camera_config_t config;

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_UXGA;
  config.jpeg_quality = 10;
  config.fb_count = 2;
  esp_err_t err = esp_camera_init(&config);

  sensor_t *s = esp_camera_sensor_get();
  s->set_brightness(s, 0);
  s->set_contrast(s, 0);
  s->set_saturation(s, 0);
  s->set_special_effect(s, 0);
  s->set_whitebal(s, 1);
  s->set_awb_gain(s, 1);
  s->set_wb_mode(s, 0);
  s->set_exposure_ctrl(s, 1);
  s->set_aec2(s, 0);
  s->set_ae_level(s, 0);
  s->set_aec_value(s, 300);
  s->set_gain_ctrl(s, 1);
  s->set_agc_gain(s, 0);
  s->set_gainceiling(s, (gainceiling_t)0);
  s->set_bpc(s, 0);
  s->set_wpc(s, 1);
  s->set_raw_gma(s, 1);
  s->set_lenc(s, 1);
  s->set_hmirror(s, 0);
  s->set_vflip(s, 0);
  s->set_dcw(s, 1);
  s->set_colorbar(s, 0);
}

unsigned int incCounter()
{
  unsigned int cnt = 0;
  EEPROM.get(0, cnt);
  EEPROM.put(0, cnt + 1);
  EEPROM.commit();
  return cnt;
}

void enableFlash(bool enable)
{
  if (enable)
  {
    rtc_gpio_hold_dis(GPIO_NUM_4);
    digitalWrite(GPIO_NUM_4, HIGH);
  }
  else
  {
    pinMode(GPIO_NUM_4, OUTPUT);
    digitalWrite(GPIO_NUM_4, LOW);
    rtc_gpio_hold_en(GPIO_NUM_4);
  }
}

void skipPictures(int n)
{
  for (int i = 0; i < n; i++)
  {
    camera_fb_t *fb = esp_camera_fb_get();
    esp_camera_fb_return(fb);
  }
}
// Initialize the micro SD card
void initMicroSDCard()
{
  // Start Micro SD card
  Serial.println("Starting SD Card");
  if (!SD_MMC.begin())
  {
    Serial.println("SD Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE)
  {
    Serial.println("No SD Card attached");
    return;
  }
}
// Get the picture filename based on the current ime
String getPictureFilename()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
    return "";
  }
  char timeString[20];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d_%H-%M-%S", &timeinfo);
  Serial.println(timeString);
  String filename = "/picture_" + String(timeString) + ".jpg";
  return filename;
}

void takePicture()
{
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb)
  {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
  }

  // Path where new picture will be saved in SD Card
  String path = getPictureFilename();
  Serial.printf("Picture file name: %s\n", path.c_str());
  // Save picture to microSD card

  File file = SD_MMC.open(path.c_str(), FILE_WRITE);
  if (!file)
  {
    Serial.printf("Failed to open file in writing mode");
  }
  else
  {
    file.write(fb->buf, fb->len); // payload (image), payload length
    Serial.printf("Saved: %s\n", path.c_str());
  }
  file.close();
  esp_camera_fb_return(fb);
}

void deepSleep(int atleast)
{
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
  rtc_gpio_pulldown_en(GPIO_NUM_13);
  rtc_gpio_pullup_dis(GPIO_NUM_13);
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_13, 1);
  delay(atleast);
  esp_deep_sleep_start();
}

void capturePicture()
{

  enableFlash(true);
  EEPROM.begin(8);
  initMicroSDCard();
  configCamera();
  skipPictures(10);
  takePicture();
  enableFlash(false);
}