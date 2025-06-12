#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // open
#include <unistd.h>     // close
#include <utime.h>      // utime
#include <sys/stat.h>   // stat

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "사용법: %s <파일이름>\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        const char *filename = argv[i];
        struct stat st;

        // 파일이 존재하지 않으면 생성
        if (stat(filename, &st) != 0) {
            int fd = open(filename, O_CREAT | O_WRONLY, 0644);
            if (fd == -1) {
                perror("파일 생성 실패");
                continue;
            }
            close(fd);
            printf("파일 생성됨: %s\n", filename);
        } else {
            // 파일이 존재하면 수정 시간만 업데이트
            if (utime(filename, NULL) != 0) {
                perror("시간 정보 갱신 실패");
                continue;
            }
            printf("수정 시간 갱신됨: %s\n", filename);
        }
    }

    return 0;
}

