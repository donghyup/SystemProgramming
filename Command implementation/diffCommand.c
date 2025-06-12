#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 4096
#define MAX_LINE_LEN 1024

// 파일을 라인 단위로 읽어 메모리에 저장
int read_lines(const char *filename, char lines[][MAX_LINE_LEN], int *count) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;
    int n = 0;
    while (fgets(lines[n], MAX_LINE_LEN, fp)) {
        n++;
        if (n >= MAX_LINES) break;
    }
    fclose(fp);
    *count = n;
    return 1;
}

// 기본 diff: 라인별로 단순 비교
void simple_diff(char a[][MAX_LINE_LEN], int n1, char b[][MAX_LINE_LEN], int n2) {
    int n = n1 > n2 ? n1 : n2;
    for (int i = 0; i < n; i++) {
        if (i >= n1) {
            printf("> %s", b[i]); // b에만 있는 줄
        } else if (i >= n2) {
            printf("< %s", a[i]); // a에만 있는 줄
        } else if (strcmp(a[i], b[i]) != 0) {
            printf("< %s", a[i]);
            printf("> %s", b[i]);
        }
    }
}

// 통합 diff(unified diff): -u 옵션
void unified_diff(char a[][MAX_LINE_LEN], int n1, char b[][MAX_LINE_LEN], int n2) {
    printf("--- file1\n");
    printf("+++ file2\n");
    int i = 0, j = 0;
    while (i < n1 || j < n2) {
        if (i < n1 && j < n2 && strcmp(a[i], b[j]) == 0) {
            // 동일한 줄
            printf(" %s", a[i]);
            i++; j++;
        } else {
            // 차이 발생: 삭제/추가/변경
            if (i < n1) printf("-%s", a[i]);
            if (j < n2) printf("+%s", b[j]);
            i++; j++;
        }
    }
}

int main(int argc, char *argv[]) {
    int unified = 0;
    char *file1 = NULL, *file2 = NULL;

    // 명령줄 인자 파싱
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-u") == 0) unified = 1;
        else if (!file1) file1 = argv[i];
        else if (!file2) file2 = argv[i];
    }

    if (!file1 || !file2) {
        fprintf(stderr, "사용법: %s [-u] 파일1 파일2\n", argv[0]);
        return 1;
    }

    char lines1[MAX_LINES][MAX_LINE_LEN];
    char lines2[MAX_LINES][MAX_LINE_LEN];
    int n1 = 0, n2 = 0;

    if (!read_lines(file1, lines1, &n1) || !read_lines(file2, lines2, &n2)) {
        fprintf(stderr, "파일을 읽을 수 없습니다.\n");
        return 1;
    }

    if (unified)
        unified_diff(lines1, n1, lines2, n2);
    else
        simple_diff(lines1, n1, lines2, n2);

    return 0;
}
/*
read_lines: 파일을 줄 단위로 읽어 배열에 저장

simple_diff: 기본 diff 방식, 각 줄 비교 후 다르면 <, >로 표시

unified_diff: -u 옵션 시, 통합 diff 포맷(---, +++, -, +, 공백)으로 출력

main: 옵션/파일명 파싱, 적절한 비교 함수 호출
*/

