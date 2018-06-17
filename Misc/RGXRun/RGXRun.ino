#include<Servo.h>

#define PIN 6
int cnt = -1;
Servo arm;

void setup() {
  Serial.begin(115700);
  while(!Serial);
  Serial.println("Serial is up");
  arm.attach(PIN);
  mv(1000);
}

void loop() {
  if(Serial.available() > 0) {
    Serial.read();
    changePosition();
  }
}

void changePosition() {
  cnt++;
  if(cnt > 5) //Assuming there are 5 injections, change based on number
    cnt = 0;
  Serial.print("Moving to position: ");
  Serial.println(cnt);

  //When you figure out what the correct numbers are just insert them where I put the approximates....
  //(i.e. where its written mv(NUMBER), put in the right number, hopefully thats clear)
  //If you want more positions just add more if statements in the same format
  if(cnt == 0)
    mv(1000);
  else if(cnt == 1)
    mv(1200);
  else if(cnt == 2)
    mv(1400);
  else if(cnt == 3)
    mv(1600);
  else if(cnt == 4)
    mv(1800);
  else if(cnt == 5)
    mv(2000);
}

void mv(int pos) {
  arm.writeMicroseconds(pos);
  Serial.print("Servo is at: ");
  Serial.println(pos);
}

