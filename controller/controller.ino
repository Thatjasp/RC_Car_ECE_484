/*
 * See documentation at https://nRF24.github.io/RF24
 * See License information at root directory of this library
 * Author: Brendan Doherty (2bndy5)
 */

/**
 * A simple example of sending data from 1 nRF24L01 transceiver to another.
 *
 * This example was written to be used on 2 devices acting as "nodes".
 * Use the Serial Monitor to change each node's behavior.
 */
#include <SPI.h>
#include "printf.h"
#include "RF24.h"
#include <JoystickShield.h> // include JoystickShield Library

JoystickShield joystickShield; // create an instance of JoystickShield object


#define CE 9
#define CSN 10

// instantiate an object for the nRF24L01 transceiver
// controller 9,10
// rc_car 8,10
RF24 radio(CE, CSN);  // using pin 7 for the CE pin, and pin 8 for the CSN pin

// Let these addresses be used for the pair
uint8_t address[][6] = { "CONTRL", "RC_CAR" };

uint8_t payload = 0;

void init_nrf() {
  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    while (1) {}  // hold in infinite loop
  }
  int radioNumber = 0; // For Controller

  // radioNumber = 1; // For Car

  // Set the PA Level low to try preventing power supply related problems
  // because these examples are likely run with nodes in close proximity to
  // each other.
  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.

  // save on transmission time by setting the radio to only transmit the
  // number of bytes we need to transmit a float
  radio.setPayloadSize(sizeof(payload));  // float datatype occupies 4 bytes

  // set the TX address of the RX node into the TX pipe
  radio.openWritingPipe(address[radioNumber]);  // always uses pipe 0

  // set the RX address of the TX node into a RX pipe
  radio.openReadingPipe(1, address[!radioNumber]);  // using pipe 1

  // additional setup specific to the node's role

  radio.stopListening();  // put radio in TX mode

  // For debugging info
  //printf_begin();             // needed only once for printing details
  // radio.printDetails();       // (smaller) function that prints raw register values
  //radio.printPrettyDetails(); // (larger) function that prints human readable data
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
  // joystickShield.onJSnotCenter(&notCenter);

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

  // while (!(report = radio.write(&payload, sizeof(uint8_t))))  {// transmit &  printf_begin();             // needed only once for printing details save the report) {
  //   // to make this example readable in the serial monitor
  //     // slow transmissions down by 1 second
  //     Serial.println(payload);
  // }
  Serial.println(payload);
  payload = 0;
  delay(100);
}

void setup() {
  init_nrf();
  init_joystick();
}  // setup

void loop() {
    joystickShield.processCallbacks(); // you don't have do anything else
    // payload = 0;
    // sendPayload();
    // Serial.p    delay(300);rintln(payload);
    // delay(300);

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
