#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // sleep 함수 사용
#include <sys/stat.h>   // 파일 크기 확인용

#define DEFAULT_LINE_COUNT 10
#define MAX_LINE_LENGTH 1024
#define MAX_LINES 4096  // tail에서 최대 저장할 줄 수

// tail -n N: 파일 끝에서 N줄 출력
void tail_lines(const char *filename, int n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("파일을 열 수 없습니다");
        return;
    }

    // 순환 버퍼로 마지막 N줄 저장
    char *lines[MAX_LINES];
    int count = 0;
    char buffer[MAX_LINE_LENGTH];

    for (int i = 0; i < MAX_LINES; i++) lines[i] = NULL;

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (lines[count % n]) free(lines[count % n]);
        lines[count % n] = strdup(buffer); // 한 줄 복사 저장
        count++;
    }

    int start = count > n ? count % n : 0;
    int total = count < n ? count : n;

    for (int i = 0; i < total; i++) {
        printf("%s", lines[(start + i) % n]);
        free(lines[(start + i) % n]);
    }

    fclose(fp);
}

// tail -f: 파일 끝에서부터 실시간 출력(로그 모니터링)
void tail_follow(const char *filename, int n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("파일을 열 수 없습니다");
        return;
    }

    // 먼저 마지막 n줄 출력
    tail_lines(filename, n);

    // 파일 끝으로 이동
    fseek(fp, 0, SEEK_END);

    struct stat st;
    long last_size = 0;
    stat(filename, &st);
    last_size = st.st_size;

    char buffer[MAX_LINE_LENGTH];

    // 무한 루프: 파일 크기 변경 감지 및 새 내용 출력
    while (1) {
        stat(filename, &st);
        if (st.st_size > last_size) {
            // 새로 추가된 부분 읽기
            while (fgets(buffer, sizeof(buffer), fp)) {
                printf("%s", buffer);
            }
            last_size = st.st_size;
        }
        fflush(stdout);
        sleep(1); // 1초마다 체크
    }
    fclose(fp);
}

int main(int argc, char *argv[]) {
    int n = DEFAULT_LINE_COUNT; // 출력할 줄 수
    int follow = 0;             // -f 옵션 여부
    char *filename = NULL;

    // 명령줄 인자 파싱
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            n = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-f") == 0) {
            follow = 1;
        } else {
            filename = argv[i];
        }
    }

    if (!filename) {
        fprintf(stderr, "사용법: %s [-n 줄수] [-f] 파일명\n", argv[0]);
        return 1;
    }

    if (follow) {
        tail_follow(filename, n); // tail -f
    } else {
        tail_lines(filename, n);  // tail -n N
    }
    return 0;
}

