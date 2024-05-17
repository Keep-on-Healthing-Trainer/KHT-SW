// Micro

#include <SoftwareSerial.h>

#define TX_PIN 0
#define RX_PIN 1

int PRES = A3;
int protocol=0;

int pre = 0;   // 감압 값

SoftwareSerial rs485(RX_PIN, TX_PIN);

void setup() {
  rs485.begin(115200);
  pinMode(PRES, INPUT);
}

void loop() {
  if (rs485.available()){
    protocol = rs485.read()-'0';
  }
  if (protocol == 1) {
    pre = analogRead(PRES);
    rs485.println(pre);
  }
}
