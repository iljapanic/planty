#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <FirebaseArduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define WIFI_SSID "IAAC-WIFI"
#define WIFI_PASSWORD "enteriaac2013"
#define FIREBASE_HOST "mdef-planty.firebaseio.com"
#define FIREBASE_AUTH ""
#define NTP_OFFSET   60 * 60      // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "europe.pool.ntp.org"

String pathHumidityPlant1 = "plant1/humidity";

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);


void setup() {
  Serial.begin(115200);
  timeClient.begin();
  setup_wifi();
  connectFirebase();
}

void loop() {

  timeClient.update();

  if (WiFi.status() != WL_CONNECTED) {
    delay(500);
    setup_wifi();
  }


  int rawMoisture = analogRead(A0);

  sendPlantData(pathHumidityPlant1, rawMoisture);


  delay(5000); // send data every 5 seconds
}


void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void sendPlantData (String path, int sensorValue) {
  // get a timestamp
  long rawTime = timeClient.getEpochTime();
  String timestamp = String(rawTime);

  // full path
  String setPath = path + "/" + timestamp;
  
  
  Firebase.setInt(setPath, sensorValue);
  
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
}

void connectFirebase() {
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
