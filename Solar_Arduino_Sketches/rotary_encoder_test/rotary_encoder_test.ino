#include <math.h>

int pin0 = 2;
int pin1 = 3;
int pin2 = 4;
int pin3 = 5;
byte total[4];
int counter = 0;
int current_total = 0;
int prev_total = -1;

void setup() {
  pinMode(pin0, INPUT);
  pinMode(pin1, INPUT);
  pinMode(pin2, INPUT);
  pinMode(pin3, INPUT);


  // put your setup code here, to run once:
  Serial.begin(9600);

  while (!Serial) {
    delay(1); // will pause Zero, Leonardo, etc until serial console opens
  }

  Serial.println("Serial Connected");
}

void loop() {
  total[0] = digitalRead(pin0);
  total[1] = digitalRead(pin1);
  total[2] = digitalRead(pin2);
  total[3] = digitalRead(pin3);

  Serial.print("Val 0 is "); Serial.println(total[0]);
  Serial.print("Val 1 is "); Serial.println(total[1]);
  Serial.print("Val 2 is "); Serial.println(total[2]);
  Serial.print("Val 3 is "); Serial.println(total[3]);

  if(prev_total == -1) prev_total = GrayToBinary32(total);
  else current_total = GrayToBinary32(total);

  if(prev_total > current_total) counter++;
  else if(prev_total < current_total) counter--;

  Serial.print("Total is "); Serial.println((int)total);


  Serial.print("Current Total is "); Serial.println(current_total);

  Serial.print("Counter is "); Serial.println(counter);

  delay(200);
}

unsigned int GrayToBinary32(unsigned int num)
{
    num = num ^ (num >> 16);
    num = num ^ (num >> 8);
    num = num ^ (num >> 4);
    num = num ^ (num >> 2);
    num = num ^ (num >> 1);
    return num;
}
