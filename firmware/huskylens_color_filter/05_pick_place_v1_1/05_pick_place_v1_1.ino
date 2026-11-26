#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);

const int N = 5;
const int MINP[N] = { 500,  500,  500,  800, 1000};
const int MAXP[N] = {2500, 2500, 2500, 2500, 1440};
int cur[N];
bool stopFlag = false;

const int GRIP_OPEN = 1000;
const int GRIP_HOLD = 1420;

// J1: 1500 = 정면, 값이 작을수록 시계방향(위에서 볼 때)
// 30도 ≈ 333us (11.1us/도 추정값) → 1500 - 333 ≈ 1170
const int PLACE_J1 = 1170;   // ← 각도가 안 맞으면 이 값만 조절

//                          J1        J2    J3    J4
const int HOME[4]        = {1500,     950,  1990, 1500};
const int ABOVE[4]       = {1500,     1680, 1935, 1055};
const int DOWN[4]        = {1500,     1630, 1935, 1100};
const int PLACE_ABOVE[4] = {PLACE_J1, 1680, 1935, 1055};
const int PLACE_DOWN[4]  = {PLACE_J1, 1880, 1935, 1170};

const int ORDER_DOWN[4] = {0, 3, 2, 1};
const int ORDER_UP[4]   = {1, 2, 3, 0};

void setJ(int ch, int us) {
  us = constrain(us, MINP[ch], MAXP[ch]);
  pwm.writeMicroseconds(ch, us);
  cur[ch] = us;
}

void checkStop() {
  if (Serial.available()) {
    char c = Serial.peek();
    if (c == 'x') { Serial.read(); stopFlag = true; Serial.println("!! 정지 (현재 자세 유지)"); }
  }
}

void moveJ(int ch, int target) {
  target = constrain(target, MINP[ch], MAXP[ch]);
  while (cur[ch] != target && !stopFlag) {
    int step = constrain(target - cur[ch], -5, 5);
    setJ(ch, cur[ch] + step);
    delay(10);
    checkStop();
  }
}

void movePose(const int pose[4], const int order[4], const char* name) {
  if (stopFlag) return;
  Serial.print("-> "); Serial.println(name);
  for (int i = 0; i < 4 && !stopFlag; i++) moveJ(order[i], pose[order[i]]);
  delay(300);
}

void grip(int us, const char* name) {
  if (stopFlag) return;
  Serial.print("-> "); Serial.println(name);
  moveJ(4, us);
  delay(500);
}

void pickAndPlace() {
  stopFlag = false;
  movePose(ABOVE, ORDER_DOWN, "ABOVE");
  grip(GRIP_OPEN, "그리퍼 열기");
  movePose(DOWN, ORDER_DOWN, "DOWN");
  grip(GRIP_HOLD, "잡기");
  movePose(ABOVE, ORDER_UP, "들어 올리기");
  movePose(PLACE_ABOVE, ORDER_DOWN, "PLACE_ABOVE (시계방향 30도)");
  movePose(PLACE_DOWN, ORDER_DOWN, "PLACE_DOWN");
  grip(GRIP_OPEN, "놓기");
  movePose(PLACE_ABOVE, ORDER_UP, "PLACE_ABOVE");
  movePose(HOME, ORDER_UP, "HOME");
  Serial.println(stopFlag ? "중단됨" : "완료");
}

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  for (int ch = 0; ch < 4; ch++) { setJ(ch, HOME[ch]); delay(500); }
  setJ(4, GRIP_OPEN);
  Serial.println("준비 완료");
  Serial.println("h:HOME  a:ABOVE  d:DOWN  p:놓는위치 위  o:열기  c:잡기  g:전체 실행  x:정지");
}

void loop() {
  if (!Serial.available()) return;
  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  stopFlag = false;
  if      (cmd == "h") movePose(HOME,        ORDER_UP,   "HOME");
  else if (cmd == "a") movePose(ABOVE,       ORDER_DOWN, "ABOVE");
  else if (cmd == "d") movePose(DOWN,        ORDER_DOWN, "DOWN");
  else if (cmd == "p") movePose(PLACE_ABOVE, ORDER_DOWN, "PLACE_ABOVE");
  else if (cmd == "o") grip(GRIP_OPEN, "열기");
  else if (cmd == "c") grip(GRIP_HOLD, "잡기");
  else if (cmd == "g") pickAndPlace();
}