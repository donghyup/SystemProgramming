#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 4096
#define MAX_FIELDS 128

// 필드 분리 함수
int split(char *line, char *fields[], const char *delim) {
    int count = 0;
    char *token = strtok(line, delim);
    while (token && count < MAX_FIELDS) {
        fields[count++] = token;
        token = strtok(NULL, delim);
    }
    return count;
}

int main(int argc, char *argv[]) {
    char delim[16] = " \t\n"; // 기본 구분자: 공백, 탭, 개행
    char *script = NULL;
    char *filename = NULL;
    FILE *fp;

    // 명령줄 인자 파싱
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-F") == 0 && i + 1 < argc) {
            strncpy(delim, argv[++i], sizeof(delim)-1);
        } else if (!script) {
            script = argv[i]; // 간단한 스크립트(예: $2)
        } else {
            filename = argv[i];
        }
    }

    if (!script) {
        fprintf(stderr, "사용법: %s [-F 구분자] '스크립트' [파일]\n", argv[0]);
        return 1;
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

    char line[MAX_LINE];
    char *fields[MAX_FIELDS];
    int nf;

    while (fgets(line, sizeof(line), fp)) {
        // 줄 끝 개행 제거
        size_t len = strlen(line);
        if (len && line[len-1] == '\n') line[len-1] = '\0';

        // 필드 분리
        nf = split(line, fields, delim);

        // 간단한 스크립트: $N 형식만 지원
        if (script[0] == '$') {
            int idx = atoi(script + 1);
            if (idx >= 1 && idx <= nf)
                printf("%s\n", fields[idx-1]);
        } else if (strcmp(script, "{print}") == 0) {
            // 모든 줄 출력
            for (int i = 0; i < nf; i++)
                printf("%s%c", fields[i], (i == nf-1 ? '\n' : ' '));
        }
        // 더 복잡한 스크립트는 필요에 따라 확장 가능
    }

    if (fp != stdin) fclose(fp);
    return 0;
}
/*
split 함수: 입력 줄을 구분자로 분리해 필드 배열에 저장

-F 옵션: 구분자 지정(기본은 공백/탭/개행)

스크립트: $N(N번째 필드 출력), {print}(모든 필드 출력) 지원

파일명 없으면 표준 입력 사용
*/
