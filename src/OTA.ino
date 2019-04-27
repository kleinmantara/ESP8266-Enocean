/*
 * ====================================================================
 * This file is part of the ESP8266-Enocean distribution
 * (https://github.com/kleinmantara/ESP8266-Enocean).
 * Copyright 2017 Thomas Hart <thart@kleinmantara.de>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * ====================================================================
 */
#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266mDNS.h>

/*
 * Call this method in setup(), directly after setup the WIFI.
 */
void setupOTA() {
  info(F("Starting OTA"));

  ArduinoOTA.setHostname(host);
  ArduinoOTA.setPassword(ota_password);
  ArduinoOTA.begin();


  int waitTimeForOTA = 2000; // 2s

  if (isErrorReason()) {
    sendMessage(String(host) + " EMERGENCY OTA: " + WiFi.localIP().toString() + ", ERROR: " + ESP.getResetInfo());

    // 1 Minute emergency OTA
    waitTimeForOTA = 60000;
  }

  uint32_t beginWait = millis();
  while (millis() - beginWait < waitTimeForOTA) {
    ArduinoOTA.handle();
    yield();
  }

}

void loopOTA() {
  ArduinoOTA.handle();
}

boolean isErrorReason() {
  //https://github.com/esp8266/Arduino/blob/f1a8287bb380cd1fc09a3e0c5c86c9ac7de65788/cores/esp8266/Esp.cpp#L301-L312
  int reason = getReason();

  /**
     0 DEFAULT
     1 WDT
     2 EXCEPTION
     3 SOFT_WDT
     4 SOFT_RESTART > OTA-Update
     5 DEEP_SLEEP_AWAKE
     6 EXT_SYS_RST
  */
  if (reason == 1 // WDT
      || reason == 2 // EXCEPTION
      || reason == 3 // SOFT_WDT
     ) {
    return true;
  }

  return false;
}
