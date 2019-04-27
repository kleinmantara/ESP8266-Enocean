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
#include <NtpClientLib.h>
#include <TimeLib.h>

boolean timeValid;

void setupNTP() {
	info(F("NTP: setup"));

	NTP.onNTPSyncEvent([](NTPSyncEvent_t error) {
		if (error) {
			timeValid = false;
			if (error == noResponse) {
					warn(F("NTP: server not reachable"));
				}
			else if (error == invalidAddress) {
					warn(F("NTP: Invalid server address"));
				}
		} else {
			if(timeValid == false) {
				info(F("NTP: successfully sync"));
				timeValid = true;
			}
		}
	});
}

boolean isTimeValid() {
	return timeValid;
}

void startNTP() {
	info(F("NTP: start"));
	NTP.begin(NPT_SERVER_IP, 1, true);
	NTP.setInterval(10, 3600); // 30s / 1h
}

void stopNTP() {
	info(F("NTP: stop"));
	NTP.stop();
}
