#include <Stepper.h>

#include <SocketIoClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define LED 2

const char* ssid = "r";
const char* password = "12345678";
SocketIoClient webSocket;

uint8_t wire1 = 0;
uint8_t wire2 = 4;
uint8_t wire3 = 5;
uint8_t wire4 = 16;

const int stepsPerRevolution = 90;
// change this to fit the number of steps per revolution
// for your motor
// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, wire1, wire2, wire3, wire4);

const uint16_t _delay = 5; /* delay in between two steps. minimum delay more the rotational speed */

void sequence(bool a, bool b, bool c, bool d){  /* four step sequence to stepper motor */
  digitalWrite(wire1, a);
  digitalWrite(wire2, b);
  digitalWrite(wire3, c);
  digitalWrite(wire4, d);
  delay(_delay);
}

void event(const char * payload, size_t length) {
  digitalWrite(14, LOW);
  delay(1000);
  digitalWrite(14, HIGH);

  Serial.println("ring it");
}

void openD(const char * payload, size_t length) {
  for(int i = 0; i<1000; i++)
  {
    myStepper.step(stepsPerRevolution);
  }

  delay(15000);
  /* Rotation in opposite direction */
  for(int i = 0; i<1000; i++)
  {
    myStepper.step(-stepsPerRevolution);
  }
  Serial.println("Opening Door");
}

void setup() {
  //pinMode(5, OUTPUT);
  //pinMode(16, OUTPUT);
  //digitalWrite(16, LOW);
  pinMode(wire1, OUTPUT); /* set four wires as output */
  pinMode(wire2, OUTPUT);
  pinMode(wire3, OUTPUT);
  pinMode(wire4, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  myStepper.setSpeed(50);

  webSocket.on("ring", event);
  webSocket.on("openIt", openD);
  webSocket.begin("iotmid.herokuapp.com");

  Serial.println("Connected.");
}

void loop() {
  webSocket.loop();
}
