# ESP8266 Enocean

Firmware für den ESP8266 um mittels des EnOcean Pi Funkmoduls Fenstergriffe an den Loxone Miniserver anzubinden.

Der ESP8266 sendet per UDP die Werte auf virtuellen UDP-Eingänge. Die Eingagnswerte sind direkt mit dem Eingang ```AIw``` des Bausteins ```Fensterüberwachung``` kompatibel.
![Loxone](https://www.loxone.com/dede/wp-content/uploads/sites/2/2016/12/Fenster%C3%BCberwachung.png)

## Software

### ESP8266 Firmware
- Allgemeine Einstellungen:
  - ```src/Main.ino```
- Enocean Telegramme
  - ```src/EnoceanHandler.ino#handleWindow```

### Loxone

Virtueller UDP Eingang
- Befehl: ```[Esszimmer=\v]```
- Beispiel: ```[Esszimmer=3]```


## Hardware
![Enocean](image.png)

### Verbinden
```
  ESP - ENOCEAN
   TX - RX  10
   RX - TX   8
  Vcc - Vcc  1
  GND - GND  6
```
![Pinout](https://cdn.weka-fachmedien.de/thumbs/media_uploads/images/1481104354-273-wor3x5p52.jpg.950x534.jpg)

### Einkaufsliste
- EnOcean Pi Funkmodul - 27€
  - https://www.voelkner.de/products/551086/Raspberry-Pi-Erweiterungs-Platine-EnOcean-Pi-Funkmodul.html
  - 4016138857849
- SMA-Steckverbinder Buchse - 6€
  - https://www.voelkner.de/products/603973/Telegaertner-SMA-Steckverbinder-Buchse-Einbau-vertikal-50-J01151A0931-1St..html
  - 2050001913456
- Antenne 868MHz - 15€
  - https://www.voelkner.de/products/762151/H-Tronic-1618115-Funk-Antenne-Frequenz-868MHz.html
  - 4260003173538
