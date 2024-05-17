// Nano

#include <SoftwareSerial.h>

#define TX_PIN 0
#define RX_PIN 1

int JAK = A1;         // 적외선 센서 핀
int BUZ = 2;          // 부저 핀

int num = 0;
bool mode = false;
int cnt = 0;         // 윗몸일으키기 횟수 변수
int pre;             // 감압 값 저장 변수
unsigned long curr = 0;
int standard = 0;

void End();
void Check();

SoftwareSerial rs485(RX_PIN, TX_PIN);


void setup() {
  rs485.begin(115200);
  Serial.begin(115200);

  pinMode(JAK, INPUT);
  pinMode(BUZ, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    num = Serial.parseInt();
    if (num == 1) {
      if (standard == 0) {
        Serial.println("N");
        num = 0;
      }
      else
        rs485.println(num);  // Micro에 시작 명령
    }
    else if (num == 2) {         // 그만하라고 하면
      End();
    }
    else if (num == 3) {          // 적외선 거리 측정
      Check();
    }
  }

  if (rs485.available()) {
    pre = rs485.read() - '0';            // 감압 값
    Serial.println(pre);
    unsigned int ray = analogRead(JAK);  // 레이저 값
    if (pre > 10)             // 10=미지수, 수치 정확하게
      mode = true;
    if (mode && ray <= standard) {
      cnt++;
      Serial.println(cnt);     // RPI로 횟수 전송
      digitalWrite(BUZ, 1);
      curr = millis();
      mode = false;
    }
    if (curr >= 100) {
      digitalWrite(BUZ, 0);
      curr = 0;
    }
  }
}

void End() {                   // 측정 종료
  Serial.println(cnt);
  rs485.println(0);
  cnt = 0;
  digitalWrite(BUZ, 0);
  curr = 0;
  num = 0;
}

void Check() {                  // 적외선 기준 측정
  standard = analogRead(JAK);
  Serial.println("Y");
  num = 0;
}
