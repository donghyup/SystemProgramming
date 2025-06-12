#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_LINE_COUNT 10    // 기본 출력 줄 수

int main(int argc, char *argv[]) {
    int n = DEFAULT_LINE_COUNT;  // 출력할 줄 수, 기본값 10
    char *filename = NULL;       // 파일명 저장 변수
    FILE *fp;
    char buffer[1024];           // 한 줄씩 읽을 버퍼
    int line_count = 0;          // 출력한 줄 수 카운트

    // 명령줄 인자 파싱
    for (int i = 1; i < argc; i++) {
        // -n 옵션 처리
        if (argv[i][0] == '-' && argv[i][1] == 'n') {
            if (i + 1 < argc) {
                n = atoi(argv[i + 1]); // 옵션값을 정수로 변환
                i++;                   // 옵션값 인덱스 건너뜀
            }
        } else {
            filename = argv[i];        // 파일명 저장
        }
    }

    // 파일 열기, 파일명이 없으면 표준 입력 사용
    if (filename) {
        fp = fopen(filename, "r");
        if (!fp) {
            perror("파일을 열 수 없습니다");
            return 1;
        }
    } else {
        fp = stdin;
    }

    // 파일에서 한 줄씩 읽어서 출력, 지정한 줄 수만큼만
    while (fgets(buffer, sizeof(buffer), fp)) {
        printf("%s", buffer);
        line_count++;
        if (line_count >= n) break; // 지정한 줄 수만큼 출력하면 종료
    }

    if (fp != stdin) fclose(fp); // 파일 닫기
    return 0;
}


