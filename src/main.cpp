#include <Arduino.h>
#include <Firebase_ESP_Client.h>
#include <WiFi.h>
// Provide the token generation process info.
#include <addons/TokenHelper.h>

// Provide the RTDB payload printing info and other helper functions.
#include <addons/RTDBHelper.h>
#define WIFI_SSID "Orange-206F"
#define WIFI_PASSWORD "RM66H5AA151"
#define API_KEY "AIzaSyCkfBds_XMAkI9lN1L6eUipMVoQlLfQpg4"

/* 2. Define the RTDB URL */
#define DATABASE_URL "https://espdata-45385-default-rtdb.firebaseio.com/"
#define LED_BUILTIN 2

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int intValue;
float floatValue;
bool signupOK = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
    Serial.println();

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

    /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  
// put function definitions here:
    
  
    if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    if (Firebase.RTDB.getInt(&fbdo, "/led2")) {
      if (fbdo.dataType() == "int") {
        intValue = fbdo.intData();
        Serial.println(intValue);
        digitalWrite(LED_BUILTIN,intValue);
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }

   /*this fun is for getting a string value from fb rtdb ya iheeb
   if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    if (Firebase.RTDB.getString(&fbdo, "/led2")) {
      if (fbdo.dataType() == "string") {
        stringVal = fbdo.StringData();
        Serial.println(stringVal);
        if(stringVal=='on'){stringVal='HIGH';}
        elif(stringVal=='off'){stringVal='LOW';}
        digitalWrite(LED_BUILTIN,stringVal);
      }*/
    }







  
}
}