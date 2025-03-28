# 🎯 3주차 실습 내용

## 📁 파일과 경로 개념
- **모든 파일의 묶음**: 디렉토리 📂
- **경로를 하나하나 지정하는 것**: path 🛤️
- **Path**: 실행 파일에 경로를 지정하는 것 ⚙️

## 🛠️ 기본 명령어 사용
### 🔍 사용자 정보 확인
```bash
$ whoami
chang
```

### 📂 디렉토리 내용 확인
```bash
ls
```
출력 예시:
```
공개 다운로드 문서 바탕화면 비디오 사진 음악 템플릿
```

## 🔎 `whoami` vs `ls /home/`
- `whoami` : 현재 사용자의 계정을 확인하는 명령어 👤
- `ls /home/` : `/home/` 디렉터리의 파일 및 폴더를 나열하는 명령어 📄

## 📜 파일의 종류 (crw-rw----)
1. **일반 파일(ordinary file)** 📄
   - 데이터를 가지고 있으며 디스크에 저장됨
   - 예: 텍스트 파일, 이진 파일
2. **디렉토리(directory)** 📂
   - 파일을 계층적으로 조직화하는 특수 파일
   - 내부에 파일 및 서브디렉토리 포함
3. **장치 파일(device special file)** 🎛️
   - 물리적인 장치에 대한 내부적인 표현
   - 키보드(stdin), 모니터(stdout), 프린터 등
4. **심볼릭 링크 파일** 🔗
   - 특정 파일을 가리키는 경로명을 저장하는 파일

## 🔍 파일 검색 명령어
- `ls /usr/bin/??` → 두 글자로 된 파일 찾기 🔎
- `ls /usr/bin/l*` → `l`로 시작하는 모든 파일 찾기 📑

## 📝 파일 관련 리눅스 명령어
- `head hello.c` → 파일의 **처음 몇 줄** 출력
- `tail hello.c` → 파일의 **마지막 몇 줄** 출력
- `wc hello.c` → 파일의 **단어, 줄, 문자 개수** 확인
