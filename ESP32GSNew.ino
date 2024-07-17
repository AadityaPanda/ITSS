#include <WiFi.h>
#include <HTTPClient.h>
#include <TimeLib.h>
#include <time.h>

const char* ssid = "vivo v27";
const char* password = "drfb1057";
const char* serverName = "https://script.google.com/macros/s/AKfycbxnleuVoQN6ugaWfUS5PSi0GnD45GMZ9K5FXKe8x0h8pIGHA_ZFRDYhdir8dG-oCgZsrQ/exec";

unsigned long lastRFIDTime = 0;
const unsigned long RFIDInterval = 1000; // Debouncing interval in milliseconds

// Counter for Serial No.
int serialNo = 0;

void setup() {
  Serial.begin(115200);   // Debugging with the computer
  Serial2.begin(115200, SERIAL_8N1, 16, 17);  // Communication with Arduino Uno (RX, TX)
  connectToWiFi();
  // Configure time with the correct time zone offset
  configTime(5 * 3600 + 30 * 60, 0, "pool.ntp.org");
  delay(2000);  // Wait for time synchronization

  // Debug time synchronization
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    Serial.println(&timeinfo, "Time synchronized: %Y-%m-%d %H:%M:%S");
  } else {
    Serial.println("Failed to synchronize time");
  }
}

void loop() {
  if (Serial2.available() > 0) {
    String input = Serial2.readStringUntil('\n');
    input.trim();
    input.toUpperCase(); // Convert input to uppercase
    Serial.print("Received input: ");
    Serial.println(input); // Debugging line

    // Extract UID from input if it starts with "UID: "
    String uid;
    if (input.startsWith("UID: ")) {
      uid = input.substring(5);
    } else {
      uid = input;
    }

    // Validate RFID data (length check)
    if (uid.length() == 8) {
      sendToGoogleSheets(uid);
      lastRFIDTime = millis();
    } else {
      Serial.println("Invalid RFID data");
    }
  }
}

void connectToWiFi() {
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void sendToGoogleSheets(String uid) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverName);
    http.addHeader("Content-Type", "application/json");

    String timestamp = getCurrentTime();
    int debit = -50; // Assuming a fixed debit amount
    int credit = 0;  // Assuming credit is initially 0
    serialNo++;  // Increment serial number with each scan

    String jsonData = "{\"s.no\":\"" + String(serialNo) + "\",\"uid\":\"" + uid + "\",\"date\":\"" + timestamp.substring(0, 10) + "\",\"time\":\"" + timestamp.substring(11) + "\",\"debit\":" + debit + ",\"credit\":" + credit + "}";

    Serial.print("Sending JSON data: ");
    Serial.println(jsonData);

    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
      
      // Handle redirect if needed
      if (httpResponseCode == HTTP_CODE_MOVED_PERMANENTLY || httpResponseCode == HTTP_CODE_FOUND || httpResponseCode == HTTP_CODE_SEE_OTHER) {
        String redirectUrl = http.getLocation();
        http.end();
        
        // Follow the redirect
        http.begin(redirectUrl);
        http.addHeader("Content-Type", "application/json");
        httpResponseCode = http.POST(jsonData);
        
        if (httpResponseCode > 0) {
          response = http.getString();
          Serial.println(httpResponseCode);
          Serial.println(response);
        } else {
          Serial.print("Error on sending POST after redirect: ");
          Serial.println(httpResponseCode);
        }
      }
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Error in WiFi connection");
  }
}

String getCurrentTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return "";
  }
  char buf[20];
  strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &timeinfo);
  return String(buf);
}