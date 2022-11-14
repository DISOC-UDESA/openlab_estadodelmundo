//Incluimos las librerias que vamos a utilizar.
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <ESP32Servo.h>

//

//Definiciones y Variables constantes
#define USE_SERIAL Serial
#define SSID "Red Alumnos"
#define PASSWORD ""
#define DelayApi 60000  //expresado en ms.

//Seleccion de Ciudad
#define Madrid  // Opciones: Buenos_Aires Madrid Barcelona Bogota México_DF Berlin Paris Qatar

//Este define nos permite seleccionar que API queremos Usar. WAQI o OpenWeather
#define OpenWeather

//Segun la api que seleccionemos, tendremos que elegir entre Token(WAQI) o Appid(OpenWeather)
//const String token = "aca va el token que nos da WAQI";
const String appid = "2c5d39580bce43c97cd49465e0988434";
//---------------------------------------------------------------//


#include "api.h"


//Constructores
WiFiMulti wifiMulti;

Servo myservo;
int pos = 0;
int targetpos = 0;
int delayservo = 0;
#if defined(ARDUINO_ESP32S2_DEV)
int servoPin = 17;
#else
int servoPin = 18;
#endif

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

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);  // standard 50 hz servo
  myservo.attach(servoPin, 1000, 2000);
}

void loop() {

  if (tiempo_ultima_llamada + DelayApi < millis()) {
    if ((wifiMulti.run() == WL_CONNECTED)) {
      getData();
      tiempo_ultima_llamada = millis();
      USE_SERIAL.println("Velocidad viento: " + String(datos.wind_speed));
      // serialData();
    }
    if (datos.wind_speed > 9) {
      targetpos = 40;
      delayservo = 1;
    } else {
      targetpos = 180;
      delayservo = 15;
    }
  }
  for (pos = 0; pos <= targetpos; pos += 1) {  // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);  // tell servo to go to position in variable 'pos'
    delay(delayservo);           // waits 15ms for the servo to reach the position
  }
  for (pos = targetpos; pos >= 0; pos -= 1) {  // goes from 180 degrees to 0 degrees
    myservo.write(pos);                        // tell servo to go to position in variable 'pos'
    delay(delayservo);                                 // waits 15ms for the servo to reach the position
  }
}