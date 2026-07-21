#include <WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <DHTesp.h>

// ---------------- PINS ----------------
#define GAS_PIN     14
#define DHT_PIN     12
#define PIR_PIN     13
#define BUZZER_PIN  15
#define LED_RED     17
#define LED_YELLOW  16
#define LED_GREEN   2
#define RELAY_PIN   21

#define SS_PIN      5
#define RST_PIN     22

// ---------------- OBJECTS ----------------
DHTesp dhtSensor;
MFRC522 rfid(SS_PIN, RST_PIN);

// ---------------- SETTINGS ----------------
const int GAS_LOW = 1200;
const int GAS_MEDIUM = 2200;
const int GAS_HIGH = 3000;

bool authorized = false;
unsigned long authTime = 0;
const unsigned long AUTH_WINDOW = 15000;

// Replace with your own RFID UID after first scan
String validUID = "DE AD BE EF";

void setup() {
  Serial.begin(115200);

  pinMode(GAS_PIN, INPUT);
  pinMode(PIR_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);

  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);

  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);

  SPI.begin();
  rfid.PCD_Init();

  Serial.println("Industrial Safety System Starting...");
  Serial.println("Scan RFID card to authorize access.");
}

void loop() {
  checkRFID();

  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  int gasValue = analogRead(GAS_PIN);
  int motion = digitalRead(PIR_PIN);

  bool dhtOk = !(isnan(data.temperature) || isnan(data.humidity));

  if (authorized && (millis() - authTime > AUTH_WINDOW)) {
    authorized = false;
    Serial.println("Authorization expired.");
  }

  int riskLevel = 0;

  if (gasValue > GAS_LOW) riskLevel = 1;
  if (gasValue > GAS_MEDIUM) riskLevel = 2;
  if (gasValue > GAS_HIGH) riskLevel = 3;

  if (motion == HIGH && gasValue > GAS_MEDIUM) {
    riskLevel = 3;
  }

  if (authorized && motion == HIGH && gasValue > GAS_LOW) {
    riskLevel = 2;
  }

  updateOutputs(riskLevel);

  Serial.println("------ SAFETY STATUS ------");
  if (dhtOk) {
    Serial.print("Temperature: ");
    Serial.print(data.temperature);
    Serial.println(" C");

    Serial.print("Humidity: ");
    Serial.print(data.humidity);
    Serial.println(" %");
  } else {
    Serial.println("DHT22 read failed");
  }

  Serial.print("Gas Value: ");
  Serial.println(gasValue);

  Serial.print("Motion: ");
  Serial.println(motion ? "Detected" : "Not Detected");

  Serial.print("RFID Authorized: ");
  Serial.println(authorized ? "YES" : "NO");

  Serial.print("Risk Level: ");
  if (riskLevel == 0) Serial.println("SAFE");
  else if (riskLevel == 1) Serial.println("LOW");
  else if (riskLevel == 2) Serial.println("MEDIUM");
  else Serial.println("HIGH");

  Serial.println("---------------------------");
  delay(1000);
}

void updateOutputs(int riskLevel) {
  switch (riskLevel) {
    case 0:
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_RED, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(RELAY_PIN, LOW);
      break;

    case 1:
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, HIGH);
      digitalWrite(LED_RED, LOW);
      digitalWrite(BUZZER_PIN, LOW);
      digitalWrite(RELAY_PIN, LOW);
      break;

    case 2:
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, HIGH);
      digitalWrite(LED_RED, LOW);
      beepShort();
      digitalWrite(RELAY_PIN, LOW);
      break;

    case 3:
      digitalWrite(LED_GREEN, LOW);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_RED, HIGH);
      beepAlert();
      digitalWrite(RELAY_PIN, HIGH);
      break;
  }
}

void beepShort() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(100);
  digitalWrite(BUZZER_PIN, LOW);
}

void beepAlert() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
  delay(100);
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW);
}

void checkRFID() {
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  String uidString = "";
  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) uidString += "0";
    uidString += String(rfid.uid.uidByte[i], HEX);
    if (i < rfid.uid.size - 1) uidString += " ";
  }
  uidString.toUpperCase();

  Serial.print("Card UID: ");
  Serial.println(uidString);

  if (uidString == validUID) {
    authorized = true;
    authTime = millis();
    Serial.println("Authorized card detected.");
  } else {
    authorized = false;
    Serial.println("Unauthorized card.");
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}