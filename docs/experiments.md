# Experiments

VisionPick Arm V1의 실험 조건, 계산 결과 및 검증 기록.

- **Project:** VisionPick Arm V1
- **Configuration:** 4-DOF Robot Arm + 1-DOF Gripper
- **Last Updated:** 2026-10-27

---

## EXP-001 | HUSKYLENS Color Recognition

**Date:** 2026-10-22  
**Status:** Completed — Initial Test

### Objective
빨강·파랑 블록 인식 및 Arduino 좌표 수신 검증.

### Setup
- HUSKYLENS 1
- Arduino UNO (I2C)
- 30 × 30 × 30mm 실험용 블록
- 카메라 높이: 20 / 25 / 30cm

### Results
- Red: ID1 인식 성공
- Blue: ID2 인식 성공
- 두 블록 동시 인식 성공
- Arduino C++ 목표 ID 필터링 구현 및 동작 확인
- 25cm 높이를 초기 설치 기준으로 선정

### Limitations
- 조명 변화에 따른 정량적 인식률 미측정
- 픽셀 좌표의 실제 거리 변환 미구현

---

## EXP-002 | 4-DOF CAD Motion Test

**Date:** 2026-10-22  
**Status:** Completed — Concept Verification

### Objective
Fusion 360에서 4축 로봇팔의 관절 동작 및 물체 접근 가능성 확인.

### Setup
- L1: 100mm
- L2: 80mm
- Shoulder Height: 70mm
- Target Object: 30mm Cube
- Tool: Fusion 360

### Results
- Base / Shoulder / Elbow / Wrist 관절 구동 성공
- Revolute 및 Rigid Joint 연결 확인
- 30mm 블록에 대한 집게 접근 자세 구현
- 손목 회전을 통한 집게 기울기 조절 확인
- 기존 3축 구조에서 손목 Pitch 축을 추가한 4축 구조로 변경

### Limitations
- 일부 목표 위치에서 자세 제약 확인
- 전체 작업 가능 범위의 정량적 검증 미완료
- 모터·베어링·실제 하중 미반영
- 제작용 CAD가 아닌 Concept CAD

---

## EXP-003 | Forward & Inverse Kinematics

**Date:** 2026-10-23  
**Status:** Completed — Mathematical Validation

### Objective
4축 로봇팔의 순기구학(FK), 역기구학(IK), 손목 자세 제어 원리 검증.

### Setup
- L1: 100mm
- L2: 80mm
- Shoulder Height: 70mm
- Base / Shoulder / Elbow / Wrist 구성
- 종이 계산 및 계산기 사용

### Results
- 2링크 평면 FK 계산 완료
- 코사인 법칙 기반 IK 계산 완료
- Elbow-up / Elbow-down 두 가지 해 계산
- 베이스 회전각 및 수평 반경 계산
- 손목 자세 보정각 계산
- TCP 오프셋을 고려한 손목 목표 좌표 계산 연습

### Key Findings
- FK: 관절 각도로부터 로봇팔 끝점 위치 계산
- IK: 목표 위치로부터 관절 각도 계산
- 동일한 목표 위치에 복수의 IK 해 존재 가능
- 실제 제어에서는 관절 제한, 충돌, TCP 오프셋 고려 필요

### Limitations
- 모터 회전 제한 미반영
- 서보모터 실제 영점·회전 방향 미보정
- Arduino C++ FK·IK 함수 구현 전

---

## EXP-004 | FK·IK Verification in Fusion 360

**Date:** 2026-10-26  
**Status:** Completed — Planar Coordinate Verification

### Objective
수학적으로 계산한 FK·IK 결과와 Fusion 360 관절 구동 결과 비교.

### Setup
- Fusion File: `VisionPick_Concept_v01`
- L1: 100mm
- L2: 80mm
- Shoulder Height: 70mm
- Revolute Joint 기반 관절 구동

### Test 1 — Forward Kinematics

수학적 입력 조건:

- Shoulder: +30°
- Elbow: -60°

| Coordinate | Calculated | Fusion |
|---|---:|---:|
| X | 155.885mm | 155.885mm |
| Z | 80mm | 80mm |

**Result:** XZ 평면 좌표 일치 확인.

Fusion에서 해당 자세를 재현할 때 Shoulder Joint 입력은 -30°였음. 수학적 각도와 CAD Joint 입력각의 부호 차이 확인.

### Test 2 — Inverse Kinematics

목표 손목 좌표:

- X: 100mm
- Z: 150mm

Fusion Joint에 계산된 관절 자세 적용 후 목표 XZ 좌표 도달 확인.

**Result:** IK 목표 위치 재현 성공.

### Limitations
- J2의 Y좌표 0mm, J4의 Y좌표 -8mm로 측면 오프셋 존재
- XZ 평면 검증 결과이며 완전한 3D 정확도 검증은 아님
- 실제 서보 영점·백래시·동작 오차는 미반영

---

## EXP-005 | Preliminary Joint Torque Analysis

**Date:** 2026-10-26  
**Status:** Completed — Preliminary Calculation

### Objective
실제 모터 선정을 위한 J1~J5 구동부의 기초 하중 및 토크 분석.

### Assumptions
- L1: 100mm
- L2: 80mm
- Target Payload: 20g
- J3 Servo Mass: 55g
- J4 Servo Mass: 30g (초기 가정)
- Gripper Assembly: 40g (초기 가정)
- Gravity: 9.81m/s²
- J2~J4 설계 여유: 2.5배
- 링크를 수평으로 펼친 정적 하중 조건

### Results

| Joint | Analysis | Calculated Result |
|---|---|---:|
| J1 Base | Rotational Inertia | 0.008868 N·m |
| J2 Shoulder | Gravity Torque × 2.5 | 6.602 kgf·cm |
| J3 Elbow | Gravity Torque × 2.5 | 2.351 kgf·cm |
| J4 Wrist | Gravity Torque × 2.5 | 0.400 kgf·cm |
| J5 Gripper | Required Normal Force | 0.4905 N / Finger |

J1 계산 조건:

- Point-Mass Approximation
- Total Inertia: 0.004434 kg·m²
- Angular Acceleration: 2 rad/s²

J5 파지력 계산 조건:

- Payload: 20g
- Friction Coefficient: 0.4 (가정)
- Safety Factor: 2

### Hardware R02 — Revised Torque

J4 모터를 MG90S 13.4g으로 변경하고 J3 모터 55g은 유지.

| Joint | Revised Design Torque |
|---|---:|
| J2 Shoulder | 5.855 kgf·cm |
| J3 Elbow | 2.019 kgf·cm |
| J4 Wrist | 0.400 kgf·cm |

그리퍼 질량 증가에 따른 추가 검토:

| Gripper Mass | J2 Torque | J3 Torque | J4 Torque |
|---|---:|---:|---:|
| 40g | 5.855 | 2.019 | 0.400 |
| 60g | 6.855 | 2.519 | 0.500 |
| 80g | 7.855 | 3.019 | 0.600 |

단위: kgf·cm. 설계 여유 2.5배 포함.

### Limitations
- 실제 CAD 질량 및 무게중심 미반영
- J1 베이스 자체 관성·마찰 미반영
- 동적 하중 및 모터 발열 미검증
- PLA+ 부품 강도·처짐 미검증
- J5 실제 기구비·마찰 미검증

---

## EXP-006 | Gripper Concept Design

**Date:** 2026-10-26 ~ 2026-10-27  
**Status:** Completed — Preliminary Layout

### Objective
30mm 블록을 파지하기 위한 양쪽 연동형 집게의 기본 형상 및 서보 배치 검토.

### Setup
- Fusion File: `Gripper_Concept_v01`
- Actuator: SG90 (보유)
- Gripper Type: Symmetric Two-Finger
- Target Payload: 30mm Cube

### Concept Dimensions

| Parameter | Value |
|---|---:|
| Body Width | 72mm |
| Body Height | 18mm |
| Finger Pivot Distance | 54mm |
| Finger Length | 40mm |
| Initial Closing Angle | 약 12° |
| Pivot Reference Diameter | Ø4mm |

### Results
- 좌우 손가락 회전축 및 기준선 스케치 완료
- 개폐 자세를 위한 초기 각도 스케치 완료
- SG90 참고 Component 모델링
- SG90의 집게 중앙 배치 및 측면 위치 확인
- `Gripper_Concept_v01` 저장

### Preliminary Mass Budget

| Component | Estimated Mass |
|---|---:|
| SG90 | 9g |
| Body & Fingers | 25g |
| Linkage | 10g |
| Fasteners & Pads | 8g |
| **Total** | **52g** |

- 목표 질량: 60g 이하
- 하중 검토 상한: 80g

### Limitations
- 위 질량은 실측값이 아닌 설계 예산
- 실제 서보 혼·연동 링크 기구 미완성
- 손가락의 동시 개폐 및 파지력 미검증
- 3D 출력 공차·마찰·유격 미검증

---

## Hardware Procurement Status

**Date:** 2026-10-27

| Joint | Selected Motor | Status |
|---|---|---|
| J1 Base | MG996R Compatible | Ordered |
| J2 Shoulder | FEETECH FT5330M | Ordered |
| J3 Elbow | MG996R Compatible | Ordered |
| J4 Wrist | TowerPro MG90S | Ordered |
| J5 Gripper | SG90 | Already Owned |

모터 모델 선정 및 주문 완료. 실제 성능 검증과 제작용 장착 치수 확정은 미완료.

전원 어댑터, 전압 변환기, 베어링, 축, 체결 부품은 추가 선정·구매 예정.

---

## Next Experiments

- [ ] EXP-007 — Servo Dimension & Position Control Test
- [ ] EXP-008 — Gripper Linkage and Mass Verification
- [ ] EXP-009 — Joint Load & Structural Verification
- [ ] EXP-010 — Integrated Arm Motion Test
- [ ] EXP-011 — Camera-to-Robot Coordinate Calibration
- [ ] EXP-012 — Automated Pick & Place Test

최종 시스템 완성 후 20회 반복 Pick & Place 실험을 수행하고, 성공률·위치 오차·실패 원인을 정량적으로 기록할 예정.
