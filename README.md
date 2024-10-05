### 프로젝트 소개
- Safety Car : AED 배달 로봇
  - CCTV를 통해 쓰러진 사람을 감지하고 AED를 배달해주는 로봇
 
### 기술 스택 및 개발 환경
- detection(VS code)
  - YOLOv8
- Back-end(IntelliJ)
  - Spring boot
- Front-end(VS code)
  - React, JavaScript
- Server
  - Redis, EC2, Nginx, Docker
- ROS(VS code)
  - ROS2, python, MORAI-Simulation
 
### 담당 업무
- ROS2를 사용한 로봇 제어 및 알고리즘 생성
  - 목적지까지 path planning(a*, Dijkstra) 및 path tracking(follow the carrot)
  - laser, camera sensor fusion을 통한 Calibration
