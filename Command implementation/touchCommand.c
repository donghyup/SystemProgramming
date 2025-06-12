#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>      // open
#include <unistd.h>     // close
#include <utime.h>      // utime
#include <sys/stat.h>   // stat

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "����: %s <�����̸�>\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        const char *filename = argv[i];
        struct stat st;

        // ������ �������� ������ ����
        if (stat(filename, &st) != 0) {
            int fd = open(filename, O_CREAT | O_WRONLY, 0644);
            if (fd == -1) {
                perror("���� ���� ����");
                continue;
            }
            close(fd);
            printf("���� ������: %s\n", filename);
        } else {
            // ������ �����ϸ� ���� �ð��� ������Ʈ
            if (utime(filename, NULL) != 0) {
                perror("�ð� ���� ���� ����");
                continue;
            }
            printf("���� �ð� ���ŵ�: %s\n", filename);
        }
    }

    return 0;
}

