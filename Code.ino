#include "SevSeg.h"
SevSeg sevseg;

#define LedRed  12
#define LedGreen  13
#define speaker 12 

const int pingPin_1_2 = 10; 
const int echoPin1 = 8; // Echo Pin of Ultrasonic Sensor
const int echoPin2 = 9; // Echo Pin of Ultrasonic Sensor
long long int delay_time = 60000;

int motion_number = 0;
bool motion0 = false;
bool motion1 = false;

int a = A0;
int b = A1;
int c = A2;
int d = A3;
int e = A4;
int f = A5;
int g = 11;

const int digitSegments[10][7] = {
  {LOW, LOW, LOW, LOW, LOW, HIGH, LOW},  // 0
  {HIGH, LOW, LOW, HIGH, HIGH, HIGH, HIGH},      // 1
  {LOW, LOW, HIGH, LOW, LOW, LOW, HIGH},   // 2
  {LOW, LOW, LOW, LOW, HIGH, LOW, HIGH},   // 3
  {HIGH, LOW, LOW, HIGH, HIGH, LOW, LOW},    // 4
  {LOW, HIGH, LOW, LOW, HIGH, LOW, LOW},   // 5
  {LOW, HIGH, LOW, LOW, LOW, LOW, LOW},  // 6
  {LOW, LOW, LOW, HIGH, HIGH, HIGH, HIGH},     // 7
  {LOW, LOW, LOW, LOW, LOW, LOW, LOW}, // 8
  {LOW, LOW, LOW, LOW, HIGH, LOW, LOW}   // 9
};

void displayNumber(int num) {
  digitalWrite(a, digitSegments[num][0]);
  digitalWrite(b, digitSegments[num][1]);
  digitalWrite(c, digitSegments[num][2]);
  digitalWrite(d, digitSegments[num][3]);
  digitalWrite(e, digitSegments[num][4]);
  digitalWrite(f, digitSegments[num][5]);
  digitalWrite(g, digitSegments[num][6]);
}

void setup() {
  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
  

  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  
  pinMode(LedRed, OUTPUT);
  pinMode(LedGreen, OUTPUT);
  pinMode(speaker, OUTPUT); 
  digitalWrite(LedRed, HIGH);
  digitalWrite(LedGreen, HIGH);

  Serial.begin(9600); 
  delay(250);
  digitalWrite(LedRed, LOW);
  digitalWrite(LedGreen, LOW);
  Serial.println("starting ...");
}


void handleButton(int buttonPin, bool &motionFlag, int &value) {
  if (digitalRead(buttonPin) == HIGH && !motionFlag) {
    value += (buttonPin == 1) ? 1 : value == 0 ? 0 : -1;
    motionFlag = true;
    if(buttonPin == 1) Serial.println("VR on");
    else Serial.println("VR Off");
  }

  if (digitalRead(buttonPin) == LOW && motionFlag) {
    motionFlag = false;
  }
}

void loop() {
  int duration, duration2, cm, cm2, count;
  int next_signal = 0;

  // Green light for set1
  digitalWrite(2, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(4, LOW);
  digitalWrite(6, LOW);

  for(int i = 0; i < (delay_time / 1000) ; i++){
    displayNumber(motion_number);
    delay(1);

    handleButton(1, motion1, motion_number);
    handleButton(0, motion0, motion_number); 

    // Trigger ultrasonic sensor 1
    digitalWrite(pingPin_1_2, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin_1_2, HIGH);
    delayMicroseconds(10);
    digitalWrite(pingPin_1_2, LOW);
    duration = pulseIn(echoPin1, HIGH);
    cm = microsecondsToCentimeters(duration);
    Serial.print("cm 1 = ");
    Serial.println(cm);

    // Trigger ultrasonic sensor 2
    digitalWrite(pingPin_1_2, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin_1_2, HIGH);
    delayMicroseconds(10);
    digitalWrite(pingPin_1_2, LOW);
    duration2 = pulseIn(echoPin2, HIGH);
    cm2 = microsecondsToCentimeters(duration2);
    Serial.print("cm 2 = ");
    Serial.println(cm2);
    
    if (cm < 200 || cm2 < 200) { 

      count += 1;
      Serial.print("Count: ");
      Serial.println(count);

      if (count > 1 && motion_number > 2) {

        digitalWrite(LedRed, HIGH);
        digitalWrite(LedGreen, LOW);
        tone(speaker, 1000);
        delay_time = 100000;
        next_signal = 10;

      } else {

        digitalWrite(LedRed, LOW);
        digitalWrite(LedGreen, HIGH);
        noTone(speaker);

      }
    } else {

      count = 0;
      digitalWrite(LedRed, LOW);
      digitalWrite(LedGreen, HIGH);
      noTone(speaker);

    }
  }
  
  if(next_signal == 0){
    delay_time = 60000;
  }else{
    next_signal -= 1;
  }

// Yellow light for set1

  digitalWrite(3, HIGH);
  digitalWrite(7, HIGH);
  digitalWrite(2, LOW);
  delay(1000);

// Red light for set2

  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(7, LOW);
  delay(delay_time);

// Yellow light for set2

  digitalWrite(6, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, LOW);
  delay(1000);
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}