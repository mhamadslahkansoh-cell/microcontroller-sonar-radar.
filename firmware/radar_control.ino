#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <Servo.h>

// Pin Definitions
#define TRIG_PIN D2
#define ECHO_PIN D1
#define SERVO_PIN D5
#define BUZZER_PIN D6

// Wi-Fi & Telegram Credentials (Replace with your own credentials)
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";
const char* botToken = "YOUR_TELEGRAM_BOT_TOKEN";
const char* chatId = "YOUR_TELEGRAM_CHAT_ID";

WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);
Servo radarServo;

unsigned long lastWifiCheck = 0;
const long wifiCheckInterval = 30000; 
unsigned long lastAlertTime = 0;
const unsigned long alertCooldown = 10000; 

float getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duration == 0) return 999;
  return duration * 0.0343 / 2.0;
}

void checkRadarAlert(int angle, float distance) {
  if (distance > 0 && distance <= 20) {
    if (millis() - lastAlertTime > alertCooldown) {
      if (WiFi.status() == WL_CONNECTED) {
        String zoneName = (angle < 65) ? "Left Zone" : (angle < 115) ? "Center Zone" : "Right Zone";
        String msg = "🚨 *Radar Alert!*\nObject detected in " + zoneName + "\nDistance: " + String(distance, 1) + " cm.";
        
        bot.sendMessage(chatId, msg, "Markdown");
        lastAlertTime = millis(); 
      }
    }
  }
}

void handleBackgroundWifi() {
  if (millis() - lastWifiCheck > wifiCheckInterval) {
    lastWifiCheck = millis();
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("\n[WiFi] Trying to reconnect in background...");
      WiFi.begin(ssid, password); 
    }
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000); 

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  analogWriteRange(1023);
  analogWriteFreq(2000);      
  analogWrite(BUZZER_PIN, 0);

  radarServo.attach(SERVO_PIN);
  radarServo.write(15);

  client.setInsecure();
  
  // Non-blocking WiFi initialization
  Serial.println("\n--- Starting WiFi in Background ---");
  WiFi.begin(ssid, password);
  
  delay(500); // Short delay to allow module setup
}

void loop() {
  // Sweep right
  for (int angle = 15; angle <= 165; angle++) {
    radarServo.write(angle);
    delay(35); 

    float distance = getDistance();

    Serial.print(angle);
    Serial.print(",");
    Serial.println(distance);

    if (distance > 0 && distance <= 20) {
      analogWrite(BUZZER_PIN, 512);   
    } else {
      analogWrite(BUZZER_PIN, 0);
    }

    checkRadarAlert(angle, distance);
    handleBackgroundWifi();
  }

  // Sweep left
  for (int angle = 165; angle >= 15; angle--) {
    radarServo.write(angle);
    delay(35); 

    float distance = getDistance();

    Serial.print(angle);
    Serial.print(",");
    Serial.println(distance);

    if (distance > 0 && distance <= 20) {
      analogWrite(BUZZER_PIN, 512);
    } else {
      analogWrite(BUZZER_PIN, 0);
    }

    checkRadarAlert(angle, distance);
    handleBackgroundWifi();
  }
}
