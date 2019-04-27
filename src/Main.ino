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

// VERSION
const String VERSION = "1";

// WIFI
const String WIFI_SSID = "SSID";
const String WIFI_PASSWD = "PASSWORD";

// Hostname and OTA
const char* host = "Encoean";
const char* hostFQN = "ESP-Encoean";
const char* ota_password = "EncoeanPwD";

// Your NTP server e.g. your router
const String NPT_SERVER_IP = "192.168.27.1";

// Your Loxone miniserver IP
IPAddress loxoneIP(192, 168, 27, 15);
const int loxonePort = 9090;

// Pushover
#define PUSHOVER
#ifdef PUSHOVER
const String PO_TOKEN = "TOKEN";
const String PO_USER = "USER";
#endif


// CODE
String logger = String("");

void setup() {
  info(F("Booting"));

  startWIFI();

  setupOTA();

  setupNTP();
  startNTP();

  setupWeb();

  setupEnocean();

  info(F("STARTED"));
}

void loop() {
  loopOTA();
  yield();

  loopWeb();
  yield();

  loopEnocean();
}
