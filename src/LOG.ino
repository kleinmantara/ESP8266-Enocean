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

#define maxLoggerLenght 3000

void debug(String msg) {
  log(F("DEBUG"), msg);
}

void info(String msg) {
  log(F("INFO"), msg);
}

/**
   log warn messages
*/
void warn(String msg) {
  log(F("WARN"), msg);
}

void log(String level, String msg) {
  logger.concat(getNow());
  logger.concat(F(" ["));
  logger.concat(level);
  logger.concat(F("] "));
  logger.concat(msg);
  logger.concat('\n');

  if (logger.length() > maxLoggerLenght) {
    logger = logger.substring(logger.length() - maxLoggerLenght);
  }
}

void msg(String msg) {
  logger.concat(F("[MSG] "));
  logger.concat(msg);
  logger.concat('\n');

  if (logger.length() > maxLoggerLenght) {
    logger = logger.substring(logger.length() - maxLoggerLenght);
  }
}
