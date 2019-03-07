#include <math.h>

int pin0 = 2;
int pin1 = 3;
int pin2 = 4;
int pin3 = 5;
int val0=0,val1=0,val2=0,val3=0;
int num=0;
int counter = 0;
int current_gray = 0;
int prev_gray = -1;

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
  val0 = digitalRead(pin0);
  val1 = digitalRead(pin1);
  val2 = digitalRead(pin2);
  val3 = digitalRead(pin3);

  Serial.print("Val 0 is "); Serial.println(val0);
  Serial.print("Val 1 is "); Serial.println(val1);
  Serial.print("Val 2 is "); Serial.println(val2);
  Serial.print("Val 3 is "); Serial.println(val3);

  num = val0+val1*2+val2*4+val3*8;

  if(prev_gray == -1) prev_gray = GrayToBinary16(num);
  else current_gray = GrayToBinary16(num);

  if(prev_gray > current_gray) counter++;
  else if(prev_gray < current_gray) counter--;

  Serial.print("num is "); Serial.println(num);

  Serial.print("current_total is "); Serial.println(current_gray);

  Serial.print("prev_gray is "); Serial.println(prev_gray);

  Serial.print("Counter is "); Serial.println(counter);

  prev_gray = current_gray;

  delay(200);
}

unsigned int GrayToBinary16(unsigned int num)
{
    num = num ^ (num >> 16);
    num = num ^ (num >> 8);
    num = num ^ (num >> 4);
    num = num ^ (num >> 2);
    num = num ^ (num >> 1);
    return num;
}
