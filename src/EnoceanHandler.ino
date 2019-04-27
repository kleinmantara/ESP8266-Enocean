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
void handleTelegramm(byte type, byte rssi, byte d[], uint8_t dl, byte enocean_optional[], uint8_t ol) {

  // d[0] telegramm type
  if(d[0] == 0xF6) {
      handleWindow(rssi, d, dl);
  } else {
    warn("unkown telegramm type " + String(d[0], HEX));
  }
}

void handleWindow(byte rssi, byte d[], uint8_t dl) {
  // E0 - Geschlossen > AUF
  // D0 - Auf > KIPP
  // C0 - Kipp > AUF
  // F0 - Auf > GESCHLOSSEN

 // check device id
  if(d[2]==0x05 &&
     d[3]==0x06 &&
     d[4]==0x07 &&
     d[5]==0x08) {
       windowEvent("Küche", rssi, d);
  } else if(
     d[2]==0x06 &&
     d[3]==0x07 &&
     d[4]==0x08 &&
     d[5]==0x09) {
       windowEvent("Esszimmer", rssi, d);
  } else {
    String data = arrayToString(d, dl);
    info("Unkown window: " + data + " > " + getWindowState(d[1]));
  }
}

void windowEvent(String name, byte rssi, byte d[]) {
  byte windowState = d[1];
  info(String(rssi) + " dBm" + " > " + name + ": " + getWindowState(windowState));
  sendToLoxone("[" + name + "=" + getLoxoneState(windowState) + "]");
}


String getWindowState(byte b) {
  if(b == 0xE0) {
    // Geschlossen > AUF
    return F("Offen");
  } else if(b == 0xD0) {
    // D0 - Auf > KIPP
    return F("Gekippt");
  } else if(b == 0xC0) {
    // C0 - Kipp > AUF
    return F("Offen");
  } else if(b == 0XF0) {
    // F0 - Auf > GESCHLOSSEN
    return F("Geschlossen");
  }

  return "Unkown state: " + String(b, HEX);
}

String getLoxoneState(byte b) {
  // https://www.loxone.com/dede/kb/fensterueberwachung/

  if(b == 0xE0) {
    // Geschlossen > AUF
    return F("3");
  } else if(b == 0xD0) {
    // D0 - Auf > KIPP
    return F("2");
  } else if(b == 0xC0) {
    // C0 - Kipp > AUF
    return F("3");
  } else if(b == 0XF0) {
    // F0 - Auf > GESCHLOSSEN
    return F("1");
  }

  return "0";
}
