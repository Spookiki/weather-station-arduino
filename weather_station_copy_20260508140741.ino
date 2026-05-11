// ============================================================
//  INCLUDES
// ============================================================
#include "DHT.h"
#include "SDS011.h"

// ============================================================
//  CONFIGURATION — TEMPERATURE / HUMIDITE (DHT22)
// ============================================================
#define DHTPIN  5
#define DHTTYPE DHT22
#define DELTA_TEMPERATURE 0.7f

DHT dht(DHTPIN, DHTTYPE);

// ============================================================
//  CONFIGURATION — GAZ (MQ-2)
// ============================================================
const int pinMQ2   = A5;
const int seuilGaz = 300;

// ============================================================
//  CONFIGURATION — POUSSIERE (SDS011)
// ============================================================
float p25, p10;
int   sdsError;
SDS011 my_sds;

// ============================================================
//  SETUP
// ============================================================
void setup() {
    Serial.begin(9600);
    dht.begin();
    my_sds.begin(2, 3);
}

// ============================================================
//  LOOP
// ============================================================
void loop() {
    float temp = dht.readTemperature() - DELTA_TEMPERATURE;
    float hum  = dht.readHumidity();
    int   gaz  = analogRead(pinMQ2);

    sdsError = my_sds.read(&p25, &p10);

    // Construction du JSON sur une seule ligne
    Serial.print("{");
    Serial.print("\"temp\":");   Serial.print(temp, 1);
    Serial.print(",\"hum\":");   Serial.print(hum, 1);
    Serial.print(",\"gaz\":");   Serial.print(gaz);
    Serial.print(",\"gaz_ok\":"); Serial.print(gaz <= seuilGaz ? "true" : "false");

    if (!sdsError) {
        Serial.print(",\"pm25\":"); Serial.print(p25, 1);
        Serial.print(",\"pm10\":"); Serial.print(p10, 1);
    } else {
        Serial.print(",\"pm25\":null");
        Serial.print(",\"pm10\":null");
    }

    Serial.println("}");

    delay(2000);
}