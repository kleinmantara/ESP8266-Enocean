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
#include <ESP8266WiFi.h>

void startWIFI() {
  info(F("Starting WiFi"));

  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.setAutoConnect(false);
  WiFi.setAutoReconnect(true);
  WiFi.mode(WIFI_STA);
  WiFi.hostname(hostFQN);
  WiFi.setSleepMode(WIFI_NONE_SLEEP); // https://github.com/esp8266/Arduino/issues/2330#issuecomment-398977655

  WiFi.begin(WIFI_SSID, WIFI_PASSWD);

  if(WL_CONNECTED == WiFi.waitForConnectResult()) {
    String msg = "Started WiFi: " + WiFi.localIP().toString() + " / " + WiFi.SSID();
    info(msg);
    sendMessage(msg);
  } else {
    warn(F("No WiFi found"));
  }
}

void stopWIFI() {
  WiFi.softAPdisconnect();
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
  delay(100);
  WiFi.forceSleepBegin();
  delay(100);
}
