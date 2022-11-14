#include <ArduinoJson.h>

#ifdef WAQI
class Api {
public:
  float aqi;         //Calidad de Aire.
  float index;       //Index en el que se encuentra.
  float pm25;        //particulas menores a 2.5 micrones.
  float pm10;        //particulas menores a 10 micrones.
  float hum;         //Humeda ambiente.
  float presion;     //Presion Barometrica.
  float temp;        //Temperatura.
  float co;          //monoxido de carbono.
  float o3;          //Ozono.
  float no2;         //Oxido de nitrogeno.
  float so2;         //Oxido de Sulfuro.
  float wind_speed;  //Velocidad del Viento
  float wind_deg;    //Grados  del Viento
  float dew;         //?????????
  float rain;        //Lluvia
};
#endif

#ifdef OpenWeather
class Api {
public:
  const char* main;
  const char* description;
  float temp;
  float feels;
  float temp_min;
  float temp_max;
  float presion;
  float hum;
  float visibilidad;
  float wind_speed;
  float wind_deg;
  float clouds;
};
#endif

//Define de Ciudades
#ifdef WAQI
#ifdef Buenos_Aires
String city = "argentina/buenos-aires/us-embassy";
#endif
#ifdef Madrid
String city = "madrid/";
#endif
#ifdef Barcelona
String city = "spain/catalunya/barcelona-eixample/";
#endif
#ifdef Bogota
String city = "colombia/bogota/us-consulate/";
#endif
#ifdef Mexico_DF
String city = "mexico/mexico/merced/";
#endif
#ifdef Berlin
String city = "germany/berlin/mitte--bruckenstrasse/";
#endif
#ifdef Paris
String city = "paris/";
#endif
#ifdef Qatar
String city = "qatar/doha/al-rayyan/";
#endif
#endif

#ifdef OpenWeather
#ifdef Buenos_Aires
String city = "3433955";
#endif
#ifdef Madrid
String city = "6355233";
#endif
#ifdef Barcelona
String city = "6356055";
#endif
#ifdef Bogota
String city = "3688689";
#endif
#ifdef Mexico_DF
String city = "3996063";
#endif
#ifdef Berlin
String city = "2950159";
#endif
#ifdef Paris
String city = "6455259";
#endif
#ifdef Qatar
String city = "289688";
#endif
#endif

Api datos;

#ifdef WAQI
void getData() {
  String payload;
  HTTPClient http;

  USE_SERIAL.print("[HTTP] begin...\n");
  String url = "https://api.waqi.info/feed/" + city + "?token=c" + token;
  http.begin(url);  //HTTP

  USE_SERIAL.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      payload = http.getString();
      USE_SERIAL.println(payload);
    }
  } else {
    USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  DynamicJsonDocument doc(4096);

  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    USE_SERIAL.print("deserializeJson() failed: ");
    USE_SERIAL.println(error.c_str());
    return;
  }

  datos.aqi = -1;
  datos.index = -1;
  datos.pm25 = -1;
  datos.pm10 = -1;
  datos.hum = -1;
  datos.presion = -1;
  datos.temp = -1;
  datos.co = -1;
  datos.o3 = -1;
  datos.no2 = -1;
  datos.so2 = -1;
  datos.wind_speed = -1;
  datos.wind_deg = -1;
  datos.dew = -1;
  datos.rain = -1;

  JsonObject data = doc["data"];
  datos.aqi = data["aqi"];
  datos.index = data["idx"];
  const char* data_dominentpol = data["dominentpol"];  // "pm25"

  for (JsonPair data_iaqi_item : data["iaqi"].as<JsonObject>()) {
    String data_iaqi_item_key = data_iaqi_item.key().c_str();  // "co", "h", "no2", "o3", "p", "pm10", ...
    if (data_iaqi_item_key == "pm25") {
      datos.pm25 = data_iaqi_item.value()["v"];
    }
    if (data_iaqi_item_key == "pm10") {
      datos.pm10 = data_iaqi_item.value()["v"];
    }
    if (data_iaqi_item_key == "h") {
      datos.hum = data_iaqi_item.value()["v"];
    }
    if (data_iaqi_item_key == "p") {
      datos.presion = data_iaqi_item.value()["v"];
    }
    if (data_iaqi_item_key == "t") {
      datos.temp = data_iaqi_item.value()["v"];
    }
    if (data_iaqi_item_key == "co") {
      datos.co = data_iaqi_item.value()["v"];
    }
    if (data_iaqi_item_key == "o3") {
      datos.o3 = data_iaqi_item.value()["v"];
    }
    if (data_iaqi_item_key == "no2") {
      datos.no2 = data_iaqi_item.value()["v"];
    }
    if (data_iaqi_item_key == "so2") {
      datos.so2 = data_iaqi_item.value()["v"];
    }
    if (data_iaqi_item_key == "w") {
      datos.wind_speed = data_iaqi_item.value()["v"];
    }
    if (data_iaqi_item_key == "wd") {
      datos.wind_deg = data_iaqi_item.value()["v"];
    }
    if (data_iaqi_item_key == "r") {
      datos.rain = data_iaqi_item.value()["v"];
    }
  }
}
#endif

#ifdef OpenWeather
void getData() {

  String payload;
  HTTPClient http;

  USE_SERIAL.print("[HTTP] begin...\n");
  String url = "https://api.openweathermap.org/data/2.5/weather?id=" + city + "&appid=" + appid + "&units=metric";
  http.begin(url);  //HTTP

  USE_SERIAL.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      payload = http.getString();
      USE_SERIAL.println(payload);
    }
  } else {
    USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();

  datos.main = "";
  datos.description = "";
  datos.temp = -1;
  datos.feels = -1;
  datos.temp_min = -1;
  datos.temp_max = -1;
  datos.presion = -1;
  datos.hum = -1;
  datos.visibilidad = -1;
  datos.wind_speed = -1;
  datos.wind_deg = -1;
  datos.clouds = -1;


  StaticJsonDocument<1024> doc;

  DeserializationError error = deserializeJson(doc, payload);

  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  JsonObject weather_0 = doc["weather"][0];
  datos.main = weather_0["main"];
  datos.description = weather_0["description"];

  JsonObject main = doc["main"];
  datos.temp = main["temp"];
  datos.feels = main["feels_like"];
  datos.temp_min = main["temp_min"];
  datos.temp_max = main["temp_max"];
  datos.presion = main["pressure"];
  datos.hum = main["humidity"];
  datos.visibilidad = doc["visibility"];

  JsonObject wind = doc["wind"];
  datos.wind_speed = wind["speed"];
  datos.wind_deg = wind["deg"];
  datos.clouds = doc["clouds"]["all"];
}
#endif

void serialData() {
#ifdef USE_SERIAL
#ifdef WAQI
  USE_SERIAL.println("aqi: " + String(datos.aqi, 2));
  USE_SERIAL.println("index: " + String(datos.index, 2));
  USE_SERIAL.println("pm25: " + String(datos.pm25, 2));
  USE_SERIAL.println("pm10: " + String(datos.pm10, 2));
  USE_SERIAL.println("hum: " + String(datos.hum, 2));
  USE_SERIAL.println("presion: " + String(datos.presion, 2));
  USE_SERIAL.println("temp: " + String(datos.temp, 2));
  USE_SERIAL.println("co: " + String(datos.co, 2));
  USE_SERIAL.println("o3: " + String(datos.o3, 2));
  USE_SERIAL.println("no2: " + String(datos.no2, 2));
  USE_SERIAL.println("so2: " + String(datos.so2, 2));
  USE_SERIAL.println("WindSpeed: " + String(datos.wind_speed, 2));
  USE_SERIAL.println("wind_deg: " + String(datos.wind_deg, 2));
  USE_SERIAL.println("dew: " + String(datos.dew, 2));
  USE_SERIAL.println("rain: " + String(datos.rain, 2));
#endif
#ifdef OpenWeather
  USE_SERIAL.println("Status: " + String(datos.main));
  USE_SERIAL.println("Description: " + String(datos.description));
  USE_SERIAL.println("temp: " + String(datos.temp, 2));
  USE_SERIAL.println("Feelsliek: " + String(datos.feels, 2));
  USE_SERIAL.println("Temp Min: " + String(datos.temp_min, 2));
  USE_SERIAL.println("Temp Max: " + String(datos.temp_max, 2));
  USE_SERIAL.println("Presion: " + String(datos.presion, 2));
  USE_SERIAL.println("Hum: " + String(datos.hum, 2));
  USE_SERIAL.println("Visibilidad: " + String(datos.visibilidad, 2));
  USE_SERIAL.println("Velocidad Viento: " + String(datos.wind_speed, 2));
  USE_SERIAL.println("Grados Viento: " + String(datos.wind_deg, 2));
  USE_SERIAL.println("Nubosidad: " + String(datos.clouds, 2));
#endif
#endif
}