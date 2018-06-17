#include<Servo.h>

#define PIN 5
#define MIN 1000
#define MAX 2000
#define NEUTRAL 1500
#define INCREMENT 5

Servo arm;
int pos;
byte tmp;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Serial is up");
  initServo();
  Serial.println("Servo initialized");
  pos = NEUTRAL;
}

void initServo() {
  pos = NEUTRAL;
  arm.attach(PIN);
  arm.writeMicroseconds(pos);
}

void loop() {
  if(Serial.available() > 0) {
    tmp = Serial.read();
    if(tmp == 'd')
      up();
    else if(tmp == 'a')
      down();
  }
  delay(50);
}

void up() {
  mv(-INCREMENT);
}
void down() {
  mv(INCREMENT);
}

void mv(int dist) {
  if(pos + dist > MIN && pos + dist < MAX)
    pos += dist;
  arm.writeMicroseconds(pos);
  Serial.print("Position: ");
  Serial.println(pos);
}

