#include <MKRIMU.h>
#include <WiFiNINA.h>
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;

char GoogleWebApp_URL[] = "script.google.com";

WiFiSSLClient client;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // Connecting to WiFi
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(10000);
  }
  Serial.println("WiFi Connection Success");

  if (client.connect(GoogleWebApp_URL, 443)) {
    Serial.println("Google Sheets Connection Success");
  }

  
  // IMU Initialization Check and Variable Display
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.println("ax, ay, az, theta, phi, psi");
}

void loop() {

  float ax, ay, az, heading, roll, pitch;
  while (millis() < 1e6) {
    if (IMU.accelerationAvailable() && IMU.eulerAnglesAvailable()) {

      // Kinematic Data
      IMU.readAcceleration(ax, ay, az);
      IMU.readEulerAngles(heading, roll, pitch);

      // Sending to google sheet

      String Request, LinearAccel, RotAccel;
      //URL += GoogleWebApp_URL;
      LinearAccel += "ax=" + String(ax) + "&ay=" + String(ay) + "&az=" + String(az);
      RotAccel += "&heading=" + String(heading) + "&roll=" + String(roll) + "&pitch=" + String(pitch);
      Request += LinearAccel + RotAccel;
      
      Serial.println(Request);
      client.println("GET /macros/s/AKfycbzfN7JUFRLJAqIqgYf4D1m_O3SPxTM8MOI0r-75sBR3bKXDMTOS/exec?" + Request + " HTTP/1.1");
      client.print("Host: ");
      client.println(GoogleWebApp_URL);
      client.println("Connection: keep-alive");
      client.println();
      
     
      }
    }
  }
