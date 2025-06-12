#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include <unistd.h>
#include <ncurses.h>
#include <time.h>

#define STAT_PATH_LEN 256
#define CMDLINE_LEN   256
#define MAX_PROCS     1024

typedef struct {
    int pid;
    char comm[CMDLINE_LEN];
    char state;
    unsigned long vsize; // ���� �޸�
    unsigned long utime, stime; // CPU �ð�
} procinfo;

int read_procs(procinfo procs[], int max) {
    DIR *proc = opendir("/proc");
    struct dirent *entry;
    int count = 0;
    if (!proc) return 0;

    while ((entry = readdir(proc)) != NULL && count < max) {
        if (!isdigit(entry->d_name[0])) continue;

        char stat_path[STAT_PATH_LEN];
        snprintf(stat_path, sizeof(stat_path), "/proc/%s/stat", entry->d_name);
        FILE *fp = fopen(stat_path, "r");
        if (!fp) continue;

        procinfo *p = &procs[count];
        // stat ������ �ֿ� �ʵ�: pid (comm) state ... utime stime ... vsize ...
        fscanf(fp, "%d (%[^)]) %c", &p->pid, p->comm, &p->state);

        // 14��°, 15��°, 23��° �ʵ� �ʿ�
        unsigned long skip;
        for (int i = 0; i < 10; i++) fscanf(fp, "%lu", &skip); // pid(1), comm(2), state(3), skip 4~13
        fscanf(fp, "%lu %lu", &p->utime, &p->stime);           // 14, 15
        for (int i = 0; i < 6; i++) fscanf(fp, "%lu", &skip);  // skip 16~21
        fscanf(fp, "%lu", &p->vsize);                          // 23

        fclose(fp);
        count++;
    }
    closedir(proc);
    return count;
}

void print_procs(procinfo procs[], int n) {
    mvprintw(0, 0, "  PID %-20s %4s %10s %10s", "COMMAND", "S", "VSZ", "UTIME");
    for (int i = 0; i < n && i < LINES-2; i++) {
        mvprintw(i+1, 0, "%5d %-20s %4c %10lu %10lu",
            procs[i].pid, procs[i].comm, procs[i].state, procs[i].vsize/1024, procs[i].utime);
    }
    clrtoeol();
    refresh();
}

int main() {
    procinfo procs[MAX_PROCS];
    int n;

    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE); // getch()�� ����ŷ����
    curs_set(0);

    int ch = 0;
    while (ch != 'q') {
        erase();
        n = read_procs(procs, MAX_PROCS);
        print_procs(procs, n);

        mvprintw(LINES-1, 0, "Press 'q' to quit. Procs: %d", n);
        refresh();

        // 1�� ���, �� ���� �Է� ������ ����
        for (int i = 0; i < 10; i++) {
            ch = getch();
            if (ch == 'q') break;
            usleep(100000); // 0.1��
        }
    }
    endwin();
    return 0;
}
// ������ : gcc -o mytop mytop.c -lncurses
//-lncurses �ɼ��� C ���α׷��� �������� �� ncurses ���̺귯���� ��ũ(link)�϶�� �ǹ�
/*
/proc���� PID�� stat ������ �о� �ֿ� ������ ����

curses(ncurses)�� ȭ���� �ֱ������� ����� �ٽ� �׸�

getch()�� ����ŷ���� �����Ͽ� Ű �Է�(q) ����

1�ʸ��� ȭ�� ����
*/ 

