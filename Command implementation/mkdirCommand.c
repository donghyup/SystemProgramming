#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>   // mkdir
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>     // access
#include <libgen.h>     // dirname

// 디렉토리 존재 여부 확인
int dir_exists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

// -p 옵션 구현: 상위 디렉토리까지 생성
int mkdir_p(const char *path, mode_t mode) {
    char tmp[1024];
    snprintf(tmp, sizeof(tmp), "%s", path);

    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (!dir_exists(tmp)) {
                if (mkdir(tmp, mode) != 0 && errno != EEXIST) {
                    perror("mkdir -p 실패");
                    return -1;
                }
            }
            *p = '/';
        }
    }

    // 마지막 디렉토리 생성
    if (!dir_exists(tmp)) {
        if (mkdir(tmp, mode) != 0 && errno != EEXIST) {
            perror("mkdir -p 실패");
            return -1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int opt_p = 0;
    int is_mkdir = 0;
    int is_rmdir = 0;

    if (argc < 3) {
        fprintf(stderr, "사용법:\n");
        fprintf(stderr, "  %s mkdir [-p] <디렉토리>\n", argv[0]);
        fprintf(stderr, "  %s rmdir <디렉토리>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "mkdir") == 0) {
        is_mkdir = 1;
        if (strcmp(argv[2], "-p") == 0) {
            opt_p = 1;
        }
    } else if (strcmp(argv[1], "rmdir") == 0) {
        is_rmdir = 1;
    } else {
        fprintf(stderr, "지원되지 않는 명령: %s\n", argv[1]);
        return 1;
    }

    if (is_mkdir) {
        const char *dir_path = opt_p ? argv[3] : argv[2];
        if (!dir_path) {
            fprintf(stderr, "디렉토리 경로가 누락되었습니다!\n");
            return 1;
        }

        if (opt_p) {
            if (mkdir_p(dir_path, 0755) == 0)
                printf("생성 완료: %s\n", dir_path);
        } else {
            if (mkdir(dir_path, 0755) == 0)
                printf("생성 완료: %s\n", dir_path);
            else
                perror("mkdir 실패");
        }
    }

    if (is_rmdir) {
        const char *dir_path = argv[2];
        if (rmdir(dir_path) == 0)
            printf("삭제 완료: %s\n", dir_path);
        else
            perror("rmdir 실패");
    }

    return 0;
}

