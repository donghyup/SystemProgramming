#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 4096
#define MAX_LINE_LEN 1024

// 오름차순 비교 함수
int cmp_asc(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

// 내림차순(역순) 비교 함수
int cmp_desc(const void *a, const void *b) {
    return strcmp(*(const char **)b, *(const char **)a);
}

int main(int argc, char *argv[]) {
    char *lines[MAX_LINES];
    int n = 0;
    int reverse = 0;
    char *filename = NULL;
    FILE *fp;

    // 명령줄 인자 파싱
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            reverse = 1;
        } else {
            filename = argv[i];
        }
    }

    // 파일 열기(없으면 표준 입력)
    if (filename) {
        fp = fopen(filename, "r");
        if (!fp) {
            perror("파일을 열 수 없습니다");
            return 1;
        }
    } else {
        fp = stdin;
    }

    // 줄 단위로 읽기
    char buf[MAX_LINE_LEN];
    while (fgets(buf, sizeof(buf), fp)) {
        lines[n] = strdup(buf);
        n++;
        if (n >= MAX_LINES) break;
    }
    if (fp != stdin) fclose(fp);

    // 정렬
    qsort(lines, n, sizeof(char *), reverse ? cmp_desc : cmp_asc);

    // 출력
    for (int i = 0; i < n; i++) {
        printf("%s", lines[i]);
        free(lines[i]);
    }
    return 0;
}
/*
줄을 배열에 저장한 뒤, qsort로 정렬(오름차순/내림차순)

-r 옵션이 있으면 내림차순, 없으면 오름차순

파일명을 지정하지 않으면 표준 입력에서 읽음

메모리 해제까지 처리
*/
