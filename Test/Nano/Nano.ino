// Nano

extern volatile unsigned long timer0_millis;
#include <SoftwareSerial.h>

#define TX_PIN 2
#define RX_PIN 3

int JAK = A0;  // 적외선 센서 핀
int BUZ = 4;   // 부저 핀

int num = 0;        // 모드 구분 변수
bool mode = false;  // 감압 판단
bool ps = false;    // 팔굽혀펴기, 스쿼트 모드
int cnt = 0;        // 윗몸일으키기 횟수 변수
int pre;            // 감압 값 저장 변수
unsigned long buzzStart = 0;
unsigned int standard = 1;

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
    if (num == 1) {          // 윗몸일으키기
      if (standard == 0) {
        Serial.print("N");
        num = 0;
      } else {
        rs485.print(1);  // Micro에 시작 명령
      }
    } else if (num == 2) {   // Stop
      End();
    } else if (num == 3) {  // 적외선 거리 측정
      Check();
    } else if( num == 4){   // 팔굽혀펴기, 스쿼트
      if(standard == 0){
        Serial.print("N");
        num = 0;
      } else {
        ps = 1;
      }
    }
  }

  if (ps == 1 && num!=2) {  // 팔굽혀펴기, 스쿼트
    int volt = map(analogRead(JAK), 0, 1023, 0, 5000);
    unsigned int ray = (27.61 / (volt - 0.1696)) * 1000;  // 레이저 값

    if (ray >= 0 && ray <= standard) {
      cnt++;
      Serial.print("cnt :");  // test
      Serial.println(cnt);  // RPI로 횟수 전송
      digitalWrite(BUZ, HIGH);
      buzzStart = millis() + 100;
    }

    if (millis() > buzzStart && buzzStart != 0) {
      digitalWrite(BUZ, LOW);
      buzzStart = 0;
    }
    delay(100);
  }

  if (rs485.available() && num!=0) {
    pre = rs485.parseInt();  // 감압 값
    Serial.println(pre);     // test
    int volt = map(analogRead(JAK), 0, 1023, 0, 5000);
    unsigned int ray = (27.61 / (volt - 0.1696)) * 1000;  // 레이저 값

    if (pre > 20) {
      mode = true;
    }

    if (mode && ray >= 0 && ray <= standard) {
      cnt++;
      Serial.print("cnt :");  // test
      Serial.println(cnt);  // RPI로 횟수 전송
      digitalWrite(BUZ, HIGH);
      buzzStart = millis() + 100;
      mode = false;
    }

    if (millis() > buzzStart && buzzStart != 0) {
      digitalWrite(BUZ, LOW);
      buzzStart = 0;
    }

  }
}

void End() {  // 측정 종료
  Serial.print(cnt);
  rs485.print(2);
  ps = 0;
  cnt = 0;
  digitalWrite(BUZ, LOW);
  buzzStart = 0;
  num = 0;
  Serial.println("END");   // test
  timer0_millis=0;
  Serial.println(millis()); // test
}

void Check() {  // 적외선 기준 측정
  int volt = map(analogRead(JAK), 0, 1023, 0, 5000);
  standard = (27.61 / (volt - 0.1696)) * 1000;
  Serial.println(standard);    // test
  Serial.print("Y");
  num = 0;
}
