/*
Este archivo reemplaza Waqi.ino.

Este código realiza la consulta a una API a elección.
Imprime todo el payload cuando la consulta es exitosa.
Imprime la variables "Status" y espera 60 segundos para volver a realizar la consulta.

Ejemplo creado para el OpenLab "¿Cómo estas mundo?" de Diseño Udesa
Noviembre 2022

*/



//Incluimos las librerias que vamos a utilizar.
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
//

//Definiciones y Variables constantes
#define USE_SERIAL Serial
#define SSID "redwifi"
#define PASSWORD "clave_wifi"
#define DelayApi 60000  //expresado en ms.

//Seleccion de Ciudad
#define Madrid  // Opciones: Buenos_Aires Madrid Barcelona Bogota México_DF Berlin Paris Qatar

//Este define nos permite seleccionar que API queremos Usar. WAQI o OpenWeather
#define OpenWeather

//Segun la api que seleccionemos, tendremos que elegir entre Token(WAQI) o Appid(OpenWeather)
//const String token = "aca va el token que nos da WAQI";
const String appid = "token_API";
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
      //serialData();
      USE_SERIAL.println("Status: " + String(datos.main));
    }
  }
}