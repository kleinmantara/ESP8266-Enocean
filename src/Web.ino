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
#include <ESP8266WebServer.h>

String header = "<html><head><title>Enocean</title></head><body>"; //-F
String htmlType = "text/html"; //-F

ESP8266WebServer server(80);

void setupWeb() {
  info(F("Starting webserver"));

  server.on("/", handleIndex);
  server.on("/mark", handleMark);
  server.on("/reboot", handleReboot);

  server.begin();
}


void loopWeb() {
  server.handleClient();
}

void handleReboot() {
  forward(F("/"));
  ESP.restart();
}

void handleMark() {
  info("--- MARK ---");
  forward(F("/"));
}

void handleIndex() {
  String msg = header;

  msg.concat(F("<h2>Encoean</h2>"));
  msg.concat(F("<pre>"));

  adminSection(msg, F("VERSIONS"));
  adminKey(msg, F("Version"));
  msg.concat(VERSION);
  adminKey(msg, F("Date"));
  msg.concat(getNow());
  adminKey(msg, F("Chip Id"));
  msg.concat(String(ESP.getChipId()));
  adminKey(msg, F("Core ver"));
  msg.concat(String(ESP.getCoreVersion()));
  adminKey(msg, F("SDK ver"));
  msg.concat(String(ESP.getSdkVersion()));
  adminKey(msg, F("Boot ver"));
  msg.concat(String(ESP.getBootVersion()));


  adminSection(msg, F("BOOTING"));
  adminKey(msg, F("Boot mode"));
  msg.concat(String(ESP.getBootMode()));
  adminKey(msg, F("Reset"));
  msg.concat(ESP.getResetInfo());
  adminKey(msg, F("Actions"));
  adminLink(msg, F("Reboot"), F("/reboot"));


  adminSection(msg, F("CPU"));
  adminKey(msg, F("MHz"));
  msg.concat(String(ESP.getCpuFreqMHz()));


  adminSection(msg, F("WIFI"));
  adminKey(msg, F("RSSI"));
  msg.concat(String(WiFi.RSSI()));


  adminSection(msg, F("FREE"));
  adminKey(msg, F("Free Heap"));
  msg.concat(String(getReadableBytes(ESP.getFreeHeap())));
  adminKey(msg, F("Flash real"));
  msg.concat(String(getReadableBytes(ESP.getFlashChipRealSize())));
  adminKey(msg, F("Flash comp"));
  msg.concat(String(getReadableBytes(ESP.getFlashChipSize())));
  adminKey(msg, F("Sketch Size"));
  msg.concat(String(getReadableBytes(ESP.getSketchSize())));
  adminKey(msg, F("Sketch Free"));
  msg.concat(String(getReadableBytes(ESP.getFreeSketchSpace())));

  msg.concat(F("</pre>"));

  msg.concat(F("<h2>Log messages("));
  msg.concat(logger.length());
  msg.concat(F(")</h2>"));
  msg.concat(F("<pre>"));
  msg.concat(logger);
  msg.concat(F("</pre>"));

  server.send(200, htmlType, msg);
}

void adminSection(String &msg, String title) {
  msg.concat(F("\n\n<b> ===["));
  msg.concat(title);
  msg.concat(F("]============================</b>"));
}

void adminKey(String &msg, String key) {
  msg.concat(F("\n"));
  int spaces = 12 - key.length();
  for (byte i = 0; i < spaces; i++) {
    msg.concat(F(" "));
  }
  msg.concat(key);
  msg.concat(F(": "));
}

void adminLink(String &msg, String title, String url) {
  msg.concat(F("<a href=\""));
  msg.concat(url);
  msg.concat(F("\">"));
  msg.concat(title);
  msg.concat(F("</a>"));
}

void serverHeader() {
  server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  server.sendHeader("Pragma", "no-cache");
  server.sendHeader("Expires", "-1");
}

void forward(String url) {
  serverHeader();
  server.sendHeader(F("Location"), url, true);
  server.setContentLength(CONTENT_LENGTH_UNKNOWN);
  server.send(302, F("text/plain"), "");
  server.client().stop(); // Stop is needed because we sent no content length
}
