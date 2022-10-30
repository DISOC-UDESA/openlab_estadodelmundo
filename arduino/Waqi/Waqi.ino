//

//Incluimos las librerias que vamos a utilizar.
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>

//
//Definiciones y Variables constantes

#define USE_SERIAL Serial
#define SSID "frk2018--"
#define PASSWORD "100381192"

//Seleccion de Ciudad
#define Madrid  // Buenos_Aires Madrid Barcelona Bogota México_DF Berlin Paris Qatar

//const String token = "fee493e712c89a1474c13ee3c388c338440e2a6";
const String appid = "2c5d39580bce43c97cd49465e0988434";


#define OpenWeather //WAQI o OpenWeather

#include "api.h"

//


//Constructores
WiFiMulti wifiMulti;



void setup() {

  USE_SERIAL.begin(115200);

  USE_SERIAL.println();
  USE_SERIAL.println();
  USE_SERIAL.println();

  for (uint8_t t = 4; t > 0; t--) {
    USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
    USE_SERIAL.flush();
    delay(1000);
  }
  wifiMulti.addAP(SSID, PASSWORD);
}

void loop() {
  // wait for WiFi connection
  if ((wifiMulti.run() == WL_CONNECTED)) {
    getData();
  }
  serialData();
  delay(60000);
}