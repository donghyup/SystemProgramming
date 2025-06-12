#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

void human_readable(unsigned long long size, char *buf, size_t buflen) {
    const char *units[] = {"B", "K", "M", "G", "T", "P"};
    int i = 0;
    double s = (double)size;
    while (s >= 1024 && i < 5) {
        s /= 1024;
        i++;
    }
    snprintf(buf, buflen, "%.1f%s", s, units[i]);
}

// 경로의 전체 크기를 재귀적으로 계산
unsigned long long du(const char *path, int print) {
    struct stat st;
    unsigned long long total = 0;

    if (lstat(path, &st) == -1) return 0;

    if (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)) {
        total += st.st_size;
    } else if (S_ISDIR(st.st_mode)) {
        total += st.st_size; // 디렉토리 자체의 크기(보통 작음)
        DIR *dir = opendir(path);
        if (!dir) return total;
        struct dirent *entry;
        char child[4096];
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(child, sizeof(child), "%s/%s", path, entry->d_name);
            total += du(child, 0); // 하위 항목 합산(재귀)
        }
        closedir(dir);
    }
    if (print) {
        char buf[32];
        human_readable(total, buf, sizeof(buf));
        printf("%s\t%s\n", buf, path);
    }
    return total;
}

int main(int argc, char *argv[]) {
    const char *target = ".";
    if (argc > 1)
        target = argv[1];

    du(target, 1); // 최상위 디렉토리만 출력, 하위는 출력하지 않음(du -sh와 유사)
    return 0;
}
//컴파일 : gcc -o mydu mydu.c
//현재 디렉토리의 전체 용량(사람이 읽기 쉬운 단위) : ./mydu
/*
코드 설명 (주석 참고)
stat(혹은 lstat)로 파일/디렉토리 크기 조사

디렉토리는 재귀적으로 하위 항목까지 모두 합산

사람이 읽기 쉬운 단위(K, M, G 등)로 변환

기본적으로 du -sh와 유사하게, 지정한 경로의 총합만 출력
*/ 
