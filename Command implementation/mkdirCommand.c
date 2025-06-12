#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>   // mkdir
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>     // access
#include <libgen.h>     // dirname

// ���丮 ���� ���� Ȯ��
int dir_exists(const char *path) {
    struct stat st;
    return (stat(path, &st) == 0 && S_ISDIR(st.st_mode));
}

// -p �ɼ� ����: ���� ���丮���� ����
int mkdir_p(const char *path, mode_t mode) {
    char tmp[1024];
    snprintf(tmp, sizeof(tmp), "%s", path);

    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            if (!dir_exists(tmp)) {
                if (mkdir(tmp, mode) != 0 && errno != EEXIST) {
                    perror("mkdir -p ����");
                    return -1;
                }
            }
            *p = '/';
        }
    }

    // ������ ���丮 ����
    if (!dir_exists(tmp)) {
        if (mkdir(tmp, mode) != 0 && errno != EEXIST) {
            perror("mkdir -p ����");
            return -1;
        }
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int opt_p = 0;
    int is_mkdir = 0;
    int is_rmdir = 0;

    if (argc < 3) {
        fprintf(stderr, "����:\n");
        fprintf(stderr, "  %s mkdir [-p] <���丮>\n", argv[0]);
        fprintf(stderr, "  %s rmdir <���丮>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "mkdir") == 0) {
        is_mkdir = 1;
        if (strcmp(argv[2], "-p") == 0) {
            opt_p = 1;
        }
    } else if (strcmp(argv[1], "rmdir") == 0) {
        is_rmdir = 1;
    } else {
        fprintf(stderr, "�������� �ʴ� ���: %s\n", argv[1]);
        return 1;
    }

    if (is_mkdir) {
        const char *dir_path = opt_p ? argv[3] : argv[2];
        if (!dir_path) {
            fprintf(stderr, "���丮 ��ΰ� �����Ǿ����ϴ�!\n");
            return 1;
        }

        if (opt_p) {
            if (mkdir_p(dir_path, 0755) == 0)
                printf("���� �Ϸ�: %s\n", dir_path);
        } else {
            if (mkdir(dir_path, 0755) == 0)
                printf("���� �Ϸ�: %s\n", dir_path);
            else
                perror("mkdir ����");
        }
    }

    if (is_rmdir) {
        const char *dir_path = argv[2];
        if (rmdir(dir_path) == 0)
            printf("���� �Ϸ�: %s\n", dir_path);
        else
            perror("rmdir ����");
    }

    return 0;
}

