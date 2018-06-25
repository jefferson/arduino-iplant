//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "host.firebaseio.com"
#define FIREBASE_AUTH "*******************"
#define WIFI_SSID "network_ssid"
#define WIFI_PASSWORD "netword_passowrd"

const int relayPin = D6; 

void setup() {
  
  Serial.begin(9600);

  pinMode(relayPin, OUTPUT);
  
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

//bool relayIsOpen = false;
void loop() {
  
  int ValorADC;
  
  float UmidadePercentual;
  
  ValorADC = analogRead(0);   //978 -> 3,3V
  
  //Serial.print("[Leitura ADC] ");
  //Serial.println(ValorADC);

  UmidadePercentual = 100 * ((1024-(float)ValorADC) / 1024);
  //Serial.print("[Umidade Percentual] ");
  //Serial.print(UmidadePercentual);
  //Serial.println("%");

  Firebase.setFloat("planta/MxjCfjOkT1a2ACZTGRVGKHIbwgF2/humidity", UmidadePercentual);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }

  bool open_tap = Firebase.getBool("planta/MxjCfjOkT1a2ACZTGRVGKHIbwgF2/open_tap");

  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  
  if(!open_tap)
  {
    Serial.print("ligando rele: " + String(open_tap) + "\n ");
    digitalWrite(relayPin, HIGH); //liga
  }
  else
  {
    Serial.print("desligando rele: " + String(open_tap) + "\n ");
    digitalWrite(relayPin, LOW); //desliga
  }
 
  delay(5000);
  
  
}

