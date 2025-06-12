#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // access
#include <errno.h>
#include <libgen.h>     // basename
#include <sys/stat.h>   // stat

// ��� ������ �����ϸ� true
int file_exists(const char *path) {
    return access(path, F_OK) == 0;
}

// ����ڿ��� ����� ��� (y/n)
int confirm_overwrite(const char *path) {
    char answer[4];
    printf("'%s' ������ �����մϴ�. ������? (y/n): ", path);
    if (fgets(answer, sizeof(answer), stdin)) {
        return (answer[0] == 'y' || answer[0] == 'Y');
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int interactive = 0;

    // �ɼ� �Ľ�
    int src_index = 1;
    if (argc > 1 && strcmp(argv[1], "-i") == 0) {
        interactive = 1;
        src_index++;
    }

    if (argc - src_index != 2) {
        fprintf(stderr, "����: %s [-i] <����> <���>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *src = argv[src_index];
    const char *dst = argv[src_index + 1];

    // ����� Ȯ��
    if (interactive && file_exists(dst)) {
        if (!confirm_overwrite(dst)) {
            printf("�̵��� ����߽��ϴ�.\n");
            exit(0);
        }
    }

    // ���� �̵� (rename)
    if (rename(src, dst) != 0) {
        perror("rename ����");
        exit(EXIT_FAILURE);
    }

    printf("'%s' ? '%s' �̵� �Ϸ�!\n", src, dst);
    return 0;
}

