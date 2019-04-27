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
#include <Pushover.h>


extern "C" {
#include "user_interface.h"
}

void sendMessage(String text) {
  String title = String(host);
  title.concat(F(" ["));
  title.concat(VERSION);
  title.concat(F("]"));

  info("SEND: " + text);
#ifdef PUSHOVER
  Pushover po = Pushover(PO_TOKEN, PO_USER);
  po.setTitle(title);
  po.setMessage(text);
  po.setSound(F("none"));
  po.setUrl("http://" + WiFi.localIP().toString() +"/");
  po.setUrlTitle(F("Web-Admin"));
  po.send();
#endif
}

/**
   0 DEFAULT
   1 WDT
   2 EXCEPTION
   3 SOFT_WDT
   4 SOFT_RESTART > OTA-Update
   5 DEEP_SLEEP_AWAKE
   6 EXT_SYS_RST
*/
int getReason() {
  //https://github.com/esp8266/Arduino/blob/f1a8287bb380cd1fc09a3e0c5c86c9ac7de65788/cores/esp8266/Esp.cpp#L301-L312
  rst_info* rinfo = ESP.getResetInfoPtr();

  return rinfo->reason;
}

String getNow()
{
  String message = String("");
  message.concat(appendZeros(hour()));
  message.concat(F(":"));
  message.concat(appendZeros(minute()));
  message.concat(F(":"));
  message.concat(appendZeros(second()));
  message.concat(F(" "));
  message.concat(appendZeros(day()));
  message.concat(F("."));
  message.concat(appendZeros(month()));
  message.concat(F("."));
  message.concat(year());

  return message;
}

String getReadableBytes(unsigned int bytes) {
  float kB = bytes / 1024;
  float mB = kB / 1024;
  return String(mB) + "mB / " + kB + "kB /" + String(bytes) + "B";
}

String appendZeros(byte value)
{
  if (value < 10) {
    String s("0");
    s.concat(value);
    return s;
  }

  return String(value);
}
