#define zeroCrossPin 2
#define ledOut 5
#define buttonPin 3
#define TIME_FOR_TRUE_ZEROCROSS 280
#define CONTROL_IMPULSE_DURATION 320

volatile boolean buttonIsPress = false;
volatile unsigned long cnt1 = 0;
volatile unsigned long cnt2 = 0;
volatile unsigned long last = 0;
int powerPercent = 15;                    // !!обязательно больше 3%
volatile int controlImpulseStart = (100 * (100 - powerPercent) + TIME_FOR_TRUE_ZEROCROSS) / 20;
int fade = 5;

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  pinMode(zeroCrossPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(ledOut, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(6, OUTPUT);
  attachInterrupt(0, zeroCrossDetectFall, FALLING);
  attachInterrupt(1, buttonDetect, FALLING);
  
  // Timer/Counter 1 initialization
  // Clock source: System Clock
  // Clock value: 16000,000 kHz
  // Mode: Normal top=0xFFFF
  // OC1A output: Disconnected
  // OC1B output: Disconnected
  // Noise Canceler: Off
  // Input Capture on Falling Edge
  // Timer Period: 0,02 ms = 20мкс
  // Timer1 Overflow Interrupt: On
  // Input Capture Interrupt: Off
  // Compare A Match Interrupt: Off
  // Compare B Match Interrupt: Off
  TCCR1A = (0 << COM1A1) | (0 << COM1A0) | (0 << COM1B1) | (0 << COM1B0) | (0 << WGM11) | (0 << WGM10);
  TCCR1B = (0 << ICNC1) | (0 << ICES1) | (0 << WGM13) | (0 << WGM12) | (0 << CS12) | (0 << CS11) | (1 << CS10);
  TCNT1H = 0xFE;
  TCNT1L = 0xC0;
  ICR1H = 0x00;
  ICR1L = 0x00;
  OCR1AH = 0x00;
  OCR1AL = 0x00;
  OCR1BH = 0x00;
  OCR1BL = 0x00;


  // Timer/Counter 1 Interrupt(s) initialization
  TIMSK1 = (0 << ICIE1) | (0 << OCIE1B) | (0 << OCIE1A) | (1 << TOIE1);
}

void loop() {
  if(buttonIsPress){
    powerPercent += fade;
    if (powerPercent <= 15 || powerPercent >= 75) {
      fade = -fade;
    }
    controlImpulseStart = (100 * (100 - powerPercent) + TIME_FOR_TRUE_ZEROCROSS) / 20;
    buttonIsPress = false;
  }

  /*if (millis() - last >= 25) {
    powerPercent += fade;
    if (powerPercent <= 15 || powerPercent >= 75) {
      fade = -fade;
    }
    controlImpulseStart = (100 * (100 - powerPercent) + TIME_FOR_TRUE_ZEROCROSS) / 20;
    last = millis();
  }*/
}

void buttonDetect(){
  buttonIsPress = true;
}

void zeroCrossDetectFall() {
  //обнуляем программный таймер
  cnt1 = 0;
  //PORTD &= ~(1 << ledOut);
  attachInterrupt(0, zeroCrossDetectRise, RISING);
}


void zeroCrossDetectRise() {
  //обнуляем программный таймер
  cnt1 = 0;
  PORTD &= ~(1 << ledOut);
  attachInterrupt(0, zeroCrossDetectFall, FALLING);
}
ISR(TIMER1_OVF_vect) {
  // every 20us
  // Reinitialize Timer1 value
  TCNT1H = 0xFEC0 >> 8;
  TCNT1L = 0xFEC0 & 0xff;
  // Place your code here
  cnt1++;
  if (cnt1 == controlImpulseStart) {
    PORTD |= (1 << ledOut);
  }
}


