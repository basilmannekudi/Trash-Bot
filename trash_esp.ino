// Trashbot esp pin assignmanets//

//D1 gp5   level 1
//D2 gp4   level 2
//D5 gp14  level 3
//D6 gp12  obst  
//D7 gp13  buzzer

#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h" 
#define API_KEY "xxxxx"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "xxxxxxxx" 
#define FIREBASE_AUTH "xxxxxx"
#define lvl1 5
#define lvl2 4
#define lvl3 14
#define obst 12
#define buzz 13
int tlvl = 0;
int l1;
int l2;
int l3;

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
bool signupOK = false;
void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
  WiFiManager wm;
bool res;
res = wm.autoConnect("Trashbot");
        config.api_key = API_KEY;
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
  pinMode(lvl1,INPUT);
  pinMode(lvl2,INPUT);
  pinMode(lvl3,INPUT);
  pinMode(obst,INPUT);
  pinMode(buzz,OUTPUT);
  
}

void loop() {
l1 = digitalRead(lvl1);
l2 = digitalRead(lvl2);
l3 = digitalRead(lvl3);
 if (l1 == HIGH && l2 == HIGH && l3 == HIGH) {
    tlvl = 0;
  } else if (l1 == LOW && l2 == HIGH && l3 == HIGH) {
    tlvl = 28;
  } else if (l1 == LOW && l2 == LOW && l3 == HIGH) {
    tlvl = 56;
  } else if (l1 == LOW && l2 == LOW && l3 == LOW) {
    tlvl = 100;
  }  
  else {
    tlvl = 0;
  }
  
if(digitalRead(obst)==LOW)
{
  digitalWrite(buzz,HIGH);
  }
  else
  {
    digitalWrite(buzz,LOW);
    }
  
   if (Firebase.ready() && signupOK){
    Firebase.RTDB.setInt(&fbdo, "data/level",tlvl);
    delay(50);
    }

}
