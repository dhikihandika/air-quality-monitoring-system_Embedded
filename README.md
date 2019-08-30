# air-quality-monitoring-system
it is project get on Internship PT. Parametrik Research and Development

# Require Hardware:
1. ESP8266
2. Arduino Promini
3. MQ-135 GAS Sensor
4. Modul comunication RS-485
5. Other device to signal proccessig and voltage regulator

# Require Software:
1. Arduino IDE (Integrated Development Evironment)
   * Add library to get ESP8266 board
      - Start Arduino and open Preferences window.
      - Enter https://arduino.esp8266.com/stable/package_esp8266com_index.json into Additional Board Manager URLs field. You can add               multiple URLs, separating them with commas.
      - Open Boards Manager from Tools > Board menu and find esp8266 platform.
      - Select the version you need from a drop-down box.
      - Click install button.
      - Don't forget to select your ESP8266 board from Tools > Board menu after installation.
   * Add library HTTPClient
      - Download library HTTPClient from github or you can look here https://www.arduinolibraries.info/libraries/http-client
      - Start/open Arduino IDE
      - open sketch> choose include library> add.ZIP library and choose your file HTTPClient has been download
      - open sketch> choose include library> Manage Libraries> find HTTPClient and install

# Attention !!
- Before make wiring and code program pay attention you must understand spesification each device 
- On project you must create two program on Arduino IDE. its use to;
1. Collect data sensor MQ135 form Arduino Promini
2. Post data to cloud and get data from another device (Arduino Promini) from ESP8266
- Communication Arduino promini and ESP8266 used RS-485 module (you can use SoftwareSerial())
