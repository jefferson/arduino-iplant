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
  
  ValorADC = analogRead(0);

  UmidadePercentual = 100 * ((1024-(float)ValorADC) / 1024);

  Firebase.setFloat("planta/quintal-code/humidity", UmidadePercentual);
  
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }

  bool open_tap = Firebase.getBool("planta/quintal-code/open_tap");

  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  
  if(!open_tap)
  {  
    digitalWrite(relayPin, HIGH); //liga
  }
  else
  {
    digitalWrite(relayPin, LOW); //desliga
  }
 
  delay(5000);
  
}

