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
 #include "CRC8.h"

#define ENOCEAN_HEADER 4
#define ENOCEAN_MAX_DATA 40
CRC8 crc8;

void setupEnocean() {
  Serial.begin(57600);
}

union DataLength {
  byte data[2];
  uint8_t length;
};


void loopEnocean() {

  while(Serial.available()) {
      byte b =  Serial.read();
      if(b == 0x55) {
          byte enocean_header[ENOCEAN_HEADER];
          byte enocean_data[ENOCEAN_MAX_DATA];
          byte enocean_optional[ENOCEAN_MAX_DATA];

          // 1-2 DATA LENGTH
          // 3   OPTIONAL DATA LENGTH
          // 4   PACKET TYPE
          Serial.readBytes(enocean_header, ENOCEAN_HEADER);
          crc8.begin();
          uint8_t checksum = crc8.get_crc8(enocean_header, ENOCEAN_HEADER);
          byte CRC8H = Serial.read();
          if(checksum == CRC8H) {
            //info("Checksum OK " + String(checksum, HEX) + " / " + String(CRC8H, HEX));
          } else {
            warn("Checksum missmatch " + String(checksum, HEX) + " / " + String(CRC8H, HEX));
            return;
          }

          DataLength dl;
          dl.data[0] = enocean_header[1];
          dl.data[1] = enocean_header[0];
          //info("0: " + String(enocean_header[0]));
          //info("1: " + String(enocean_header[1]));
          //info("Data length   : " + String(dl.length));

          byte type = enocean_header[3];
          uint8_t dataLength = dl.length;
          uint8_t optionalLength = enocean_header[2];

          String header = arrayToString(enocean_header, ENOCEAN_HEADER);
#ifdef DEBUG_ENOCEAN
          info("Header:         " + String(header));
          info("Data length   : " + String(dataLength));
          info("Optinal length: " + String(optionalLength));
          info("Type          : " + String(type, HEX));
#endif

          if(dataLength > ENOCEAN_MAX_DATA) {
              warn("data to long: " + String(dataLength));
              return;
          }

          if(optionalLength > ENOCEAN_MAX_DATA) {
              warn("optional to long: " + String(optionalLength));
              return;
          }

          int needed = dataLength + optionalLength + 1; // Checksum

          byte waitCounter;
          byte available;
          do {
            yield();
            available = Serial.available();
            //info("needed   : " + String(needed));
            //info("available: " + String(available));
          } while(available < needed || waitCounter++ > 50);

          if(available >= needed) {
            Serial.readBytes(enocean_data, dataLength);
            Serial.readBytes(enocean_optional, optionalLength);

#ifdef DEBUG_ENOCEAN
            String data = arrayToString(enocean_data, dataLength);
            info("data:     " + data);
            String optional = arrayToString(enocean_optional, optionalLength);
            info("optional: " + optional);
#endif

            // CHECKSUM
            byte CRC8D = Serial.read();

            byte enocean_buffer[2 * ENOCEAN_MAX_DATA];
            memcpy(enocean_buffer             , enocean_data    , dataLength);
            memcpy(enocean_buffer + dataLength, enocean_optional, optionalLength);

            //String all = arrayToString(enocean_buffer, needed-1);
            //info("all: " + all);

            byte rssi = enocean_optional[5];

            crc8.begin();
            uint8_t checksumData = crc8.get_crc8(enocean_buffer, needed-1);
            if(checksumData == CRC8D) {
              //info("Checksum DATA OK " + String(checksumData, HEX) + " / " + String(CRC8D, HEX));

              handleTelegramm(type, rssi, enocean_data, dataLength, enocean_optional, optionalLength);
            } else {
              warn("Checksum DATA missmatch " + String(checksumData, HEX) + " / " + String(CRC8D, HEX));
              return;
            }
          } else {
            warn("missing data");
          }
      } else {
        info("Searching for 0x55...");
      }
  }
}

String arrayToString(byte array[], unsigned int len) {
  char str[32] = "";
  arrayToString(array, len, str);
  return String(str);
}

void arrayToString(byte array[], unsigned int len, char buffer[]) {
    for (unsigned int i = 0; i < len; i++) {
        byte nib1 = (array[i] >> 4) & 0x0F;
        byte nib2 = (array[i] >> 0) & 0x0F;
        buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
        buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
    }
    buffer[len*2] = '\0';
}
