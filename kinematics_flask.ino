#include <BOSCH_IMU.h>
#include <WiFiNINA.h>
#include "secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;

WiFiClient client;

float ax, ay, az, qw, qx, qy, qz;
double currTime, prevTime;
int prec = 5;

void setup() {
  Serial.begin(9600);
 
  // Connecting to WiFi
  while (status != WL_CONNECTED) {
    Serial.print("Connecting: ");
    Serial.print(ssid);
    status = WiFi.begin(ssid, pass);
    delay(3000);
    Serial.println();
  }
  Serial.println(": Success");

  // Connecting to server
  Serial.print("Connecting: ");
  Serial.print(servername);
  if (client.connect(servername, port)) {
    Serial.println(": Success");
  }
  else {
    Serial.println(": Failure");
  }
  
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {

   while (millis() < 1e6) {
    if (IMU.accelerationAvailable()) {
      
      double elapsedTime = millis()-prevTime;
      currTime += elapsedTime/1000.0;
      prevTime = millis();
      
      IMU.readAcceleration(ax, ay, az);
      IMU.readQuaternion(qw, qx, qy, qz);
      
      Serial.print(currTime, prec);
      Serial.print(", ");
      Serial.print(ax, prec);
      Serial.print(',');
      Serial.print(ay, prec);
      Serial.print(',');
      Serial.print(az, prec);
      Serial.print(',');
      Serial.print(qw, prec);
      Serial.print(',');
      Serial.print(qx, prec);
      Serial.print(',');
      Serial.print(qy, prec);
      Serial.print(',');
      Serial.println(qz, prec);
      send_IMU_data(currTime, ax, ay, az, qw, qx, qy, qz);
      }
  }
}

void send_IMU_data(double currTime, float ax, float ay,float az, float qw, float qx, float qy, float qz) {

  // Creating request
  String request, accel, quaternion;
  accel += "&ax=" + String(ax) + "&ay=" + String(ay) + "&az=" + String(az);
  quaternion += "&qw=" + String(qw) + "&qx=" + String(qx) + "&qy=" + String(qy) + "&qz=" + String(qz);
  request += "t=" + String(currTime) + accel + quaternion;
  
  // Making GET request to {local server} 
  client.connect(servername, port);
  client.println("GET /log?" + request + " HTTP/1.1");
  client.print("Host: ");
  client.println(servername);
  client.println("Connection: close");
  client.println();
  client.stop();
  
}
