// #include "MIDIUSB.h"

#define OUTPUT_FIRST 22
#define INPUT_FIRST 30

byte keyboard_status[64];
byte keyboard_status_old[64];
byte keyboard_status_toc[64];
byte keyboard_status_toc_old[64];
unsigned long tic[64];
unsigned long toc[64];

void readPin(int pin);
void readPin2(int pin);
// void noteOn(byte channel, byte pitch, byte velocity);
// void noteOff(byte channel, byte pitch, byte velocity);


// void noteOn(byte channel, byte pitch, byte velocity) {
//   midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
//   MidiUSB.sendMIDI(noteOn);
//   MidiUSB.flush();
// }

// void noteOff(byte channel, byte pitch, byte velocity) {
//   midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
//   MidiUSB.sendMIDI(noteOff);
//   MidiUSB.flush();
// }

void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);

  // set the output pin as output:
  for(int i=0; i<8; i++) {
    pinMode(INPUT_FIRST + i, INPUT);
    pinMode(OUTPUT_FIRST + i, OUTPUT);
    digitalWrite(OUTPUT_FIRST + i, LOW);
    pinMode(INPUT_FIRST + i + 16, INPUT);
    pinMode(OUTPUT_FIRST + i + 16, OUTPUT);
    digitalWrite(OUTPUT_FIRST + i + 16, LOW);
  }
  
  

  // Initialize keyboard_status
  for(int i=0; i<64;i++){
    keyboard_status[i]=0;
    keyboard_status_old[i]=0;
    keyboard_status_toc[i]=0;
    keyboard_status_toc_old[i]=0;
  }
}

void loop() {
  for(int i=0; i<8; i++) {
    digitalWrite(OUTPUT_FIRST + i, HIGH);
    digitalWrite(OUTPUT_FIRST + i + 16, HIGH);
    readPin(0 + i);
    readPin(8 + i);
    readPin(16 + i);
    readPin(24 + i);
    readPin2(0 + i);
    readPin2(8 + i);
    readPin2(16 + i);
    readPin2(24 + i);
    digitalWrite(OUTPUT_FIRST + i, LOW);
    digitalWrite(OUTPUT_FIRST + i + 16, LOW);
    // delay(2);
    
  }
  
}

void readPin(int pin) {
  keyboard_status[pin] = digitalRead(INPUT_FIRST + (pin / 8) * 2);
  keyboard_status_toc[pin] = digitalRead(INPUT_FIRST + 1 + (pin / 8) * 2);

  if (keyboard_status[pin] == 1 && keyboard_status_old[pin] == 0) {
    keyboard_status_old[pin] = 1;
    tic[pin] = micros();
    //Key pressed
    // Serial.print("Key ");
    // Serial.print(pin);
    // Serial.println(" Pressed");
  }
  if (keyboard_status[pin] == 0 && keyboard_status_old[pin] == 1) {
    keyboard_status_old[pin] = 0;
    tic[pin] = 0;
    // noteOff(1, ((pin - 64) * -1) + 35 , 64);
    //Key released
    Serial.print(pin);
    Serial.println("Key released");
  }
  if (keyboard_status_toc[pin] == 1 && keyboard_status_toc_old[pin] == 0) {
    keyboard_status_toc_old[pin] = 1;
    toc[pin] = micros();
    Serial.print("Key: ");
    Serial.print(pin);
    Serial.print(" Pressed | Velocity: ");
    Serial.println(toc[pin] - tic[pin]);
    int velocity = toc[pin] - tic[pin];
    if (velocity > 100000) {
      velocity = 0;
    } else if (velocity < 8000) {
      velocity = 127;
    } else {
      velocity = ((velocity - 100000) / 1000) * -1;
    }
    // noteOn(1, ((pin - 64) * -1) + 35 , velocity);
  }
    if (keyboard_status_toc[pin] == 0 && keyboard_status_toc_old[pin] == 1) {
    keyboard_status_toc_old[pin] = 0;
    toc[pin] = 0;
  }  
}

void readPin2(int pin) {
  int oldPin = pin;
  pin = pin + 32;
  keyboard_status[pin] = digitalRead(INPUT_FIRST + 16 + (oldPin / 8) * 2);
  keyboard_status_toc[pin] = digitalRead(INPUT_FIRST + 16 + 1 + (oldPin / 8) * 2);
  // Serial.println(pin);
  // Serial.println(INPUT_FIRST + (pin / 8) * 2);

  if (keyboard_status[pin] == 1 && keyboard_status_old[pin] == 0) {
    keyboard_status_old[pin] = 1;
    tic[pin] = micros();
    //Key pressed
    // Serial.print("Key ");
    // Serial.print(pin);
    // Serial.println(" Pressed");
  }
  if (keyboard_status[pin] == 0 && keyboard_status_old[pin] == 1) {
    keyboard_status_old[pin] = 0;
    tic[pin] = 0;
    // noteOff(1, ((pin - 64) * -1) + 35 , 64);
    //Key released
    Serial.print(pin);
    Serial.println("Key released");
  }
  if (keyboard_status_toc[pin] == 1 && keyboard_status_toc_old[pin] == 0) {
    keyboard_status_toc_old[pin] = 1;
    toc[pin] = micros();
    Serial.print("Key: ");
    Serial.print(pin);
    Serial.print(" Pressed | Velocity: ");
    Serial.println(toc[pin] - tic[pin]);
    int velocity = toc[pin] - tic[pin];
    if (velocity > 100000) {
      velocity = 0;
    } else if (velocity < 8000) {
      velocity = 127;
    } else {
      velocity = ((velocity - 100000) / 1000) * -1;
    }
    // noteOn(1, ((pin - 64) * -1) + 35 , velocity);
  }
    if (keyboard_status_toc[pin] == 0 && keyboard_status_toc_old[pin] == 1) {
    keyboard_status_toc_old[pin] = 0;
    toc[pin] = 0;
  }
}