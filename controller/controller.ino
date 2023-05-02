#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include <JoystickShield.h> // include JoystickShield Library

JoystickShield joystickShield; // create an instance of JoystickShield object


#define CE 9
#define CSN 10

// rc_car 8,10
RF24 radio(CE, CSN);  
uint8_t address[][6] = { "CONTRL", "RC_CAR" };

uint8_t payload = 0;

void init_nrf() {
  if (!radio.begin()) {
    while (1) {}  
  }
  int radioNumber = 0; // For Controller

  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.

  radio.setPayloadSize(sizeof(payload));  // float datatype occupies 4 bytes

  radio.openWritingPipe(address[radioNumber]);  // always uses pipe 0

  radio.openReadingPipe(1, address[!radioNumber]);  // using pipe 1


  radio.stopListening();  // put radio in TX mode

}

void init_joystick() {
  Serial.begin(9600);
  delay(100);
  // new calibration function
  joystickShield.calibrateJoystick();
    // Joystick Callbacks
  joystickShield.onJSUp(&up);
  joystickShield.onJSRightUp(&rightUp);
  joystickShield.onJSRight(&right);
  joystickShield.onJSRightDown(&rightDown);
  joystickShield.onJSDown(&down);
  joystickShield.onJSLeftDown(&leftDown);
  joystickShield.onJSLeft(&left);
  joystickShield.onJSLeftUp(&leftUp);

    // Button Callbacks
  joystickShield.onJoystickButton(&joystickButton);
  joystickShield.onUpButton(&upButton);
  joystickShield.onRightButton(&rightButton);
  joystickShield.onDownButton(&downButton);
  joystickShield.onLeftButton(&leftButton);
  joystickShield.onFButton(&FButton);
  joystickShield.onEButton(&EButton);
}

void sendPayload() {
  bool report;
  int failures = 0;
  report = radio.writeFast(&payload, sizeof(uint8_t));
  if (!report) {
    radio.reUseTX();
  } 

  Serial.println(payload);
  payload = 0;
  delay(100);
}

void setup() {
  init_nrf();
  init_joystick();
}  // setup

void loop() {
    joystickShield.processCallbacks(); 

}

/** Define Callback Function **/
void up() {
    // Serial.println("Up from callback");
    payload = 1;
    sendPayload();
}

void rightUp() {
    // Serial.println("Right Up from callback");
    payload = 20;
    sendPayload();
}

void right() {
    // Serial.println("Right from callback");
    payload = 3;
    sendPayload();
}

void rightDown() {
    // Serial.println("Right Down from callback");
    payload = 4;
    sendPayload();
}

void down() {
    // Serial.println("Down from callback");
    payload = 5;
    sendPayload();
}

void leftDown() {
    // Serial.println("Left Down from callback");
    payload = 6;
    sendPayload();
}

void left() {
    // Serial.println("Left from callback");
    payload = 7;
    sendPayload();
}

void leftUp() {
    // Serial.println("Left Up from callback");
    payload = 8;
    sendPayload();
}

void joystickButton() {
    // Serial.println("Joystick from callback");
    payload = 9;
    sendPayload();
}

void upButton() {
    // Serial.println("Up Button from callback");
    payload = 10;
    sendPayload();
}

void rightButton() {
    // Serial.println("Right Button from callback");
    payload = 11;
    sendPayload();
}

void downButton() {
    // Serial.println("Down Button from callback");
    payload = 12;
    sendPayload();
}

void leftButton() {
    // Serial.println("Left Button from callback");
    payload = 13;
    sendPayload();
}

void FButton() {
    // Serial.println("F Button from callback");
    payload = 14;
    sendPayload();
}

void EButton() {
    // Serial.println("E Button from callback");
    payload = 15;
    sendPayload();
}

void notCenter() {
  //  Serial.println("Not Center from callback");
   // new position functions
  // Serial.print("x ");	Serial.print(joystickShield.xAmplitude());Serial.print(" y ");Serial.println(joystickShield.yAmplitude());
}
