#define DATA_IN 9
#define CLOCK_IN 8

int clk = 1, lst_clk = 1, out = 0;
unsigned long tim = 0, timSt = 0;

void setup() {
  pinMode(DATA_IN, INPUT);
  pinMode(CLOCK_IN, INPUT);

  Serial.begin(115200);
  Serial.println("Starting program");
}

void loop() {
  lst_clk = clk;
  clk = digitalRead(CLOCK_IN);

  if(lst_clk == 1 && clk == 0) {
    out = digitalRead(DATA_IN) + digitalRead(DATA_IN) + digitalRead(DATA_IN);
    if((micros() - tim) > 800)
      Serial.println(" ");
    else if((micros() - tim) > 400)
      Serial.print(" ");

    if(out > 0)
      Serial.print("1");
    else 
      Serial.print("0");
    Serial.print(",");
    tim = micros();
  }
}
