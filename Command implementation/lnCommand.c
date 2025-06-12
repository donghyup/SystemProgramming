#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // link, symlink

int main(int argc, char *argv[]) {
    int symbolic = 0;

    // 최소 인자 확인
    if (argc < 3) {
        fprintf(stderr, "사용법: %s [-s] <대상> <링크>\n", argv[0]);
        return 1;
    }

    int arg_offset = 0;
    if (strcmp(argv[1], "-s") == 0) {
        symbolic = 1;
        arg_offset = 1;
    }

    const char *target = argv[1 + arg_offset];
    const char *linkname = argv[2 + arg_offset];

    // 심볼릭 링크
    if (symbolic) {
        if (symlink(target, linkname) != 0) {
            perror("심볼릭 링크 생성 실패");
            return 1;
        }
        printf("심볼릭 링크 생성: %s → %s\n", linkname, target);
    }
    // 하드 링크
    else {
        if (link(target, linkname) != 0) {
            perror("하드 링크 생성 실패");
            return 1;
        }
        printf("하드 링크 생성: %s ↔ %s\n", linkname, target);
    }

    return 0;
}

