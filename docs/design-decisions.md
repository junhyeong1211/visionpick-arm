# Design Decisions

VisionPick Arm의 주요 설계 결정 및 근거 기록.

## 2026-10-22 | Initial Design

### Mechanical Structure
- 3축 로봇팔 + 1축 그리퍼
- 산업용 오픈 프레임 디자인
- Fusion 360 직접 설계
- PLA+ 기반 3D 프린팅 예정

### Vision System
- HUSKYLENS 1 사용
- 색상 인식 우선 적용
- 독립형 상부 고정 카메라
- 카메라 높이: 250mm (임시)

### Workspace
- 작업대: 약 300 × 300mm
- 집기 반경: 약 140mm (임시)
- 집기 및 배출 위치를 유사 반경에 배치

### Gripper
- 양쪽 연동형 2핑거 구조
- SG90 서보모터 활용 검토
- 목표 개방 폭: 45~50mm

### Pending Decisions
- 링크 길이 및 관절 구조 검증
- 모터 토크 계산 및 모델 선정
- 전원 공급 구조
- 베어링 및 체결 부품 규격
- 최종 CAD 치수

