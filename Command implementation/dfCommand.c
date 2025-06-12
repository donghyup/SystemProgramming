#include <stdio.h>
#include <stdlib.h>
#include <sys/statvfs.h>
#include <string.h>

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

int main(int argc, char *argv[]) {
    const char *path = "/";
    if (argc > 1)
        path = argv[1];

    struct statvfs stat;
    if (statvfs(path, &stat) != 0) {
        perror("statvfs ����");
        return 1;
    }

    unsigned long long total = (unsigned long long)stat.f_blocks * stat.f_frsize;
    unsigned long long avail = (unsigned long long)stat.f_bavail * stat.f_frsize;
    unsigned long long used = total - ((unsigned long long)stat.f_bfree * stat.f_frsize);

    char total_buf[32], used_buf[32], avail_buf[32];
    human_readable(total, total_buf, sizeof(total_buf));
    human_readable(used, used_buf, sizeof(used_buf));
    human_readable(avail, avail_buf, sizeof(avail_buf));

    printf("Filesystem      Size  Used Avail Use%% Mounted on\n");
    printf("%-15s %5s %5s %5s %3d%% %s\n",
        path,
        total_buf,
        used_buf,
        avail_buf,
        total ? (int)(used * 100 / total) : 0,
        path
    );
    return 0;
}
//��Ʈ ���Ͻý��� ��뷮(�⺻) : bash./mydf, 
//Ư�� ����Ʈ����Ʈ(/home ��) : ./mydf /home

/*
�ڵ� ���� (�ּ� ����)
statvfs�� ���Ͻý��� ��� ���� ��ȸ

�ѿ뷮, ��뷮, ���뷮�� ����Ʈ ������ ���

����� �б� ���� ����(K, M, G, T ��)�� ��ȯ

df -h�� ������ ǥ �������� ���
*/
