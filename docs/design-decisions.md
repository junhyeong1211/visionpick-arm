# Design Decisions

VisionPick Arm V1의 주요 설계 결정, 변경 이유 및 기술적 검토 기록.

**Last Updated:** 2026-10-27

> 설계값은 확정값(Confirmed), 임시값(Assumed), 미확정(Pending)으로 구분함.

---

## 2026-10-22 | 4-DOF Robot Configuration

### Decision

기존 3축 로봇팔을 **4축 + 1축 그리퍼** 구조로 변경.

### Reason

- 손목 회전축 없이 집게의 방향을 독립적으로 제어하기 어려움
- 블록을 수직 방향으로 접근해 파지하기 위한 Wrist Pitch 필요
- 어깨·팔꿈치 자세 변화에 따른 집게 방향 보정 기능 확보

### Configuration

| Joint | Motion |
|---|---|
| J1 | Base Yaw |
| J2 | Shoulder Pitch |
| J3 | Elbow Pitch |
| J4 | Wrist Pitch |
| J5 | Gripper Actuation |

총 5개 서보모터 사용.

### Trade-offs

- 손목 모터 추가에 따른 상위 관절 토크 증가
- 제어 알고리즘 및 구조 설계 복잡도 증가
- 집게 자세 제어 능력 확보

**Status:** Concept CAD 관절 구동 검증 완료.

---

## 2026-10-22 | Industrial Open-Frame Design

### Decision

산업용 미니 로봇팔과 개방형 기계 구조를 결합한 디자인 채택.

### Design Requirements

- 오렌지·블랙 색상 조합
- 관절·모터·체결부가 드러나는 오픈 프레임
- PLA+ 기반 3D 프린팅
- 분해·재조립이 가능한 볼트 체결 구조
- 링크·모터 브래킷·집게의 모듈화

### Manufacturing

연세대학교 메이커스페이스의 3D 프린터 활용.

2026년 장비 이용료 및 기본 흰색 PLA+ 무료 제공 확인. 검정·주황 필라멘트 보유 확인.

최종 출력은 모터 실측, 조립 공차 및 구조 검증 이후 진행.

---

## 2026-10-22 | Vision & Workspace

### Vision System

- DFRobot HUSKYLENS 1 사용
- Color Recognition 우선 적용
- Red: ID1 / Blue: ID2
- 로봇팔과 분리된 상부 고정 카메라 채택

### Workspace

| Parameter | Trial Value |
|---|---:|
| Workbench | 300 × 300mm |
| Camera Height | 250mm |
| Pickup Radius | 약 140mm |
| Target Object | 30mm Cube |

카메라 높이 20/25/30cm를 비교했으며, 모두 대상이 화면에 들어오는 것을 확인. 초기 설치 높이는 25cm로 선정.

집기 지점과 배출 지점을 유사한 수평 반경에 배치해 베이스 회전 동작을 단순화하는 방향 채택.

**Pending:** 카메라 좌표 보정 및 전체 작업 영역 검증.

---

## 2026-10-23 ~ 10-26 | Kinematics Validation

### Concept Parameters

| Parameter | Value |
|---|---:|
| L1 | 100mm |
| L2 | 80mm |
| Shoulder Height | 70mm |

위 값은 기구학 검증용 임시 치수임.

### Validation Results

- FK·IK 수학 계산 완료
- Elbow-up / Elbow-down 해 계산
- 베이스 회전 및 손목 자세 보정식 검토
- Fusion 360에서 FK 목표 좌표 `(155.885, 80)mm` 재현
- Fusion 360에서 IK 목표 좌표 `(100, 150)mm` 재현

위 좌표는 XZ 평면 기준.

Fusion Joint 영점·회전 방향과 수학적 관절 각도의 차이를 확인함.

**Pending:** 실제 3차원 좌표계, 관절 제한, TCP 오프셋 및 서보 입력각 보정.

---

## 2026-10-26 | Preliminary Torque Analysis

### Decision

수평 자세의 정적 중력 토크를 기본으로 관절별 모터 요구조건 산정.

### Assumptions

- Payload: 20g
- J3 Motor: 55g
- J4 Motor: 13.4g (MG90S 후보 적용)
- Gripper Assembly: 40g 기본 가정
- Gravity: 9.81m/s²
- J2~J4 설계 여유: 2.5배

### Revised Results — Hardware R02

| Joint | Design Torque |
|---|---:|
| J2 Shoulder | 5.855 kgf·cm |
| J3 Elbow | 2.019 kgf·cm |
| J4 Wrist | 0.400 kgf·cm |

그리퍼 질량 변화에 따른 민감도:

| Gripper Mass | J2 | J3 | J4 |
|---|---:|---:|---:|
| 40g | 5.855 | 2.019 | 0.400 |
| 60g | 6.855 | 2.519 | 0.500 |
| 80g | 7.855 | 3.019 | 0.600 |

단위: kgf·cm, 설계 여유 포함.

### Additional Analysis

- J1: 점질량 근사에 따른 회전 관성 및 가속 토크 계산
- J5: 마찰 기반 손가락당 파지력 목표 0.4905N 산정
- 실제 모터 질량 변경을 반영해 기존 하중 계산 수정

### Limitations

- 최종 CAD 질량·무게중심 미반영
- 동적 하중, 마찰, 발열 미검증
- 링크 강도 및 처짐 미검증
- 집게의 실제 전달 기구비 미반영

---

## 2026-10-26 ~ 10-27 | Gripper Concept

### Decision

SG90 서보모터 1개를 사용하는 **양쪽 링크 연동형 2핑거 집게** 채택.

### Concept Parameters

| Parameter | Trial Value |
|---|---:|
| Body Width | 72mm |
| Body Height | 18mm |
| Finger Pivot Distance | 54mm |
| Finger Length | 40mm |
| Target Opening Width | 45~50mm |

- `Gripper_Concept_v01` 스케치 제작
- 좌우 손가락 회전축 및 개폐각 구상
- SG90 참고 Component 제작 및 배치
- 전체 질량 60g 이하 목표
- 80g 조건까지 하중 검토

현재 집게는 Concept 단계이며, 실제 연동 링크 및 파지 성능은 미검증.

---

## 2026-10-27 | Servo Motor Selection

### Selected Configuration

| Joint | Servo Motor | Status |
|---|---|---|
| J1 Base | MG996R Compatible | Ordered |
| J2 Shoulder | FEETECH FT5330M | Ordered |
| J3 Elbow | MG996R Compatible | Ordered |
| J4 Wrist | TowerPro MG90S | Ordered |
| J5 Gripper | SG90 | Already Owned |

### Selection Rationale

**J1 — MG996R Compatible**

비용 절감을 고려한 베이스 구동 후보. 실제 가동 범위와 베이스 지지 구조 확인 필요.

**J2 — FEETECH FT5330M**

어깨 관절의 상대적으로 큰 중력 토크를 고려해 선정. 6V 정격 부하 토크 자료 확보.

**J3 — MG996R Compatible**

계산된 팔꿈치 하중을 기준으로 선정한 경제형 후보. 실제 부하 유지 성능 검증 필요.

**J4 — TowerPro MG90S**

말단 질량 감소와 위치 제어를 고려한 경량 서보 선정.

**J5 — SG90**

기존 보유 부품 재사용. 실제 집게 파지력 시험 이후 적합성 판정.

### Pending Verification

- 정확한 제품 모델 및 동작 전압
- 실물 치수·출력축·서보혼·장착홀
- 각도 제어 범위 및 반복 오차
- 부하 유지·발열·전류 측정

모터 주문 완료와 최종 성능 검증 완료를 구분함.

---

## 2026-10-27 | Power & Mechanical Components

### Power Architecture

- Arduino UNO는 USB로 전원 공급
- 서보모터는 외부 안정화 DC 전원 사용
- Arduino와 모터 전원의 GND 공통 연결
- 6V/5V 분리 전원 구조 우선 검토

어댑터, 전압 변환 모듈, 분배 단자 및 보호장치는 구매 확정 전.

### Mechanical Support

- 베이스 회전부의 하중 지지 구조 필요
- 어깨·팔꿈치 관절의 양쪽 지지 구조 검토
- 서보 출력축에 굽힘 하중이 집중되지 않도록 설계
- 베어링, 축, 볼트 및 스페이서는 최종 조립 구조에 따라 선정

608ZZ·625ZZ는 후보 규격이며 확정 규격이 아님.

---

## 2026-10-27 | Modular Architecture

### Decision

향후 제어 소프트웨어와 기계 구조를 개선할 수 있도록 모듈형 CAD 설계 채택.

### Module Structure

- Base Module
- Shoulder & Arm Module
- Elbow Module
- Wrist Module
- Gripper Module

### Design Guidelines

- 모터 하우징과 링크 분리
- 교체 가능한 모터 장착 브래킷
- 분리 가능한 집게 장착 인터페이스
- 볼트 기반 체결 및 유지보수 구조
- 서보 배선 교체와 접근성 고려

ROS2·AI 비전 고도화 및 새로운 집게 개발 시 기존 부품을 최대한 재사용하는 것을 목표로 함.

모듈별 체결 인터페이스의 최종 치수는 제작용 CAD에서 정의 예정.

---

## Open Decisions

- [ ] J1~J5 실물 모터 치수·동작 범위 검증
- [ ] 전원 어댑터·변환기·보호회로 선정
- [ ] 베어링·축·볼트 규격 확정
- [ ] 그리퍼 연동 링크 및 실제 질량 검증
- [ ] 제작용 CAD 질량·강도·처짐 분석
- [ ] 모듈별 체결 인터페이스 확정
- [ ] 전체 Assembly 간섭 검증
- [ ] 3D 프린팅 시험 출력 및 조립성 검증

**Next Phase:** `VisionPick_Assembly_v01` 제작용 CAD 설계.
