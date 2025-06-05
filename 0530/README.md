
# 📘 grep 명령어 정리

`grep`은 파일이나 입력에서 특정 문자열 또는 정규표현식과 일치하는 줄을 찾아 출력하는 명령어입니다. 로그 분석, 텍스트 필터링, 조건 검색 등에 자주 사용됩니다.

---

## 📌 기본 문법

```bash
grep [옵션] '패턴' 파일명
```

예시:

```bash
grep "hello" file.txt
```
> file.txt 파일에서 "hello"가 포함된 줄을 출력합니다.

---

## ⚙️ 자주 사용하는 옵션

| 옵션 | 설명 |
|------|------|
| `-i` | 대소문자 무시 (Ignore case) |
| `-v` | 일치하지 않는 줄 출력 (반전 검색) |
| `-r`, `-R` | 디렉터리 재귀적으로 검색 |
| `-n` | 줄 번호와 함께 출력 |
| `-l` | 일치하는 파일 이름만 출력 |
| `-c` | 일치하는 줄의 개수 출력 |
| `-o` | 일치하는 문자열만 출력 |
| `--color=auto` | 일치하는 부분을 색상으로 강조 |

---

## 📝 사용 예시

### ✅ 기본 검색
```bash
grep "main" program.c
```
> program.c에서 "main"이 포함된 줄 출력

### ✅ 대소문자 구분 없이 검색
```bash
grep -i "main" program.c
```

### ✅ 문자열이 없는 줄 출력
```bash
grep -v "error" log.txt
```

### ✅ 줄 번호 출력
```bash
grep -n "printf" main.c
```

### ✅ 하위 디렉터리 전체에서 검색
```bash
grep -r "TODO" ./src
```

### ✅ 일치하는 파일 이름만 출력
```bash
grep -l "config" *.conf
```

---

## 🔍 정규표현식 사용 예시

```bash
grep "^start" file.txt     # 'start'로 시작하는 줄
grep "end$" file.txt       # 'end'로 끝나는 줄
grep "[0-9]\{3\}" file.txt # 세 자리 숫자 포함한 줄
```

---

## 💡 참고사항

- `grep -E`: 확장 정규표현식 사용 (`egrep`과 동일)
- `grep -F`: 리터럴 문자열로 검색 (`fgrep`과 동일)
- `man grep`: 전체 옵션 및 설명 확인


