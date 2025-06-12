#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024

int main(int argc, char *argv[]) {
    int count_mode = 0;      // -c 옵션 여부
    char *filename = NULL;
    FILE *fp;

    // 명령줄 인자 파싱
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            count_mode = 1;
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

    char prev[MAX_LINE_LEN] = "";
    char curr[MAX_LINE_LEN];
    int count = 0;
    int first = 1;

    while (fgets(curr, sizeof(curr), fp)) {
        if (first || strcmp(curr, prev) != 0) {
            // 이전 줄과 다르면(혹은 첫 줄)
            if (!first) {
                if (count_mode)
                    printf("%7d %s", count, prev); // -c 옵션: 개수와 함께 출력
                else
                    printf("%s", prev);            // 기본: 줄만 출력
            }
            strcpy(prev, curr);
            count = 1;
            first = 0;
        } else {
            // 이전 줄과 같으면 카운트 증가
            count++;
        }
    }
    // 마지막 줄 출력
    if (!first) {
        if (count_mode)
            printf("%7d %s", count, prev);
        else
            printf("%s", prev);
    }

    if (fp != stdin) fclose(fp);
    return 0;
}
/*
한 줄씩 읽으면서 이전 줄과 비교, 다르면 출력

-c 옵션이 있으면 중복 개수와 함께 출력, 없으면 줄만 출력

파일명을 지정하지 않으면 표준 입력에서 읽음
*/
