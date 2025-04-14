# Unreal Engine LiveLink Plugin - MoA ( Unreal 5.2 )

![Unreal Engine](https://img.shields.io/badge/Unreal%20Engine-5.2-blue?style=flat&logo=unrealengine)
![License](https://img.shields.io/badge/license-MIT-green)

🔗 **Unreal Engine**의 **LiveLink**기능을 확장하는 플러그인입니다.  
이 플러그인은 **MotionTechnologies Inc.** 의 소프트웨어 **MoA의 실시간 모션캡처 데이터를 UE로 스트리밍**하는 데 활용됩니다.
---

## 📜 **Features**
✅ 실시간 **LiveLink 스트리밍 지원**  
✅ **모션 캡처 장비**와의 원활한 연동  
✅ **OSC 프로토콜** 지원  
✅ **Remap Asset** 제공  
✅ **Retarget Sample File** 제공  

---

## 📦 **Installation Guide**
### **1️⃣ 플러그인 다운로드**
```bash
git clone https://github.com/seunghye-study/MoALiveLink.git
```
### **1️⃣ 플러그인 폴더 복사**
MoALiveLink 폴더를 아래 경로로 이동합니다.
```bash
YourProject/Plugins/MoALiveLink
```

### **3️⃣ Unreal Engine에서 플러그인 활성화**
1. Unreal Engine Editor 실행
2. Edit → Plugins 이동
3. MoALiveLink 활성화 후 엔진 재시작

---

##  🚀  **Usage**

🔹 1. LiveLink 연결 설정
- LiveLink 패널 열기 → Window > Virtual Production > LiveLink
- Source 버튼 클릭 → MoALiveLink 선택
- IP 및 포트를 설정한 후 Connect 버튼 클릭

🔹 2. 애니메이션 블루프린트에서 사용
- Animation Blueprint 생성
- LiveLink Pose 추가 및 Output Pose에 연결
- LiveLink Pose Source에 MoALiveLink 선택택
- Animation Blueprint 컴파일

---
