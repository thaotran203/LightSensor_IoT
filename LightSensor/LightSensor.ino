/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPL6uJdblKjA"
#define BLYNK_TEMPLATE_NAME "Light Sensor"
#define BLYNK_AUTH_TOKEN "S6xVzFfF-G2cOmz8kTa6_UwWWOg0NThK"
#define FIREBASE_HOST "https://fir-smarthome-4d96e-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyCcTrOFOgzZGBaoQfp8jbjmW3b5VO__NgA"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <FirebaseESP8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "S6xVzFfF-G2cOmz8kTa6_UwWWOg0NThK";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Greenwich-Student";
char pass[] = "12345678";

// Define the pins
const int light_sensor_pin = A0;
const int led_pin_1 = D1;
const int led_pin_2 = D2;
const int led_pin_3 = D3;
const int led_pin_4 = D5;
const int led_pin_5 = D6;
int threshold = 100;
String LightStatus = "";
FirebaseData firebaseData;

void setup()
{
  // Debug console
  Serial.begin(9600);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  Blynk.begin(auth, ssid, pass);
  pinMode(light_sensor_pin, INPUT);
  pinMode(led_pin_1, OUTPUT);
  pinMode(led_pin_2, OUTPUT);
  pinMode(led_pin_3, OUTPUT);
  pinMode(led_pin_4, OUTPUT);
  pinMode(led_pin_5, OUTPUT);
}

BLYNK_WRITE(V3) {
  int value = param.asInt();
  
  if (value == 1) {
    digitalWrite(led_pin_3, HIGH);
    Blynk.virtualWrite(V3, HIGH);
  } else {
    digitalWrite(led_pin_3, LOW);
    Blynk.virtualWrite(V3, LOW);
  }
}

BLYNK_WRITE(V5) {
  int value = param.asInt();
  
  if (value == 1) {
    digitalWrite(led_pin_4, HIGH);
    Blynk.virtualWrite(V5, HIGH);
  } else {
    digitalWrite(led_pin_4, LOW);
    Blynk.virtualWrite(V5, LOW);
  }
}

BLYNK_WRITE(V6) {
  int value = param.asInt();
  
  if (value == 1) {
    digitalWrite(led_pin_5, HIGH);
    Blynk.virtualWrite(V6, HIGH);
  } else {
    digitalWrite(led_pin_5, LOW);
    Blynk.virtualWrite(V6, LOW);
  }
}

void loop()
{
  int rawData = analogRead(light_sensor_pin);
  Serial.println(rawData);

  if (rawData < threshold) {
      LightStatus = "It's night time now";
      Firebase.RTDB.setString(&firebaseData, "Light Sensor/Status 1", LightStatus);
      Firebase.RTDB.setString(&firebaseData, "Light Sensor/Status 2", "The light turns ON");
      digitalWrite(led_pin_1, HIGH);
      digitalWrite(led_pin_2, HIGH);
      
      Blynk.virtualWrite(V1, 1); // update Blynk app to show LEDs are ON
      Blynk.virtualWrite(V2, 1);

  }
  else {
      LightStatus = "It's day time now";
      Firebase.RTDB.setString(&firebaseData, "Light Sensor/Status 1", LightStatus);
      Firebase.RTDB.setString(&firebaseData, "Light Sensor/Status 2", "The  light turns OFF");
      digitalWrite(led_pin_1, LOW);
      digitalWrite(led_pin_2, LOW);

      Blynk.virtualWrite(V1, 0); // update Blynk app to show LEDs are OFF
      Blynk.virtualWrite(V2, 0);
  }
  
  Blynk.run();
  Firebase.RTDB.setFloat(&firebaseData, "Light Sensor/Value", rawData);

}
