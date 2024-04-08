#include <SoftwareSerial.h>

#define TX_PIN 4
#define RX_PIN 3

int PRES = A0;
String protocol = "";

int pre = 0;   // 감압 값

SoftwareSerial rs485(RX_PIN, TX_PIN);

void setup() {
  rs485.begin(9600);

  pinMode(PRES, INPUT);
}

void loop() {
  if (rs485.available())
    protocol += char(rs485.read());
  if (protocol == "START") {
    pre = analogRead(PRES);
    rs485.print(pre);
  }
  else if (protocol == "END" || protocol == "STARTEND") {
    protocol = "";
  }
}
