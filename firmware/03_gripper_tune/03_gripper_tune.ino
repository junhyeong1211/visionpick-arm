// ============================================================
// gripper_tune.ino
// 목적: J5(그리퍼) CONTACT(닿는 지점) 찾기 + GRIP_HOLD 결정 + 유지 테스트
// 날짜: 2026-11-26
// 하드웨어: Arduino UNO + PCA9685(0x40, 50Hz), J5 = SG90 (채널 4)
// J1~J4는 HOME 자세로 고정해서 팔이 처지지 않게 함
// 시리얼: 115200, 줄 끝 = New Line
// ============================================================
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

const int HOME[4] = {1500, 950, 1990, 1500};  // J1~J4 고정 자세

const int G_CH  = 4;
const int G_MIN = 1100;   // 최대 열림
const int G_MAX = 1460;   // 안전 상한 (1500에서 그리퍼가 꺾였음)
const int HOLD_OFFSET = 60;          // GRIP_HOLD = CONTACT + 60에서 시작
const unsigned long HOLD_MS = 30000; // 유지 테스트 30초

int g = G_MIN;       // 현재 J5 펄스
int contact = -1;    // 기록한 CONTACT 값 (-1 = 아직 없음)

void setG(int us) {
  g = constrain(us, G_MIN, G_MAX);
  pwm.writeMicroseconds(G_CH, g);
  Serial.print("J5 = "); Serial.println(g);
  if (us > G_MAX) Serial.println("  (안전 상한 1460에서 멈춤)");
}

// 지정 펄스로 잡고 일정 시간 유지. 'x' 입력 시 중단 후 열기
void holdTest(int us) {
  setG(us);
  Serial.println("유지 테스트 시작 (30초). 'x' = 중단");
  unsigned long t0 = millis();
  unsigned long nextPrint = 10000;
  while (millis() - t0 < HOLD_MS) {
    if (Serial.available() && Serial.read() == 'x') {
      Serial.println("!! 중단 → 열기");
      setG(G_MIN);
      return;
    }
    if (millis() - t0 >= nextPrint) {
      Serial.print("  "); Serial.print(nextPrint / 1000); Serial.println("초 경과");
      nextPrint += 10000;
    }
  }
  Serial.println("30초 완료 → 지금 SG90 온도를 손으로 확인하세요");
  Serial.println("(계속 잡고 있음. 'o' = 열기, 'f' = 힘 빼기)");
}

void printState() {
  Serial.print("현재 J5 = "); Serial.print(g);
  Serial.print("  |  CONTACT = ");
  if (contact < 0) Serial.println("미기록");
  else {
    Serial.print(contact);
    Serial.print("  |  추천 GRIP_HOLD 시작값 = ");
    Serial.println(min(contact + HOLD_OFFSET, G_MAX));
  }
}

void printHelp() {
  Serial.println("---------------- 명령 ----------------");
  Serial.println("o        : 열기 (1100)");
  Serial.println("s1360    : J5를 1360us로");
  Serial.println("+ / -    : 10us 닫기 / 열기");
  Serial.println("> / <    : 2us 닫기 / 열기");
  Serial.println("m        : 지금 값을 CONTACT로 기록");
  Serial.println("h        : CONTACT+60으로 30초 유지 테스트");
  Serial.println("h1420    : 1420us로 30초 유지 테스트");
  Serial.println("f        : J5 힘 빼기 (발열 방지)");
  Serial.println("p        : 상태 출력");
  Serial.println("--------------------------------------");
}

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);

  for (int ch = 0; ch < 4; ch++) {     // J1~J4 HOME 고정
    pwm.writeMicroseconds(ch, HOME[ch]);
    delay(500);
  }
  setG(G_MIN);                          // 그리퍼 열기
  Serial.println("준비 완료 (J1~J4 HOME 고정)");
  printHelp();
}

void loop() {
  if (!Serial.available()) return;
  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  if (cmd.length() == 0) return;

  char c = cmd.charAt(0);
  if      (c == 'o') setG(G_MIN);
  else if (c == 's') setG(cmd.substring(1).toInt());
  else if (c == '+') setG(g + 10);
  else if (c == '-') setG(g - 10);
  else if (c == '>') setG(g + 2);
  else if (c == '<') setG(g - 2);
  else if (c == 'm') {
    contact = g;
    Serial.print("CONTACT 기록 = "); Serial.println(contact);
    printState();
  }
  else if (c == 'h') {
    if (cmd.length() > 1) holdTest(cmd.substring(1).toInt());
    else if (contact < 0) Serial.println("먼저 'm'으로 CONTACT를 기록하세요");
    else holdTest(min(contact + HOLD_OFFSET, G_MAX));
  }
  else if (c == 'f') {
    pwm.setPWM(G_CH, 0, 4096);   // 신호 끔 → 서보 힘 빠짐
    Serial.println("J5 힘 빼기 (다음 명령 시 다시 켜짐)");
  }
  else if (c == 'p') printState();
  else printHelp();
}