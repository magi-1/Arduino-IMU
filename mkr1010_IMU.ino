#include <MKRIMU.h>
#include <WiFiNINA.h>
#include "secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int status = WL_IDLE_STATUS;

WiFiClient client;

void setup() {
  Serial.begin(9600);
 

  // Connecting to WiFi
  while (status != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    status = WiFi.begin(ssid, pass);
    delay(1000);
  }
  Serial.println(": Success");

  // Connecting to server
  Serial.print("Connecting to ");
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

  // IMU naming conventions
  Serial.println("ax, ay, az, theta, beta, phi");
}

void loop() {

  while (millis() < 1e6) {
    if (IMU.accelerationAvailable() && IMU.eulerAnglesAvailable()) {
      
      send_IMU_data();
      
      //add this functionality later
      //comes from GET on app and is mapped to BT data, request made on iphone not arduino
      //user="Landon"&workout="benchpress"&set=3
      //raises question of where batching should be done and how, record on iphone, then make sql insert via flask? probably
      }
    }
  }

void send_IMU_data() {
  // Recording sensor data
  float ax, ay, az, theta, beta, phi;
  IMU.readAcceleration(ax, ay, az);
  IMU.readEulerAngles(theta, beta, phi);

  // Creating request
  String request, linear_accel, rot_accel;
  linear_accel += "ax=" + String(ax) + "&ay=" + String(ay) + "&az=" + String(az);
  rot_accel += "&theta=" + String(theta) + "&beta=" + String(beta) + "&phi=" + String(phi);
  request += linear_accel + rot_accel;

  
  // Making GET request to {local server} 
  client.connect(servername, port);
  client.println("GET /log?" + request + " HTTP/1.1");
  client.print("Host: ");
  client.println(servername);
  client.println("Connection: close");
  client.println();
  client.stop();

  // Viewing data on serial port
  print_IMU_data(ax, ay, az, theta, beta, phi);
}

void print_IMU_data(float ax, float ay,float az,float theta,float beta,float phi) {
  Serial.print(ax);
  Serial.print(", ");
  Serial.print(ay);
  Serial.print(", ");
  Serial.print(az);
  Serial.print(", ");
  Serial.print(theta);
  Serial.print(", ");
  Serial.print(beta);
  Serial.print(", ");
  Serial.println(phi);
}
