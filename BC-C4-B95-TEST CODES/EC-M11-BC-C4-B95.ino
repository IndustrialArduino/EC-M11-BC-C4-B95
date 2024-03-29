#include <SPI.h>
#include "Adafruit_MAX31855.h"

const int GSM_RST = 32;
const int GSM_PWR_KEY = 22;
const int MODEM_RX = 25;
const int MODEM_TX = 26;

// Pin definitions for the MAX31855 thermocouple sensor
#define MAXDO  19
#define MAXCS   5
#define MAXCLK 18
#define NSS   26

// Pin definitions for additional outputs
#define OUTPUT1 13
#define OUTPUT2 15

Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, MODEM_RX, MODEM_TX);
  pinMode(GSM_RST, OUTPUT);
  pinMode(GSM_PWR_KEY, OUTPUT);
  pinMode(MAXCS, OUTPUT);
  pinMode(NSS, OUTPUT);

  digitalWrite(GSM_RST, HIGH);
  digitalWrite(GSM_PWR_KEY, HIGH);
  digitalWrite(MAXCS, LOW);
  digitalWrite(NSS, HIGH);

  // Modem initialization
  digitalWrite(GSM_RST, HIGH);
  delay(1000);
  digitalWrite(GSM_RST, LOW);
  delay(1000);
  digitalWrite(GSM_RST, HIGH);
  delay(1000);

  Serial.println("SIM AT START >>>>>>>>>>>>>>");

  // Optional: You can add your modem initialization commands here.
  // For example:
  // Serial2.println("AT");
  // delay(2000);
  // ...

  // Additional setup for the MAX31855 thermocouple sensor
  digitalWrite(OUTPUT1, HIGH);
  digitalWrite(OUTPUT2, LOW);
  delay(1000);

  digitalWrite(OUTPUT1, LOW);
  digitalWrite(OUTPUT2, HIGH);

  digitalWrite(OUTPUT1, HIGH);
  delay(500);
  digitalWrite(OUTPUT1, LOW);
  delay(1000);

  digitalWrite(OUTPUT2, HIGH);
  delay(500);
  digitalWrite(OUTPUT2, LOW);
  delay(1000);

  Serial.println("MAX31855 test");
  // Wait for MAX chip to stabilize
  delay(500);
  Serial.print("Initializing sensor...");
  Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);
  if (!thermocouple.begin()) {
    Serial.println("ERROR.");
    while (1) delay(10);
  }

  // Optional: Configure fault checks as desired
  // thermocouple.setFaultChecks(MAX31855_FAULT_OPEN | MAX31855_FAULT_SHORT_VCC);

  Serial.println("DONE.");
}

void loop() {
  // Modem communication
  if (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
  }

  // You can add your modem communication here.
  // For example:
  // if (Serial.available()) {
  //   int inByte = Serial.read();
  //   Serial2.write(inByte);
  // }

  // Thermocouple data
  Serial.print("Internal Temp = ");
  Serial.println(thermocouple.readInternal());

  double c = thermocouple.readCelsius();
  if (isnan(c)) {
    Serial.println("Thermocouple fault(s) detected!");
    uint8_t e = thermocouple.readError();
    if (e & MAX31855_FAULT_OPEN) Serial.println("FAULT: Thermocouple is open - no connections.");
    if (e & MAX31855_FAULT_SHORT_GND) Serial.println("FAULT: Thermocouple is short-circuited to GND.");
    if (e & MAX31855_FAULT_SHORT_VCC) Serial.println("FAULT: Thermocouple is short-circuited to VCC.");
  } else {
    Serial.print("C = ");
    Serial.println(c);
  }

  // Analog reading
  Serial.print("Analog Read : ");
  Serial.print(analogRead(36));

  Serial.println("");
  delay(1000);
}
