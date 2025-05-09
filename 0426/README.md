
Unix 시스템 호출과 파일 관리 🐧
Unix 커널은 하드웨어를 효율적으로 관리하며 다양한 서비스를 제공합니다.
주요 기능과 시스템 호출을 체계적으로 정리했습니다. ✨

🛠️ Unix 커널 서비스
🔧 하드웨어 운영 관리: 시스템 자원의 최적화된 할당

📂 파일 관리: open(), read(), write() 등을 통한 파일 조작

🔄 프로세스 관리: fork(), exec(), wait()으로 프로세스 제어

🧠 메모리 관리: malloc(), free()를 이용한 동적 메모리 할당

📡 통신 관리: pipe(), socket()을 활용한 프로세스 간 통신

🖨️ 주변장치 관리: 장치 드라이버와의 인터페이스 제공

📜 시스템 호출 요약
📄 파일 관련
open(), close(), read(), write(), dup(), lseek()

🏃 프로세스 제어
fork(), exec(), exit(), wait(), getpid(), getppid()

🧠 메모리 관리
malloc(), calloc(), free()

⚠️ 시그널 처리
signal(), alarm(), kill(), sleep()

📨 IPC(프로세스 간 통신)
pipe(), socket()

📁 Unix 파일 시스템
바이트 스트림 구조 🔄: 특별한 포맷 없이 연속된 바이트 저장

범용 인터페이스 🌐: 디스크 파일, 장치(프린터, 터미널 등) 모두 파일로 추상화

🔍 파일 조작 가이드
📂 파일 열기 (open())
```c
int open(const char *path, int oflag, ... /* mode_t mode */);
```
🔧 주요 플래그

O_RDONLY 👉 읽기 전용 모드 📖

O_WRONLY 👉 쓰기 전용 모드 ✍️

O_RDWR 👉 읽기/쓰기 모드 📚

O_APPEND ➕ 파일 끝에 데이터 추가

O_CREAT 🆕 파일이 없으면 생성 (권한: mode 지정)

O_TRUNC 🧹 기존 파일 내용 삭제

O_EXCL ⛔ 파일 존재 시 오류 발생

O_NONBLOCK ⏳ 넌블로킹 모드

O_SYNC 💾 물리적 쓰기 완료 후 반환

🆕 파일 생성 (creat())
c
int creat(const char *path, mode_t mode);
동등한 호출 🔄

c
open(path, O_WRONLY | O_CREAT | O_TRUNC, mode);
파일이 존재하면 내용 삭제 후 열기 🗑️

지정된 mode로 파일 권한 설정 🔐

📖 데이터 읽기 (read())
c
ssize_t read(int fd, void *buf, size_t nbytes);
fd 파일 디스크립터에서 데이터 읽기 👉

nbytes 크기만큼 buf에 저장 📦
