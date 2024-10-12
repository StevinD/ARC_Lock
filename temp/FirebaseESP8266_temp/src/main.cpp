#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>
#include <SPI.h>
#include <MFRC522.h>

//hidden code id : 1
#define WIFI_SSID ""
#define WIFI_PASSWORD ""
#define DATABASE_URL ""

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

const int relayPin = D2;
const int buzzerPin = D1;

const int wifiWaitTime = 15000;

#define SS_PIN D4
#define RST_PIN -1
MFRC522 rfid(SS_PIN, RST_PIN);

String cardID = "";

void beep(int type)
{
  if (type == 1)
  {
    digitalWrite(buzzerPin, HIGH);
    delay(250);
    digitalWrite(buzzerPin, LOW);
    delay(250);
    digitalWrite(buzzerPin, HIGH);
    delay(250);
    digitalWrite(buzzerPin, LOW);
    delay(250);
  }
  else if (type == 2)
  {
    digitalWrite(buzzerPin, HIGH);
    delay(500);
    digitalWrite(buzzerPin, LOW);
    delay(500);
  }
}
void setup()
{
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  int i = 0;
  while (WiFi.status() != WL_CONNECTED && i < wifiWaitTime)
  {
    Serial.print(".");
    delay(300);
    i += 300;
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println();
    Serial.print("Connected with IP: ");
    Serial.println(WiFi.localIP());
    Serial.println();
  }
  if (i >= wifiWaitTime)
  {
    Serial.println();
    Serial.println("Connection failed");
    Serial.println();
  }

  config.signer.test_mode = true;
  config.database_url = DATABASE_URL;
  //hiden code id : 2
  config.signer.tokens.legacy_token = "";

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);

  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  SPI.begin();     // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 module
  Serial.println("Scan an RFID tag...");
}

void loop()
{
  cardID = "";
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial())
  {
    // Convert UID to a string
    Serial.print("Card UID: ");
    for (byte i = 0; i < rfid.uid.size; i++)
    {
      // Add each byte to the cardID string in HEX format
      cardID += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
      cardID += String(rfid.uid.uidByte[i], HEX);
    }
    Serial.println(cardID); // Print the card ID stored in the string
    if (cardID == "43a20d2e" || cardID == "a1c79100" || cardID == "c9d19100" || cardID == "73f09100" || cardID == "734d6030" || cardID == "f31df106")
    {
      digitalWrite(relayPin, HIGH);
      beep(1);
      delay(3000);
      digitalWrite(relayPin, LOW);
    }
    else
    {
      String access = Firebase.getString(fbdo, "access/" + cardID + "/status") ? fbdo.to<const char *>() : fbdo.errorReason().c_str();
      Serial.println(access);

      if (access == "granted")
      {
        digitalWrite(relayPin, HIGH);
        beep(1);
        delay(3000);
        digitalWrite(relayPin, LOW);
      }
      else
      {
        if (access == "path not exist")
        {
          Firebase.setString(fbdo, "access/" + cardID + "/status", "denied") ? "ok" : fbdo.errorReason().c_str();
          Firebase.setString(fbdo, "access/" + cardID + "/Name", "Guest") ? "ok" : fbdo.errorReason().c_str();
        }
        beep(2);
      }
    }
    // Halt PICC and stop encryption on PCD
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
  String lock = Firebase.getString(fbdo, "lock") ? fbdo.to<const char *>() : fbdo.errorReason().c_str();
  if (lock == "open")
  {
    digitalWrite(relayPin, HIGH);
    beep(1);
    delay(3000);
    digitalWrite(relayPin, LOW);
    Firebase.setString(fbdo, "lock", "close") ? "ok" : fbdo.errorReason().c_str();
  }
}
