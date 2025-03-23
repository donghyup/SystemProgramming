# 2주차 실습 내용
![image](https://github.com/user-attachments/assets/b560d445-2746-4cbe-b665-97825343a222)

## 📂 디렉토리 및 파일 관리
### 📁 디렉토리 생성 및 이동
- 디렉토리 생성: `mkdir "디렉토리 이름"` 🏗️
- 하위 디렉토리 생성 후 이동: `cd "디렉토리 이름"` 🚀

### 📜 파일 생성 및 확인
- 텍스트 파일 생성: `cat > A.txt` ✏️
- 파일 구조 확인: `tree` 🌲
  - 예시: `7 directories, 5 files` 형태로 출력됨 ✅
- 파일 내용 확인: `cat c.txt` 📖
  - 파일 내용 작성 후 저장: **Ctrl + D** (Windows 기준) 💾

## 🔧 자원 관리 명령어
- 현재 디렉토리 확인: `pwd` 📍
- 홈 디렉토리로 이동: `cd ~` 🏠
- 매뉴얼 확인: `man pwd` 📚

## 🌳 tree 명령어 설치
- 설치 명령어: `sudo apt install tree` 🛠️
- 오류 발생 시 해결 방법:
  1. 인터넷 연결 확인: `ping -c 4 google.com` 🌐
  2. 네트워크 설정 수정:
     ```bash
     sudo nano /etc/resolv.conf
     ```
  3. 다음 내용을 추가:
     ```bash
     nameserver 8.8.8.8
     nameserver 8.8.4.4
     ```
  4. 저장 후 종료: **Ctrl + X** → **Y** → **Enter** ✅

## 📌 기본 디렉토리 및 파일 개념
- `/` : 루트 디렉토리 🌍
- `~` : 홈 디렉토리 🏡
- `.` : 현재 디렉토리 🔍
- `..` : 상위 디렉토리 ⬆️

## ✨ 파일 생성 및 보기
- 파일 생성: `cat > sample.txt` 📝
- 파일 내용 확인: `cat sample.txt` 🔎
