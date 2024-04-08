 #include <SoftwareSerial.h>

#define TX_PIN 0
#define RX_PIN 1

int JAK = A1;         // 적외선 센서 핀
int BUZ = 3;          // 부저 핀

int num = 0;
bool mode = false;
int cnt = 0;         // 윗몸일으키기 횟수 변수
int pre;             // 감압 값 저장 변수
int attiny = 0;       // ATTINY에 문자 보내기 위한
unsigned long curr = 0;
int standard = 0;

void Start();
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
    num = Serial.parseInt();    // RPI값 받아오기
  }
  if (num == 1) {            // 시작하라고 하면
    Start();
  }
  else if (num == 2) {         // 그만하라고 하면
    End();
  }
  else if (num == 3) {          // 적외선 거리 측정
    Check();
  }
}

void Start() {                 // 측정 시작
  if(standard==0){
    Serial.println("NO");
    num=0;
    return;
  }
  if (attiny == 0) {
    rs485.print("START");   // ATTINY에 시작 명령
    attiny = 1;
  }
  pre = rs485.read() - '0';            // 감압 값
  unsigned int ray = analogRead(JAK);  // 레이저 값
  if (pre > 1)             // 1=미지수, 수치 정확하게
    mode = true;
  if (mode && ray < standard) {
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

void End() {                   // 측정 종료
  Serial.print(cnt);
  rs485.print("END");
  cnt = 0;
  attiny = 0;
  digitalWrite(BUZ, 0);
  curr = 0;
  num = 0;
}

void Check() {                  // 적외선 기준 측정
  standard = analogRead(JAK);
  Serial.println("OK");
  num = 0;
}
