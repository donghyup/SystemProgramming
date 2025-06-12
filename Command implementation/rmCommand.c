#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // unlink, rmdir, access
#include <sys/stat.h>   // stat, S_ISDIR
#include <dirent.h>     // opendir, readdir
#include <errno.h>

// 옵션 변수
int opt_interactive = 0;
int opt_recursive = 0;
int opt_force = 0;

// 삭제 전 확인
int confirm(const char *path) {
    if (opt_force || !opt_interactive) return 1;
    char answer[4];
    printf("'%s'를 삭제할까요? (y/n): ", path);
    if (fgets(answer, sizeof(answer), stdin)) {
        return (answer[0] == 'y' || answer[0] == 'Y');
    }
    return 0;
}

// 재귀적으로 디렉토리 삭제
void delete_recursive(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        if (!opt_force)
            perror("디렉토리 열기 실패");
        return;
    }

    struct dirent *entry;
    char fullpath[1024];

    while ((entry = readdir(dir)) != NULL) {
        // . , .. 는 건너뜀
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(fullpath, &st) == -1) {
            if (!opt_force)
                perror("stat 실패");
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            delete_recursive(fullpath); // 서브 디렉토리 재귀 호출
        } else {
            if (confirm(fullpath))
                unlink(fullpath); // 파일 삭제
        }
    }

    closedir(dir);

    if (confirm(path)) {
        if (rmdir(path) != 0 && !opt_force)
            perror("디렉토리 삭제 실패");
    }
}

// 일반 파일/디렉토리 삭제 처리
void delete_path(const char *path) {
    struct stat st;
    if (stat(path, &st) == -1) {
        if (!opt_force)
            perror("파일 확인 실패");
        return;
    }

    if (S_ISDIR(st.st_mode)) {
        if (opt_recursive)
            delete_recursive(path);
        else if (!opt_force)
            fprintf(stderr, "'%s'는 디렉토리입니다. -r 옵션이 필요해요!\n", path);
    } else {
        if (confirm(path)) {
            if (unlink(path) != 0 && !opt_force)
                perror("파일 삭제 실패");
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "사용법: %s [-i] [-r] [-f] <파일...>\n", argv[0]);
        return 1;
    }

    // 옵션 파싱
    int i = 1;
    while (i < argc && argv[i][0] == '-') {
        for (int j = 1; argv[i][j]; j++) {
            switch (argv[i][j]) {
                case 'i': opt_interactive = 1; break;
                case 'r': opt_recursive = 1; break;
                case 'f': opt_force = 1; break;
                default:
                    fprintf(stderr, "알 수 없는 옵션: -%c\n", argv[i][j]);
                    return 1;
            }
        }
        i++;
    }

    // 경로 삭제
    for (; i < argc; i++) {
        delete_path(argv[i]);
    }

    return 0;
}

