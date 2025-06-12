#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // unlink, rmdir, access
#include <sys/stat.h>   // stat, S_ISDIR
#include <dirent.h>     // opendir, readdir
#include <errno.h>

// �ɼ� ����
int opt_interactive = 0;
int opt_recursive = 0;
int opt_force = 0;

// ���� �� Ȯ��
int confirm(const char *path) {
    if (opt_force || !opt_interactive) return 1;
    char answer[4];
    printf("'%s'�� �����ұ��? (y/n): ", path);
    if (fgets(answer, sizeof(answer), stdin)) {
        return (answer[0] == 'y' || answer[0] == 'Y');
    }
    return 0;
}

// ��������� ���丮 ����
void delete_recursive(const char *path) {
    DIR *dir = opendir(path);
    if (!dir) {
        if (!opt_force)
            perror("���丮 ���� ����");
        return;
    }

    struct dirent *entry;
    char fullpath[1024];

    while ((entry = readdir(dir)) != NULL) {
        // . , .. �� �ǳʶ�
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

        struct stat st;
        if (stat(fullpath, &st) == -1) {
            if (!opt_force)
                perror("stat ����");
            continue;
        }

        if (S_ISDIR(st.st_mode)) {
            delete_recursive(fullpath); // ���� ���丮 ��� ȣ��
        } else {
            if (confirm(fullpath))
                unlink(fullpath); // ���� ����
        }
    }

    closedir(dir);

    if (confirm(path)) {
        if (rmdir(path) != 0 && !opt_force)
            perror("���丮 ���� ����");
    }
}

// �Ϲ� ����/���丮 ���� ó��
void delete_path(const char *path) {
    struct stat st;
    if (stat(path, &st) == -1) {
        if (!opt_force)
            perror("���� Ȯ�� ����");
        return;
    }

    if (S_ISDIR(st.st_mode)) {
        if (opt_recursive)
            delete_recursive(path);
        else if (!opt_force)
            fprintf(stderr, "'%s'�� ���丮�Դϴ�. -r �ɼ��� �ʿ��ؿ�!\n", path);
    } else {
        if (confirm(path)) {
            if (unlink(path) != 0 && !opt_force)
                perror("���� ���� ����");
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "����: %s [-i] [-r] [-f] <����...>\n", argv[0]);
        return 1;
    }

    // �ɼ� �Ľ�
    int i = 1;
    while (i < argc && argv[i][0] == '-') {
        for (int j = 1; argv[i][j]; j++) {
            switch (argv[i][j]) {
                case 'i': opt_interactive = 1; break;
                case 'r': opt_recursive = 1; break;
                case 'f': opt_force = 1; break;
                default:
                    fprintf(stderr, "�� �� ���� �ɼ�: -%c\n", argv[i][j]);
                    return 1;
            }
        }
        i++;
    }

    // ��� ����
    for (; i < argc; i++) {
        delete_path(argv[i]);
    }

    return 0;
}

