#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <MFRC522.h>
#include <SPI.h>
#include <SoftwareSerial.h>

#define BUZZER_PIN 3
#define SERVO_PIN 4
#define ESP_RX_PIN 0
#define ESP_TX_PIN 1

#define GATE_OPEN_ANGLE 0
#define GATE_CLOSE_ANGLE 90
#define GATE_STEP_DELAY 10 // Delay between each step (in milliseconds)

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo tollGate;
MFRC522 mfrc522(10, 9); // Define the pins for RC522
SoftwareSerial espSerial(ESP_RX_PIN, ESP_TX_PIN); // RX, TX

// Structure to store RFID UID and name
struct RFIDCard {
  byte uid[4];
  String name;
};

// Registered RFID cards with their UIDs and names
RFIDCard registeredCards[5] = {
  {{0x43, 0xA2, 0x0D, 0xC9}, "UP1E239"}, 
  {{0x43, 0x90, 0xB6, 0x11}, "HR24144"}, 
  {{0x63, 0x67, 0xA8, 0x12}, "DL8C503"},
  {{0x41, 0x1C, 0x97, 0x0A}, "PB7B325"},
  {{0xA1, 0xA9, 0xB7, 0x2C}, "OD7Q761"}
};

void setup() {
  Serial.begin(115200);
  espSerial.begin(115200); // UART communication with ESP32
  pinMode(BUZZER_PIN, OUTPUT);
  tollGate.attach(SERVO_PIN);
  lcd.init();
  lcd.backlight();
  SPI.begin();
  mfrc522.PCD_Init(); // Initialize MFRC522
  
  // Display default message initially
  lcd.clear();
  lcd.print("Welcome to RKGIT");
  lcd.setCursor(0, 1);
  lcd.print("Toll station");
}

void loop() {
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String uidStr = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (mfrc522.uid.uidByte[i] < 0x10) {
        uidStr += "0";
      }
      uidStr += String(mfrc522.uid.uidByte[i], HEX);
    }
    uidStr.toUpperCase(); // Convert UID to uppercase
    Serial.print("UID: ");
    Serial.println(uidStr);

    if (isValidUID(uidStr)) {
      String name = checkAuthorization();
      if (name != "") {
        lcd.clear();
        lcd.print("Vehicle");
        lcd.setCursor(0, 1);
        lcd.print(name + " detected");
        buzzerBeep(2, 100);
        operateGate(GATE_OPEN_ANGLE);
        sendDataToESP(uidStr); // Send UID to ESP32
        delay(3000);
        operateGate(GATE_CLOSE_ANGLE); // Close gate after successful scan
      } else {
        lcd.clear();
        lcd.print("Invalid Attempt");
        buzzerBeep(1, 500);
      }
    } else {
      Serial.println("Invalid UID format");
    }
    delay(3000); // Delay before resetting display
    displayDefaultMessageOnLCD(); // Reset display
    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }
}

void operateGate(int targetAngle) {
  int currentAngle = tollGate.read();
  int step = (currentAngle < targetAngle) ? 1 : -1; // Determine step direction
  for (int angle = currentAngle; angle != targetAngle; angle += step) {
    tollGate.write(angle);
    delay(GATE_STEP_DELAY);
  }
  tollGate.write(targetAngle);
}

String checkAuthorization() {
  for (int i = 0; i < 5; i++) {
    bool match = true;
    for (int j = 0; j < 4; j++) {
      if (mfrc522.uid.uidByte[j] != registeredCards[i].uid[j]) {
        match = false;
        break;
      }
    }
    if (match) {
      return registeredCards[i].name;
    }
  }
  return "";
}

bool isValidUID(String uid) {
  if (uid.length() != 8) { // Check if the length of the UID is not 8 characters
    return false;
  }
  for (int i = 0; i < uid.length(); i++) {
    if (!isHexadecimalDigit(uid.charAt(i))) {
      return false;
    }
  }
  return true;
}

void buzzerBeep(int count, int duration) {
  for (int i = 0; i < count; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(duration);
    digitalWrite(BUZZER_PIN, LOW);
    delay(duration);
  }
}

int sNo = 1; // Initialize the serial number counter

void sendDataToESP(String uid) {
  String jsonData = "{\"s.no\":\"" + String(sNo) + "\",\"uid\":\"" + uid + "\",\"debit\":\"-50\"}\n";
  espSerial.print(jsonData);
  Serial.print("Sending to ESP: ");
  Serial.println(jsonData);

  // Increment serial number for the next scan
  sNo++;
}

void displayDefaultMessageOnLCD() {
  lcd.clear();
  lcd.print("Welcome to RKGIT");
  lcd.setCursor(0, 1);
  lcd.print("Toll station");
}