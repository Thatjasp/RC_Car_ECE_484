#define R_PWM_PIN A5
#define L_PWM_PIN A4
#define R_FORWARD_PIN 4
#define R_BACKWARD_PIN 5
#define L_FORWARD_PIN 7
#define L_BACKWARD_PIN 6
#define NRF_CE 8
#define NRF_CSN 10
#define NRF_SCK 13
#define NRF_MOSI 11
#define NRF_MISO 12

#include <SPI.h>
#include "RF24.h"




// instantiate an object for the nRF24L01 transceiver
// controller 9,10
// rc_car 8,10
RF24 radio(NRF_CE, NRF_CSN);  // using pin 7 for the CE pin, and pin 8 for the CSN pin

// Let these addresses be used for the pair
uint8_t address[][6] = { "CONTRL", "RC_CAR" };
// It is very helpful to think of an address as a path instead of as
// an identifying device destination

// to use different addresses on a pair of radios, we need a variable to
// uniquely identify which address this radio will use to transmit
bool radioNumber = 1;  // 0 uses address[0] to transmit, 1 uses address[1] to transmit

// Used to control whether this node is sending or receiving
bool role = false;  // true = TX role, false = RX role

// For this example, we'll be using a payload containing
// a single float number that will be incremented
// on every successful transmission
uint8_t payload;

void init_nrf() {
  Serial.begin(9600);
  // initialize the transceiver on the SPI bus
  if (!radio.begin()) {
    while (1) {Serial.println("did not print");}  // hold in infinite loop
  }
  radioNumber = 1; // For Car
  // radio.flush_rx();
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
  radio.startListening();  // put radio in RX mode

  // For debugging info
  // printf_begin();             // needed only once for printing details
  // radio.printDetails();       // (smaller) function that prints raw register values
  // radio.printPrettyDetails(); // (larger) function that prints human readable data
  
}
void init_motor() {
  pinMode(R_PWM_PIN, OUTPUT);
  pinMode(L_PWM_PIN, OUTPUT);

  pinMode(R_FORWARD_PIN, OUTPUT);
  pinMode(R_BACKWARD_PIN, OUTPUT);

  pinMode(L_FORWARD_PIN, OUTPUT);
  pinMode(L_BACKWARD_PIN, OUTPUT);
}
void setup() {
  init_nrf();
  init_motor();
}  // setup

void processPayload() {
  switch(payload) {
    case 0:
      stop();
      break;
    case 1:
      forward();
      break;
    case 3:
      // This turns right, the functions are incorrect
      left();
      break;
    case 5:
      backward();
      break;
    case 7:
    // this turns left, thefunctions are incorrect
      right();
      break;
    case 11:
      stop();
      break;
 }
}

void receiveProcess() {
  // This device is a RX node
  if (radio.available()) {              // is there a payload? get the pipe number that recieved it
      radio.read(&payload, sizeof(uint8_t));             // fetch payload from FIFO
      Serial.println(payload);
      processPayload();
  }
  else {
    Serial.println("nothin");
    // delay(300);
  }
}

void loop() {
  receiveProcess();
  delay(100);
}  // loop

void stop() {
	digitalWrite(L_FORWARD_PIN, LOW);
	digitalWrite(L_BACKWARD_PIN, LOW);
	analogWrite(L_PWM_PIN, 0);
	digitalWrite(R_FORWARD_PIN, LOW);
	digitalWrite(R_BACKWARD_PIN, LOW);
	analogWrite(R_PWM_PIN, 0);
}

void forward() {
	digitalWrite(L_FORWARD_PIN, HIGH);
	digitalWrite(L_BACKWARD_PIN, LOW);
	analogWrite(L_PWM_PIN, 200);
  
	digitalWrite(R_FORWARD_PIN, HIGH);
	digitalWrite(R_BACKWARD_PIN, LOW);
	analogWrite(R_PWM_PIN, 200);
}
void backward() {
  digitalWrite(L_FORWARD_PIN, LOW);
	digitalWrite(L_BACKWARD_PIN, HIGH);
	analogWrite(L_PWM_PIN, 200);
  
	digitalWrite(R_FORWARD_PIN, LOW);
	digitalWrite(R_BACKWARD_PIN, HIGH);
	analogWrite(R_PWM_PIN, 200);
}
void right() {
  digitalWrite(L_FORWARD_PIN, HIGH);
	digitalWrite(L_BACKWARD_PIN, LOW);
	analogWrite(L_PWM_PIN, 200);
	digitalWrite(R_FORWARD_PIN, LOW);
	digitalWrite(R_BACKWARD_PIN, LOW);
	analogWrite(R_PWM_PIN, 0);
}

void left() {
  digitalWrite(L_FORWARD_PIN, LOW);
	digitalWrite(L_BACKWARD_PIN, LOW);
	analogWrite(L_PWM_PIN, 0);
	digitalWrite(R_FORWARD_PIN, HIGH);
	digitalWrite(R_BACKWARD_PIN, LOW);
	analogWrite(R_PWM_PIN, 200);
}
  
