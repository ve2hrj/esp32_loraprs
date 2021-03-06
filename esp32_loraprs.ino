#include <timer.h>
#include "WiFi.h"
#include "loraprs.h"

#define LED_BUILTIN         2
#define LED_TOGGLE_PERIOD   1000

#define LORAPRS_CLIENT

#ifdef LORAPRS_CLIENT
// calibrate client based on server frequency drift report
#define LORAPRS_FREQ        432.499E6
#else
#define LORAPRS_FREQ        432.5E6
#endif

#ifdef LORAPRS_CLIENT
#define LORAPRS_BT_NAME     "loraprs_client"
#define LORAPRS_WIFI_SSID   ""
#define LORAPRS_WIFI_KEY    ""
#define LORAPRS_LOGIN       "NOCALL-0"
#define LORAPRS_PASS        "00000"
#else
#define LORAPRS_BT_NAME     ""
#define LORAPRS_WIFI_SSID   "<mywifi>"
#define LORAPRS_WIFI_KEY    "<key>"
#define LORAPRS_LOGIN       "NOCALL-0"
#define LORAPRS_PASS        "00000"
#endif 

LoraPrs loraPrs(
  LORAPRS_FREQ, 
  LORAPRS_BT_NAME, 
  LORAPRS_WIFI_SSID, 
  LORAPRS_WIFI_KEY,
  LORAPRS_LOGIN,
  LORAPRS_PASS);

auto watchdogLedTimer = timer_create_default();

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, 1);

  Serial.begin(115200);
  while (!Serial);

  loraPrs.setup();

  watchdogLedTimer.every(LED_TOGGLE_PERIOD, toggleWatchdogLed);
}

void loop() {
  loraPrs.loop();
  watchdogLedTimer.tick();
}

bool toggleWatchdogLed(void *) {
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  return true;
}
