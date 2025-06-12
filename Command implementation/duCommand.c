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

// ����� ��ü ũ�⸦ ��������� ���
unsigned long long du(const char *path, int print) {
    struct stat st;
    unsigned long long total = 0;

    if (lstat(path, &st) == -1) return 0;

    if (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)) {
        total += st.st_size;
    } else if (S_ISDIR(st.st_mode)) {
        total += st.st_size; // ���丮 ��ü�� ũ��(���� ����)
        DIR *dir = opendir(path);
        if (!dir) return total;
        struct dirent *entry;
        char child[4096];
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(child, sizeof(child), "%s/%s", path, entry->d_name);
            total += du(child, 0); // ���� �׸� �ջ�(���)
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

    du(target, 1); // �ֻ��� ���丮�� ���, ������ ������� ����(du -sh�� ����)
    return 0;
}
//������ : gcc -o mydu mydu.c
//���� ���丮�� ��ü �뷮(����� �б� ���� ����) : ./mydu
/*
�ڵ� ���� (�ּ� ����)
stat(Ȥ�� lstat)�� ����/���丮 ũ�� ����

���丮�� ��������� ���� �׸���� ��� �ջ�

����� �б� ���� ����(K, M, G ��)�� ��ȯ

�⺻������ du -sh�� �����ϰ�, ������ ����� ���ո� ���
*/ 
