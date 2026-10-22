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


## EXP-002 | 4-DOF CAD Motion Test
**Date:** 2026-10-22

### Objective
Fusion 360에서 4축 로봇팔의 관절 동작 및 물체 접근 가능성 확인.

### Setup
- L1: 100mm
- L2: 80mm
- Shoulder Height: 70mm
- Target Object: 30mm Cube
- Tool: Fusion 360

### Results
- Base / Shoulder / Elbow / Wrist 구동 성공
- Revolute 및 Rigid Joint 연결 확인
- 30mm 블록에 대한 집게 접근 자세 구현
- 손목 회전을 통한 집게 기울기 조절 확인

### Limitations
- 일부 목표 위치에서 자세 제약 확인
- 관절 각도 및 도달 범위 정량 분석 미완료
- 실제 하중과 모터 성능 미반영

### Next Steps
- 4축 기구학 계산
- 작업 가능 범위 검증
- 관절 토크 및 모터 선정


