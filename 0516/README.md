# 7,8장 내용 정리


cat << 'EOF' > README.md
# 📁 유닉스 파일 유틸리티 및 파일 잠금 실습

## 🗂️ Chapter 7: 파일 및 레코드 잠금

### 🔒 파일 잠금 (File Locking)

- **공유 잠금 (Shared Lock, \`LOCK_SH\`)**  
  여러 프로세스가 동시에 읽을 수 있음.  
- **배타 잠금 (Exclusive Lock, \`LOCK_EX\`)**  
  단 하나의 프로세스만 접근 가능.  
- **잠금 해제 (\`LOCK_UN\`)**  
  파일 잠금 해제.

#### ✅ flock() 함수

\`\`\`c
int flock(int fd, int operation);
\`\`\`

- 전체 파일을 잠금.
- \`LOCK_NB\` 옵션을 통해 non-blocking 잠금 가능.

#### ✅ fcntl() 함수

\`\`\`c
int fcntl(int fd, int cmd, struct flock *lock);
\`\`\`

- 레코드(파일 일부) 단위로 잠금 가능.
- \`F_SETLK\`, \`F_SETLKW\`, \`F_GETLK\` 등의 명령어 사용.
- \`struct flock\` 구조체로 세부 영역 설정.

#### ✅ lockf() 함수

\`\`\`c
int lockf(int fd, int cmd, off_t len);
\`\`\`

- 현재 파일 위치부터 \`len\` 길이만큼 잠금 설정.
- \`F_LOCK\`, \`F_TLOCK\`, \`F_ULOCK\`, \`F_TEST\` 명령어 사용.

### 🔐 권고 잠금 vs 강제 잠금

| 구분 | 권고 잠금 (Advisory) | 강제 잠금 (Mandatory) |
|------|----------------------|------------------------|
| 규칙 강제 여부 | 자발적 준수 | 커널 강제 |
| 시스템 부하 | 낮음 | 상대적으로 높음 |
| 사용 환경 | Linux, BSD 등 | System V, Solaris 등 |

---

## 🛠️ Chapter 8: 파일 유틸리티 명령어 정리

### 🔍 find: 조건에 맞는 파일 검색

\`\`\`bash
$ find [경로] [옵션] [조건] -print
\`\`\`

- \`-name\`, \`-type\`, \`-user\`, \`-size\`, \`-perm\`, \`-mtime\`, \`-atime\` 등 다양한 조건 사용.
- \`-exec\` 옵션으로 검색된 파일에 명령 적용 가능.

### 🔎 grep: 문자열 검색

\`\`\`bash
$ grep [옵션] "패턴" 파일명
\`\`\`

- \`-i\`, \`-n\`, \`-v\`, \`-c\`, \`-w\`, \`-l\` 등 다양한 옵션 존재.
- 정규 표현식으로 고급 검색 가능.
- 파이프(\`|\`)와 함께 활용 시 효율 상승!

### 🔃 sort: 텍스트 파일 정렬

\`\`\`bash
$ sort [-옵션] 파일명
\`\`\`

- 기본: 오름차순 정렬
- \`-r\`: 내림차순
- \`-k\`, \`-t\`, \`-n\`, \`-f\`, \`-o\` 등 다양한 옵션으로 필드 기준 정렬 가능

### 🔀 파일 비교 도구

- **cmp**: 바이트 단위 비교, 차이 위치 출력
- **diff**: 줄 단위 비교, 수정사항 출력 (a: 추가, d: 삭제, c: 변경)

### ✂️ 기타 파일 조작

- **split**: 파일 분할  
  \`$ split -l 10 you.txt\`
- **cat**: 파일 병합 (줄 순서 유지)  
  \`$ cat xaa xab > xmerge\`
- **paste**: 줄 단위로 파일 병합  
  \`$ paste line.txt you.txt > result.txt\`

---

## 🌟 핵심 요약

| 명령어 | 기능 요약 |
|--------|-----------|
| \`find\` | 파일 조건 검색 |
| \`grep\` | 문자열 검색 |
| \`sort\` | 정렬 |
| \`cmp\` / \`diff\` | 파일 비교 |
| \`split\` / \`cat\` / \`paste\` | 파일 분할 및 병합 |
| \`flock\`, \`fcntl\`, \`lockf\` | 파일 및 레코드 잠금 |
