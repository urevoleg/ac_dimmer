#include <util/delay.h>

#define zeroCrossPin 2
#define ledOut 5

volatile boolean zeroCrossFlag = false;
int powerPercent = 25;                    // !!обязательно больше 3%
unsigned long last = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(zeroCrossPin, INPUT);
  pinMode(ledOut, OUTPUT);
  attachInterrupt(0, zeroCrossDetect, RISING);
}

void loop() {
  if (zeroCrossFlag) {
    zeroCrossFlag = false;
    unsigned long startTime = micros();
    while (micros() - startTime <= 278) {
    }
    
    startTime = micros();
    while (micros() - startTime <= (100 * (100 - powerPercent))) {
      PORTD &= ~(1 << ledOut);
    }

    startTime = micros();
    while (micros() - startTime <= (500)) {
      PORTD |= (1 << ledOut);
    }
    PORTD &= ~(1 << ledOut);
  }

  /*if(millis() - last > 1000){
    powerPercent += 5;
    if(powerPercent == 105){
      powerPercent = 5;
    }
    last = millis();
    }*/
}

void zeroCrossDetect() {
  zeroCrossFlag = true;
}
