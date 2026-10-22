# Experiments

VisionPick Arm의 실험 조건 및 결과 기록.

## EXP-001 | HUSKYLENS Color Recognition
**Date:** 2026-10-22

### Objective
빨강·파랑 블록 인식 및 Arduino 좌표 수신 검증.

### Setup
- HUSKYLENS 1
- Arduino UNO (I2C)
- 30mm 정육면체 블록
- 카메라 높이: 20 / 25 / 30cm

### Results
- Red: ID1 인식 성공
- Blue: ID2 인식 성공
- 두 블록 동시 인식 성공
- C++ 목표 ID 필터링 성공
- 25cm 높이를 초기 설치 기준으로 선정

### Limitations
- 조명 변화에 따른 정량적 인식률 미측정
- 픽셀 좌표의 실제 거리 변환 미구현

---

## EXP-002 | To Be Added

