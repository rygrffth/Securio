#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#define PIR_PIN 26
#define LED_PIN 2
#define BUZZER_PIN 15
#define WIFI_SSID "ELITLAB"
#define WIFI_PASSWORD "l@b3l1tiA"
#define API_KEY "AIzaSyDabdkHCwP50AEQa8FJdcIXV8o2Nk9ME3c"
#define DATABASE_URL "https://tesapp-5aaf1-default-rtdb.firebaseio.com/"
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;
String sValue, sValue2;
void setup() {
  Serial.begin(115200);
  pinMode(PIR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback; 
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  if (Firebase.ready() && signupOK ) {
    if (Firebase.RTDB.getString(&fbdo, "/L1")) {
      if (fbdo.dataType() == "string") {
        sValue = fbdo.stringData();
        int a = sValue.toInt();
        Serial.println(a);
        if (a == 1){
          int pirState = digitalRead(PIR_PIN);
    
    if (pirState) {
      Serial.println("GERAKAN TERDETEKSI!");
      Serial.println("ADA PENYUSUPP!");
      digitalWrite(LED_PIN, HIGH);
      for(int i = 0; i < 8; i++) {
        digitalWrite(BUZZER_PIN, HIGH); 
        delay(100); 
        digitalWrite(BUZZER_PIN, LOW); 
        delay(100); 
      }
      if (Firebase.RTDB.setString(&fbdo, "Status", "1")){
        Serial.println("PASSED");
        Serial.println("PATH: " + fbdo.dataPath());
        Serial.println("TYPE: " + fbdo.dataType());
      }
      else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
    } 
    else {
      Serial.println("TIDAK ADA GERAKAN TERDETEKSI.");
      Serial.println("SITUASI AMAN.");
      digitalWrite(LED_PIN, LOW); 
      if (Firebase.RTDB.setString(&fbdo, "Status", "2")){
        Serial.println("PASSED");
        Serial.println("PATH: " + fbdo.dataPath());
        Serial.println("TYPE: " + fbdo.dataType());
      }
      else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
    }
    }
  
    else if (a == 0){
        digitalWrite(LED_PIN, LOW); 
    if (Firebase.RTDB.setString(&fbdo, "Status", "0")){
        Serial.println("PASSED");
        Serial.println("PATH: " + fbdo.dataPath());
        Serial.println("TYPE: " + fbdo.dataType());
      }
      else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
         }
        
        
        else{
  Serial.println("SISTEM MATI.");
  Serial.println("NYALAKAN TERLEBIH DAHULU.");
  digitalWrite(LED_PIN, LOW); 
    if (Firebase.RTDB.setString(&fbdo, "Status", "2")){
        Serial.println("PASSED");
        Serial.println("PATH: " + fbdo.dataPath());
        Serial.println("TYPE: " + fbdo.dataType());
      }
      else {
        Serial.println("FAILED");
        Serial.println("REASON: " + fbdo.errorReason());
      }
        }
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
  }
}
