// GPS
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Define pins for SoftwareSerial
const int RX_Pin = 9;
const int TX_Pin = 8;

const int GPS_Baud_Rate = 9600;

// Instantiate TinyGPS++ object
TinyGPSPlus gpsModule;

// Create software serial port named "gpsSerialPort"
SoftwareSerial gpsSerialPort(RX_Pin, TX_Pin);

// DHT
#include "DHT.h"

#define DHTPIN 5

#define DHTTYPE DHT22

// THERMOMETRE
#define DELTA_TEMPERATURE 0.7
const int pinMQ2 = A5;
const int seuilGaz = 300;
DHT dht(DHTPIN, DHTTYPE);

// DETECTEUR DE POUSSIERE
#include "SDS011.h"

float p10, p25;
int error;
SDS011 my_sds;

void setup() {

   Serial.begin(9600);

  // Initialize the software serial port at the GPS module's default baud rate
  gpsSerialPort.begin(GPS_Baud_Rate);
  
  dht.begin();

  my_sds.begin(2, 3);

      }

void loop() {

  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println("⟳ Collecte des données de l'Arduino en cours... 🔄");
  Serial.println("🌡️ Température : " + String(dht.readTemperature() - DELTA_TEMPERATURE)+" °C");
  Serial.println("💧 Humidité : " + String(dht.readHumidity())+" %");
  delay(1000);
  int valeurGaz = analogRead(pinMQ2);

  // Afficher la valeur lue sur le moniteur série
  Serial.println("💨 Lecture du capteur MQ-2 en cours...");
  
  // Vérifier si la valeur dépasse le seuil
  if (valeurGaz > seuilGaz) {
    Serial.println("⚠️ Seuil de gaz à combustible ou fumée anormal !");
  } else {
    Serial.println("✅ Seuil de gaz à combustible ou fumée normal.");
    Serial.println(valeurGaz);
  }

  error = my_sds.read(&p25, &p10);
  if (!error) {
    Serial.println("🍃 Taux de Particules < 2,5 µm :" + String(p25) + " μg/m³");
    Serial.println("🚗 Taux de particules > 2,5 µm  " + String(p10) + " μg/m³");
      
  }
  // Process incoming GPS data
  while (gpsSerialPort.available() > 0)
    if (gpsModule.encode(gpsSerialPort.read()))
  

  // Check if no GPS data has been received for 5 seconds
  if (millis() > 5000 && gpsModule.charsProcessed() < 10)
  {
    Serial.println("Error: GPS module not detected");
  }

  Serial.println("Données du GPS 🪐 :");
  if (gpsModule.location.isValid())
  {
    Serial.println("Latitude: ");
    Serial.println(gpsModule.location.lat(), 6); //print latitude
    Serial.println("Longitude: ");
    Serial.println(gpsModule.location.lng(), 6); //print longitude
    Serial.println("Altitude: ");
    Serial.println(gpsModule.altitude.meters()); //print altitude
  }
  else
  {
    Serial.println("❌ Localisation Introuvable");
  }

  Serial.print("📅 Date : ");
  if (gpsModule.date.isValid())
  {
    Serial.println(gpsModule.date.day());
    Serial.println("/");
    Serial.println(gpsModule.date.month()); 
    Serial.println("/");
    Serial.println(gpsModule.date.year());
  }
  else
  {
    Serial.println("❌ Date introuvable");
  }

delay(3000);
  }
  