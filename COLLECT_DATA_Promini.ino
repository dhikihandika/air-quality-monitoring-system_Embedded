//--------------------------------------------------------------------------------//
//------ Inisialisasi Program komunikasi serial antara Promini dengan ESP  -------//
//--------------------------------------------------------------------------------//

#include <SoftwareSerial.h>     // Menambahkan library SoftwareSerial
SoftwareSerial yeah(12,10);     // Pin RX, TX Promini 
// Deklarasi variabel sensor adalah integer, variabel bb adalah port analog output
// (A0) Promini
int sensor;                        
int bb = A0;  


//--------------------------------------------------------------------------------//                                
//-------------------- Setup kecepatan data rate serial (bps) --------------------//
//--------------------------------------------------------------------------------//
void setup(){
  yeah.begin(115200);
  Serial.begin(115200);
  }


//--------------------------------------------------------------------------------//
//--------------------- Komunikasi Serial Promini dengan ESP ---------------------//


//--------------------------------------------------------------------------------//
void loop() {
sensor = analogRead(bb);        // Membaca data analog sensor 
// statemenet jika port komunikasi serial (12, 10) promini adalah 0 (nol)
if(yeah.available()==0){ 
  yeah.write(sensor);           // Tulis and send data sensor from Promini to ESP
  Serial.print("Collect Data Sensor");
  Serial.print(" = ");
  Serial.println(sensor);       // Print data sensor yang dibaca
  }
}
