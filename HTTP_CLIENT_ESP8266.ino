//====================================================================================================================================================================//
//*--------------------------------------------------------------------- Iniziatioan Program ------------------------------------------------------------------------*//
//====================================================================================================================================================================//
#define VERBOSE 1
//#defiine USING_MQTT 1
#define HTTP_POST 1

//My Thingsboard token is "iwt7oimSo7ivAXXRe6dl"
#include <PubSubClient.h>                                 // Library Publish Susbcribe ESP8266
#include <ESP8266WiFi.h>                                  // Library use the wifi on ESP8266 device
#include <ESP8266HTTPClient.h>                            // Library can make ESP8266 interact with Web Server
#include <Wire.h>                                         // Library allow to using communication I2C device
#include <SoftwareSerial.h>                               // Library to use SoftwareSerial  
#include <NTPClient.h>                                    // Library to connection ESP8266 to NTP (Network Time Protocol) Server
#include <WiFiUdp.h>                                      // Library use UDP (User Diagram Protocol) to send and receive timestamp from NTP Server
//#include <WiFi.h>

#define co2zero 18                                        // Define co2zero = 18
SoftwareSerial port(D6, D5);                              // Pin RX, TX ESP8266

// Update value for connection your networkLibrary
const char* id = "Parametrik Office";                     // SSID name WiFi network can be use
const char* keypass = "tabassam";                         // Password your WiFi network
///const char* serv = "demo.thingsboard.io";              // Server Address can be used

WiFiUDP UDP;                                              // Variable name (UDP) User Diagram Protocol "ntpUDP" can be use send and receive timestamps with NTP Server
NTPClient Tclient(UDP);                                   // Variable name NTP it is timeClient

WiFiClient ClientESP;                                     // Variable name WiFiClient it is "espClient"
PubSubClient client(ClientESP);                           // Variable name PubSubClient it is "espClient"

long lastmq = 0;                                          // Variable declaration "lastmq" it is Long data type
char msg[200];                                            // Variable decalration "msg" it is char with byte of buffer it is [200]
int startUp = 0;                                          // Varibale decalaration "startUp" it is Integer data type


//====================================================================================================================================================================//
//------------------------------------------------------------ Wifi Setup connection Porocedure ----------------------------------------------------------------------//
//====================================================================================================================================================================//
void setup_wifi() {                                       // Procedure Setup_wifi ()
  delay(10);                                              // Delay 10 ms
  if (WiFi.status() != WL_CONNECTED) {                    // Function if wifi status not equal connected
    delay(10);                                            // Delay 10 ms
    Serial.println("Wifi connection to the network, ");   // Print on serial monitor "Wifi connection to the network"
    Serial.println();                                     //            --||--
    Serial.print ("Connection to ");                      //            --||--
    Serial.println(id);                                 // Print on serial monitor variable SSID can be create
    WiFi.begin (id, keypass);                              // Inisialisasi pengaturan WiFi (SSID dan Password)
  }

  while (WiFi.status () != WL_CONNECTED) {               // While runnig program until WiFi status has been Connected
    delay(500);                                           // Delay 500 ms
    Serial.print(".");                                    // Print on serial monitor ".."
  }
  randomSeed(micros());                                  // Intializes to generate random number on a computer
  Serial.println("Connected");                           // Print on serial monitor "Connected"
  Serial.println("IP Address");                          // Print on serial monitor "IP Address"
  Serial.println(WiFi.localIP());                        // Print on serial monitor character inside function WiFi.LocalIP();
  Serial.println("");                                    // print on serial monitor
  delay(2000);                                           // Delay 2000 ms
}


//====================================================================================================================================================================//
//------------------------------------------------------------ Setup connection with PC and SoftwareSerial -----------------------------------------------------------//
//====================================================================================================================================================================//
void setup() {
  pinMode(BUILTIN_LED, OUTPUT);                           // Inialize BUILTIN_LED pin as an output
  Serial.begin(115200);                                   // Initialize data rate transmission upload program it is 115200 bps
  port.begin(115200);                                     // initialize data rate transmissin comunication SoftwareSerial it is 115200 bps
  delay(2000);                                            // Delay it is 2000 ms

  setup_wifi();                                           // Call procedure Setup_wifi()
  //client.setServer(serv, 1883);
  //client.setCallback(callback);

  Tclient.begin();                                        // Initialize time zone
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT +7 = 25200
  Tclient.setTimeOffset(25200);                        // Set offset time zone
}


//====================================================================================================================================================================//
//--------------------------------------------------------------- Function to get data from MQ135 --------------------------------------------------------------------//
//====================================================================================================================================================================//
void loop() {                                             // Loop structure
  //================== Read ADC sensor promini and convertion to ppm co2 =====================//
  //Variable Dekalaration
  float convppm;                                          // Variable declaration that convppm it is float data type
  float co2ppm;                                           // Variable declaration thah co2ppm it is float data type
  float adcMQ135;                                         // Variable decraration it is adcMQ135 it is float data type

  adcMQ135 = port.read();                                 // adcMQ135 function that adc read data from port is using to serial communication ESP with Promini

  //Convertioan fungtion ADC to PPMCO2
  convppm = (adcMQ135 - co2zero);                         // Function convpp
  co2ppm = map(convppm, 0, 1023, 250, 2000);              // Re - maps value "convppm" it is from adc to PPM

  //============================ Moving avarage ADC MQ135 Sensor =============================//
  //Variable declaration
  float average = 0;                                      // Variable declaration avarange it is float data type
  float sum = 0;                                          // Varibale declaration sum it is a float data type
  float data[10];                                         // Variable declaration data it is float data type
  int temporary_sig = 0;                                  // Variable declaration temporary_sig it is a integer daa tpl
  int calculate = 0;                                      // Variable declaration calculate it is a integer data type
  char dataMQ135[20];                                     // variable declaration dataMQ135 it is have a 20 byte arry

  //Smoothing function arduino:
  sum = sum - data[calculate];                            // sum fuction 1
  data[calculate] = co2ppm;                               // Char OF arnnry with buffer data it is 15
  sum = sum + data[calculate];                            // Sum function
  calculate++;                                            // Increment variable "calculate" again and again
  if (calculate < 10 && temporary_sig == 1) {             // Control sturcture if else
    average = sum / 10;                                   // Execute statemnet when condition (calculate<10 && temporary_sig == 1) is TRUE
  } else {                                                // else false
    if (calculate < 10 && temporary_sig == 0) {           // Control structure if else
      average = sum / calculate;                          // Execute statement when condition (calculate<10 && temporary_sig == 0) ir TRUE
    }
  }
  if (calculate >= 10) {                                  // Control structure if else
    temporary_sig = 1;                                    // Execute satetement when condition (calculate>=10) is TRUE
    calculate = 0;                                        //          ---||---
  }

  dtostrf(average, 6, 3, dataMQ135);                      // Float avarange to char array dataMQ135 convertion

  //============================================================================================//

  Tclient.update();                                    // Get valid date and time need update
  //delay(1000);
  if (WiFi.status() == WL_CONNECTED) {                    // Check WiFi connection status if Connected
    if (startUp == 0) {                                  // If value of startUp equal 0
      startUp++;                                         // Increment variable "startUp" again and again
      dataMQ135;                                         // Value char array
      snprintf (msg, 200, "{\"Air Quality\":%s}", dataMQ135);    // Store series dataMQ135 character on buffer "msg" with maxsimum number 200 bytes

      Serial.print("POST Payload :");                     // Show on monitor "POST Payload :"
      Serial.println(msg);                                // Show on monitor value in (msg)

      HTTPClient http;                                    // Declare oject of class HTTPClient
      //http://cloud.thingsboard.io/api/v1/QSz4EEogrsNYXDgl7Qax/telemetry"
      http.begin("http://cloud.thingsboard.io/api/v1/QSz4EEogrsNYXDgl7Qax/telemetry");    // Setup HTTP URL with path token Thingsboard
      http.addHeader("Content-Type", "text/plain");                                       // Define addHeader Content-Type is "text/plain"

      String str(msg);                                    // Send request
      Serial.println(str);                                // Get the respose payload

      int httpCode = http.POST(str);                      // Define int "httpCode" is Methode HTTP POST
      String payload = http.getString();                  // Definne string "payload" is Methode HTTP GET

      if (payload == "-1") {                              // Control structur when condition payload equal to "-1"
        Serial.print("Restart ");                         // Show on monitor "Restrat" when condition TRUE
        Serial.println(". ");                             // Show on monitor " . "
        ESP.restart();                                    // Function to restart ESP8266 device
      } else {                                            // When conditon FALSE
        Serial.println(httpCode);                         // Print HTTP return code
        Serial.println(payload);                          // Print request response payload
      }
      http.end();                                         // Close connection
    } else {
      long now = millis();                                // Declare long now equal to function in miliseconds
      if (now - lastmq > dataMQ135[10]) {                 // Control structure if else when condition (now - lastmq > dataMQ135[10]) is TRUE
        lastmq = now;                                     // Set "lastmq equal to now"

        dataMQ135;                                                // Value char array
        snprintf (msg, 200, "{\"Air Quality\":%s}", dataMQ135);   // Store series dataMQ135 character on buffer "msg" with maxsimum number 200 bytes

        Serial.print("POST Payload :");                     // Show on monitor "POST Payload :"
        Serial.println(msg);                                // Show on monitor value in (msg)

        HTTPClient http;                                    // Declare oject of class HTTPClient
        //My Thingsboard token is "iwt7oimSo7ivAXXRe6dl"
        http.begin("http://demo.thingsboard.io/api/v1/iwt7oimSo7ivAXXRe6dl/telemetry");   // Setup HTTP URL with path token Thingsboard
        http.addHeader("Content-Type", "text/plain");                                     // Define addHeader Content-Type is "text/plain"

        String str(msg);                                    // Send request
        Serial.println(str);                                // Get the respose payload

        int httpCode = http.POST(str);                      // Print HTTP return code
        String payload = http.getString();                  // Print request response payload

        if (payload == "-1") {                              // Jika payload sama dengan 0
          Serial.print("Restart ");                    // Print on monitor "Restrat"
          Serial.println(". ");                        //
          ESP.restart();                               // Function to restart ESP8266 device
        } else {                                        // Jika payload tidak sama dengan 0
          Serial.println(httpCode);                    // Print HTTP return code
          Serial.println(payload);                     // Print request response payload
        }
        http.end();                                         // Close connection

         }
       }
     } else {
    setup_wifi();                                   // Else Callback function "setup_wifi()"
  }
}
