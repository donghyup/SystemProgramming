#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // access
#include <errno.h>
#include <libgen.h>     // basename
#include <sys/stat.h>   // stat

// 대상 파일이 존재하면 true
int file_exists(const char *path) {
    return access(path, F_OK) == 0;
}

// 사용자에게 덮어쓸지 물어봄 (y/n)
int confirm_overwrite(const char *path) {
    char answer[4];
    printf("'%s' 파일이 존재합니다. 덮어쓸까요? (y/n): ", path);
    if (fgets(answer, sizeof(answer), stdin)) {
        return (answer[0] == 'y' || answer[0] == 'Y');
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int interactive = 0;

    // 옵션 파싱
    int src_index = 1;
    if (argc > 1 && strcmp(argv[1], "-i") == 0) {
        interactive = 1;
        src_index++;
    }

    if (argc - src_index != 2) {
        fprintf(stderr, "사용법: %s [-i] <원본> <대상>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *src = argv[src_index];
    const char *dst = argv[src_index + 1];

    // 덮어쓰기 확인
    if (interactive && file_exists(dst)) {
        if (!confirm_overwrite(dst)) {
            printf("이동을 취소했습니다.\n");
            exit(0);
        }
    }

    // 실제 이동 (rename)
    if (rename(src, dst) != 0) {
        perror("rename 실패");
        exit(EXIT_FAILURE);
    }

    printf("'%s' ? '%s' 이동 완료!\n", src, dst);
    return 0;
}

