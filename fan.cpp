#include <IRremote.h>
#include <SoftwareSerial.h> 

const int RECV_PIN = 4;
const int RELAY_FAST_PIN = 13;
const int RELAY_MEDIUM_PIN = 12;
const int RELAY_SLOW_PIN = 11;

const int MANUAL_FAST_PIN = A2;
const int MANUAL_MEDIUM_PIN = A1;
const int MANUAL_SLOW_PIN = A0;

const int SIGNAL_QUALITY = 100;
const float HIGH_VOLTAGE = 5.00;

const int HIGHA = HIGH;
const int LOWA = LOW;

const long ZERO = 0xFF6897;
const long ONE = 0xFF30CF;
const long TWO = 0xFF18E7;
const long THREE = 0xFF7A85;

long currentState;
long currentManualState;

IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  
  pinMode(RELAY_FAST_PIN, OUTPUT);
  pinMode(RELAY_MEDIUM_PIN, OUTPUT);
  pinMode(RELAY_SLOW_PIN, OUTPUT);

  currentState = ZERO;
  currentManualState =  ZERO;
  
  poweroff();
}

void loop() {
  if(irrecv.decode(&results)) {       
    if(results.value != 0xFFFFFFFF) {     
      changeState(results.value);
    }    
  
    irrecv.resume();
  } else {
     
     if(isHigh(MANUAL_FAST_PIN)) {
       checkManual(THREE);
     } else if (isHigh(MANUAL_MEDIUM_PIN)) {    
       checkManual(TWO);
     } else if (isHigh(MANUAL_SLOW_PIN)) {      
      checkManual(ONE);
     } else {
      checkManual(ZERO);
     }
     
  }
}

boolean isHigh(int pin) {
  float manualFast = 0.0;
  for(int i = 0; i < SIGNAL_QUALITY; i++) {
   manualFast = manualFast + (analogRead(pin) * (HIGH_VOLTAGE / 1023.0));
  }
  return (manualFast / SIGNAL_QUALITY) == HIGH_VOLTAGE;
}

void checkManual(long state) {
  if(currentManualState == state) return;
  currentManualState = state;
    
  changeState(state);
}

void changeState(long state) {
 if(currentState == state) return;
 currentState = state;
 
  if(state == ZERO) {
      poweroff();
  } else if (state == ONE) {
      one();
  } else if (state == TWO) {
      two();
  } else if (state == THREE) {
      three();
  }
}

void poweroff() {
 digitalWrite(RELAY_FAST_PIN, LOWA);
 digitalWrite(RELAY_MEDIUM_PIN, LOWA);
 digitalWrite(RELAY_SLOW_PIN, LOWA);
}

void one() {
 digitalWrite(RELAY_FAST_PIN, LOWA);
 digitalWrite(RELAY_MEDIUM_PIN, LOWA);
 digitalWrite(RELAY_SLOW_PIN, HIGHA);
}

void two() {
 digitalWrite(RELAY_FAST_PIN, LOWA);
 digitalWrite(RELAY_SLOW_PIN, LOWA);
 digitalWrite(RELAY_MEDIUM_PIN, HIGHA);
 
}

void three(){
 digitalWrite(RELAY_MEDIUM_PIN, LOWA);  
 digitalWrite(RELAY_SLOW_PIN, LOWA);
 digitalWrite(RELAY_FAST_PIN, HIGHA);
}
