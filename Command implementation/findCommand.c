#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fnmatch.h>

void find(const char *dir, const char *name_pattern, int type_file) {
    DIR *dp = opendir(dir);
    if (!dp) return;

    struct dirent *entry;
    char path[4096];

    while ((entry = readdir(dp)) != NULL) {
        // . 과 .. 무시
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name);

        struct stat st;
        if (stat(path, &st) == -1) continue;

        // -type f: 파일만 출력
        if (type_file && !S_ISREG(st.st_mode)) {
            // 파일만 찾는 옵션이면서 파일이 아니면 건너뜀
        } else {
            // -name: 이름 패턴이 있으면 fnmatch로 비교
            if (name_pattern) {
                if (fnmatch(name_pattern, entry->d_name, 0) == 0)
                    printf("%s\n", path);
            } else {
                printf("%s\n", path);
            }
        }

        // 디렉토리면 재귀 호출
        if (S_ISDIR(st.st_mode)) {
            find(path, name_pattern, type_file);
        }
    }
    closedir(dp);
}

int main(int argc, char *argv[]) {
    char *start_dir = ".";
    char *name_pattern = NULL;
    int type_file = 0;

    // 명령줄 인자 파싱
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-name") == 0 && i + 1 < argc) {
            name_pattern = argv[++i];
        } else if (strcmp(argv[i], "-type") == 0 && i + 1 < argc) {
            if (strcmp(argv[i + 1], "f") == 0)
                type_file = 1;
            i++;
        } else {
            start_dir = argv[i];
        }
    }

    find(start_dir, name_pattern, type_file);
    return 0;
}

