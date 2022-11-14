/*

imprimo datos del getdata en este hilo...
importo lib servo
muevo el servo en función de la temperatura de Madrid
el timer de la llamada detiene el movimiento
*/

//Incluimos las librerias que vamos a utilizar.
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>

//
//Definiciones y Variables constantes

#define USE_SERIAL Serial
#define SSID "frk2018--"
#define PASSWORD "100381192"

//Seleccion de Ciudad
#define Madrid  // Buenos_Aires Madrid Barcelona Bogota México_DF Berlin Paris Qatar

//const String token = "fee493e712c89a1474c13ee3c388c338440e2a6";
const String appid = "2c5d39580bce43c97cd49465e0988434";


#define OpenWeather  //WAQI o OpenWeather

#include "api.h"

//


//Constructores
WiFiMulti wifiMulti;

//Definiciones del servomotor
Servo myservo;
int pos = 0;

#if defined(ARDUINO_ESP32S2_DEV)
int servoPin = 17;
#else
int servoPin = 18;
#endif
//Fin definiciones servomotor


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

  //Inicialización servomotor...
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);            // standard 50 hz servo
  myservo.attach(servoPin, 1000, 2000);  // attaches the servo on pin 18 to the servo object
                                         // using default min/max of 1000us and 2000us
                                         // different servos may require different min/max settings
                                         // for an accurate 0 to 180 sweep
}

void loop() {
  // wait for WiFi connection
  if ((wifiMulti.run() == WL_CONNECTED)) {
    getData();
  }
  //serialData();
  USE_SERIAL.println("Datos Temp: " + String(datos.temp));
  delay(10000);

  myservo.write(datos.temp);


  //Movimiento servomotor
  if (datos.temp < 10) {
    for (pos = 0; pos <= 180; pos += 1) {
      // in steps of 1 degree
      myservo.write(pos);
      delay(15);
    }
    for (pos = 180; pos >= 0; pos -= 1) {
      myservo.write(pos);
      delay(15);
    }
  } else {
    for (pos = 0; pos <= 180; pos += 30) {
      // in steps of 1 degree
      myservo.write(pos);
      delay(15);
    }
    for (pos = 180; pos >= 0; pos -= 30) {
      myservo.write(pos);
      delay(15);
    }
  }
}