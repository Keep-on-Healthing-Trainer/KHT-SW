// Pro Micro

#include <SoftwareSerial.h>

#define TX_PIN 3
#define RX_PIN 2

int PRES = A3;
int protocol = 0;

int pre = 0;   // 감압 값
SoftwareSerial rss485(RX_PIN, TX_PIN);
 

void setup() {
  rss485.begin(115200);
  Serial.begin(115200);
  pinMode(PRES, INPUT);
}

void loop() {
  if (rss485.available()) {
    protocol = rss485.parseInt();
    if(protocol==2){
      delay(80);
      protocol=2;
    }
    Serial.println(protocol); 
  }
  
  if (protocol == 1) {
    pre = analogRead(PRES);
    rss485.print(pre); 
    protocol = 0;
  }
}
