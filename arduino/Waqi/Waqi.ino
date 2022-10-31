//Incluimos las librerias que vamos a utilizar.
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
//

//Definiciones y Variables constantes
#define USE_SERIAL Serial
#define SSID "aqui va el nombre de la red wifi"
#define PASSWORD "aca va la password"
#define DelayApi 60000  //expresado en ms.

//Seleccion de Ciudad
#define Madrid  // Opciones: Buenos_Aires Madrid Barcelona Bogota México_DF Berlin Paris Qatar

//Este define nos permite seleccionar que API queremos Usar. WAQI o OpenWeather
#define OpenWeather

//Segun la api que seleccionemos, tendremos que elegir entre Token(WAQI) o Appid(OpenWeather)
//const String token = "aca va el token que nos da WAQI";
const String appid = "aca va el appid que nos da openweather";
//---------------------------------------------------------------//


#include "api.h"


//Constructores
WiFiMulti wifiMulti;

unsigned long tiempo_ultima_llamada = 0;

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

  if (tiempo_ultima_llamada + DelayApi < millis()) {
    if ((wifiMulti.run() == WL_CONNECTED)) {
      getData();
      tiempo_ultima_llamada = millis();
    }
  }
  serialData();
}