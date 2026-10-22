# Design Decisions

VisionPick Arm의 주요 설계 결정 및 근거 기록.

## 2026-10-22 | Wrist Joint Added

### Decision
기존 3축 + 그리퍼 구조를 4축 + 그리퍼 구조로 변경.

### Reason
- 손목 회전축 없이 집게 자세를 유지하기 어려움
- 물체 집기 시 집게의 수직 접근 필요
- 관절 자세와 집게 방향을 보다 독립적으로 제어하기 위함

### Updated Configuration
- J1: Base Rotation
- J2: Shoulder Pitch
- J3: Elbow Pitch
- J4: Wrist Pitch
- J5: Gripper Actuation

### Trade-offs
- 구동 모터 총 5개 필요
- 손목 모터 추가에 따른 관절 토크 증가
- 구조 및 제어 알고리즘 복잡도 증가

### Next
기구학 및 토크 계산 후 실제 모터 선정.

