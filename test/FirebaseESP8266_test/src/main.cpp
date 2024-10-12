#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>

#define WIFI_SSID "Wifi router"
#define WIFI_PASSWORD "password"
#define DATABASE_URL "test-db-30431-default-rtdb.asia-southeast1.firebasedatabase.app"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
unsigned long count = 0;

long now = 0;
long after = 0;

void setup()
{
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.signer.test_mode = true;
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = "bAaDbqR7yIv0A6WFyGEoqyaDmq6OGN0btkAD9IzZ";

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);
}

void loop()
{
  // put your main code here, to run repeatedly:
  now = millis();
  Firebase.setString(fbdo, F("/test/string"), "Hello World!") ? "ok" : fbdo.errorReason().c_str();
  after = millis();
  Serial.println("Time taken to send data: " + String(after - now) + "ms");
  now = millis();
  Firebase.setString(fbdo, F("/test/string"), "Good Morning!") ? "ok" : fbdo.errorReason().c_str();
  after = millis();
  Serial.println("Time taken to send data: " + String(after - now) + "ms");
  now = millis();
  Serial.printf("Get string... %s\n", Firebase.getString(fbdo, F("/test/string")) ? fbdo.to<const char *>() : fbdo.errorReason().c_str());
  after = millis();
  Serial.println("Time taken to recieve data: " + String(after - now) + "ms");
}
