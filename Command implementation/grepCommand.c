#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_LINE 4096

// 문자열 내 부분 문자열(needle)이 포함되어 있는지(대소문자 구분)
int strcasestr_simple(const char *haystack, const char *needle) {
    // 소문자로 변환하여 strstr 사용
    char *h = strdup(haystack);
    char *n = strdup(needle);
    for (char *p = h; *p; ++p) *p = tolower(*p);
    for (char *p = n; *p; ++p) *p = tolower(*p);
    int found = strstr(h, n) != NULL;
    free(h); free(n);
    return found;
}

// 한 파일에서 문자열 검색
void grep_file(const char *filename, const char *pattern, int ignore_case, int invert_match) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return;

    char line[MAX_LINE];
    int matched;

    while (fgets(line, sizeof(line), fp)) {
        if (ignore_case)
            matched = strcasestr_simple(line, pattern);
        else
            matched = strstr(line, pattern) != NULL;

        // -v 옵션: 매칭 안 되는 줄 출력
        if (invert_match) matched = !matched;

        if (matched) {
            printf("%s:%s", filename, line);
        }
    }
    fclose(fp);
}

// 디렉토리 순회(재귀, -r 옵션)
void grep_dir(const char *dirname, const char *pattern, int ignore_case, int invert_match) {
    DIR *dir = opendir(dirname);
    if (!dir) return;

    struct dirent *entry;
    char path[4096];

    while ((entry = readdir(dir)) != NULL) {
        // . 과 .. 무시
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);

        struct stat st;
        stat(path, &st);
        if (S_ISDIR(st.st_mode)) {
            // 디렉토리면 재귀 호출
            grep_dir(path, pattern, ignore_case, invert_match);
        } else if (S_ISREG(st.st_mode)) {
            // 일반 파일이면 검색
            grep_file(path, pattern, ignore_case, invert_match);
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {
    int ignore_case = 0;   // -i 옵션
    int invert_match = 0;  // -v 옵션
    int recursive = 0;     // -r 옵션
    char *pattern = NULL;
    char *target = NULL;

    // 명령줄 인자 파싱
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) ignore_case = 1;
        else if (strcmp(argv[i], "-v") == 0) invert_match = 1;
        else if (strcmp(argv[i], "-r") == 0) recursive = 1;
        else if (!pattern) pattern = argv[i];
        else if (!target) target = argv[i];
    }

    if (!pattern || !target) {
        fprintf(stderr, "사용법: %s [-i] [-v] [-r] 패턴 파일|디렉토리\n", argv[0]);
        return 1;
    }

    struct stat st;
    stat(target, &st);
    if (recursive && S_ISDIR(st.st_mode)) {
        grep_dir(target, pattern, ignore_case, invert_match); // -r 디렉토리 재귀 검색
    } else {
        grep_file(target, pattern, ignore_case, invert_match); // 일반 파일 검색
    }
    return 0;
}

