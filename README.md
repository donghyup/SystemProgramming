  📘 정의: >
    쉘 스크립트는 명령어를 파일에 순차적으로 작성해
    쉘이 한 줄씩 해석하며 실행하는 **스크립트 기반의 프로그래밍 도구**입니다.
    주로 Linux/Unix 환경에서 사용되며, 시스템 자동화, 설치 스크립트, 관리 작업 등에 활용됩니다.

  ⚙️ 주요 특징:
    - CLI(Command Line Interface) 기반 스크립트 언어
    - 운영체제 명령어를 그대로 활용 가능
    - 변수, 조건문, 반복문, 함수 지원
    - 대부분의 배포판에서 `bash`, `sh`, `zsh` 등 쉘을 사용

  📂 확장자:
    - `.sh` : 쉘 스크립트 파일 확장자 (예: `myscript.sh`)

  🛠️ 실행 방법:
    - `bash script.sh`
    - `./script.sh` (실행 권한 부여 필요: `chmod +x script.sh`)

  🧩 기본 구조:
    syntax: |
      #!/bin/bash
      echo "Hello, world!"

      name="홍길동"
      echo "이름은 $name 입니다."

      if [ $name == "홍길동" ]; then
        echo "주인공 등장!"
      fi

  🧠 핵심 요소:
    📌 변수:
      - 사용 예: `name="홍길동"`
      - 참조 시: `$name`
    🔁 반복문:
      - for 문:
          example: |
            for i in {1..5}; do
              echo "번호: $i"
            done
      - while 문:
          example: |
            count=0
            while [ $count -lt 3 ]; do
              echo "Count is $count"
              count=$((count + 1))
            done
    ❓ 조건문:
      - if 문:
          example: |
            if [ $USER == "root" ]; then
              echo "관리자 권한입니다."
            else
              echo "일반 사용자입니다."
            fi
    🔧 함수:
      - 선언과 호출:
          example: |
            say_hello() {
              echo "안녕하세요!"
            }

            say_hello

  📋 주요 명령어 요약:
    - echo: 문자열 출력
    - read: 사용자 입력 받기
    - if / else: 조건 분기
    - for / while: 반복 처리
    - chmod: 실행 권한 설정
    - exit: 스크립트 종료
    - $? : 직전 명령어의 종료 상태 코드

  🚀 사용 사례:
    - 시스템 초기화 자동화
    - 백업 스크립트
    - 서버 로그 정리
    - 사용자 생성 및 권한 설정
    - crontab과 연계한 정기 실행 작업

  ✅ 장점:
    - 간단하고 빠른 자동화 구현
    - 운영체제와 긴밀한 통합
    - 대부분의 시스템에 기본 탑재

  ⚠️ 단점:
    - 복잡한 로직 구현에는 부적합
    - 구문 오류에 취약 (디버깅 어려움)
    - 타 언어보다 느린 실행 속도

  🔐 보안 주의사항:
    - 사용자 입력을 다룰 때 `eval` 사용 지양
    - 외부 파일 실행 시 경로를 명확히
    - 실행 권한 설정 시 신중하게 (`chmod +x`)
