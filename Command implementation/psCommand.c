#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>

#define STAT_PATH_LEN 256
#define CMDLINE_LEN   256

// /proc/[pid]/stat ���Ͽ��� ���μ��� �̸� ����
void print_process_info(const char *pid) {
    char stat_path[STAT_PATH_LEN];
    snprintf(stat_path, sizeof(stat_path), "/proc/%s/stat", pid);

    FILE *fp = fopen(stat_path, "r");
    if (!fp) return;

    int pid_num;
    char comm[CMDLINE_LEN];
    char state;

    // stat ������ ù �� �ʵ�: pid (comm) state
    if (fscanf(fp, "%d (%[^)]) %c", &pid_num, comm, &state) == 3) {
        printf("%5d %-20s %c\n", pid_num, comm, state);
    }
    fclose(fp);
}

int main(int argc, char *argv[]) {
    DIR *proc = opendir("/proc");
    struct dirent *entry;

    if (!proc) {
        perror("/proc ���丮�� �� �� �����ϴ�");
        return 1;
    }

    printf(" PID  COMMAND              STATE\n");

    while ((entry = readdir(proc)) != NULL) {
        // ���丮 �̸��� ������ ��츸(PID)
        if (isdigit(entry->d_name[0])) {
            print_process_info(entry->d_name);
        }
    }

    closedir(proc);
    return 0;
}
/*
/proc ���丮 ������ �̸��� ������ ���丮��(PID) Ž��

�� /proc/[pid]/stat ���Ͽ��� PID, ���μ�����, ���� ����

ps -e�� ����ϰ� ��ü ���μ��� ����� ���
*/
