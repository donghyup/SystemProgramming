#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#define STAT_PATH_LEN 256
#define CMDLINE_LEN   256

// /proc/[pid]/stat 파일에서 프로세스 이름 추출
void print_process_info(const char *pid) {
    char stat_path[STAT_PATH_LEN];
    snprintf(stat_path, sizeof(stat_path), "/proc/%s/stat", pid);

    FILE *fp = fopen(stat_path, "r");
    if (!fp) return;

    int pid_num;
    char comm[CMDLINE_LEN];
    char state;

    // stat 파일의 첫 세 필드: pid (comm) state
    if (fscanf(fp, "%d (%[^)]) %c", &pid_num, comm, &state) == 3) {
        printf("%5d %-20s %c\n", pid_num, comm, state);
    }
    fclose(fp);
}

int main(int argc, char *argv[]) {
    DIR *proc = opendir("/proc");
    struct dirent *entry;

    if (!proc) {
        perror("/proc 디렉토리를 열 수 없습니다");
        return 1;
    }

    printf(" PID  COMMAND              STATE\n");

    while ((entry = readdir(proc)) != NULL) {
        // 디렉토리 이름이 숫자인 경우만(PID)
        if (isdigit(entry->d_name[0])) {
            print_process_info(entry->d_name);
        }
    }

    closedir(proc);
    return 0;
}
/*
/proc 디렉토리 내에서 이름이 숫자인 디렉토리만(PID) 탐색

각 /proc/[pid]/stat 파일에서 PID, 프로세스명, 상태 추출

ps -e와 비슷하게 전체 프로세스 목록을 출력
*/
