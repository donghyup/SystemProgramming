#include <stdio.h>
#include <dirent.h>     // opendir, readdir
#include <sys/stat.h>   // stat 구조체, 파일 상태 정보
#include <string.h>     // strcmp, snprintf
#include <stdlib.h>     // exit
#include <unistd.h>     // getcwd

//파일 권한을 문자로 출력하는 함수
void print_permissions(mode_t mode) {
    char perms[11] = "----------";

    if (S_ISDIR(mode)) perms[0] = 'd'; // 디렉토리면 첫 글자를 d로

    // 사용자 권한
    if (mode & S_IRUSR) perms[1] = 'r';
    if (mode & S_IWUSR) perms[2] = 'w';
    if (mode & S_IXUSR) perms[3] = 'x';

    // 그룹 권한
    if (mode & S_IRGRP) perms[4] = 'r';
    if (mode & S_IWGRP) perms[5] = 'w';
    if (mode & S_IXGRP) perms[6] = 'x';

    // 다른 사용자 권한
    if (mode & S_IROTH) perms[7] = 'r';
    if (mode & S_IWOTH) perms[8] = 'w';
    if (mode & S_IXOTH) perms[9] = 'x';

    printf("%s ", perms);
}

//ls 기능 구현 함수
void ls(const char *path, int show_all, int long_format, int color) {
    DIR *dir = opendir(path); // 디렉토리 열기
    if (!dir) {
        perror("opendir 실패");
        return;
    }

    struct dirent *entry;

    // 디렉토리 내 항목을 하나씩 읽어옴
    while ((entry = readdir(dir)) != NULL) {
        // 숨김파일 무시 ('.'로 시작하는 파일은 패스)
        if (!show_all && entry->d_name[0] == '.') continue;

        // -l 옵션이 있는 경우 상세정보 출력
        if (long_format) {
            struct stat st;
            char fullpath[1024];
            snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

            // 파일 상태 정보 가져오기
            if (stat(fullpath, &st) == 0) {
                print_permissions(st.st_mode);        // 권한 출력
                printf("%5ld ", st.st_size);          // 크기 출력
            }
        }

        // --color 옵션: 디렉토리는 파란색으로 표시
        if (color && entry->d_type == DT_DIR) {
            printf("\033[1;34m%s\033[0m", entry->d_name);  // 파란색 출력
        } else {
            printf("%s", entry->d_name); // 일반 출력
        }

        // -l 옵션이면 줄 바꿈, 아니면 한 줄에 출력
        if (long_format) printf("\n");
        else printf("  ");
    }

    closedir(dir);  // 디렉토리 닫기
    printf("\n");   // 마지막 줄 개행
}

int main(int argc, char *argv[]) {
    int show_all = 0;     // -a 옵션
    int long_format = 0;  // -l 옵션
    int color = 0;        // --color 옵션

    //옵션 파싱 시작
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) show_all = 1;
        else if (strcmp(argv[i], "-l") == 0) long_format = 1;
        else if (strcmp(argv[i], "--color") == 0) color = 1;
        else {
            fprintf(stderr, "알 수 없는 옵션: %s\n", argv[i]);
            exit(1);
        }
    }

    // 현재 디렉토리 기준으로 실행
    ls(".", show_all, long_format, color);
    return 0;
}

