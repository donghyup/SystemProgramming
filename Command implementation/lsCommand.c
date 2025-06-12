#include <stdio.h>
#include <dirent.h>     // opendir, readdir
#include <sys/stat.h>   // stat ����ü, ���� ���� ����
#include <string.h>     // strcmp, snprintf
#include <stdlib.h>     // exit
#include <unistd.h>     // getcwd

//���� ������ ���ڷ� ����ϴ� �Լ�
void print_permissions(mode_t mode) {
    char perms[11] = "----------";

    if (S_ISDIR(mode)) perms[0] = 'd'; // ���丮�� ù ���ڸ� d��

    // ����� ����
    if (mode & S_IRUSR) perms[1] = 'r';
    if (mode & S_IWUSR) perms[2] = 'w';
    if (mode & S_IXUSR) perms[3] = 'x';

    // �׷� ����
    if (mode & S_IRGRP) perms[4] = 'r';
    if (mode & S_IWGRP) perms[5] = 'w';
    if (mode & S_IXGRP) perms[6] = 'x';

    // �ٸ� ����� ����
    if (mode & S_IROTH) perms[7] = 'r';
    if (mode & S_IWOTH) perms[8] = 'w';
    if (mode & S_IXOTH) perms[9] = 'x';

    printf("%s ", perms);
}

//ls ��� ���� �Լ�
void ls(const char *path, int show_all, int long_format, int color) {
    DIR *dir = opendir(path); // ���丮 ����
    if (!dir) {
        perror("opendir ����");
        return;
    }

    struct dirent *entry;

    // ���丮 �� �׸��� �ϳ��� �о��
    while ((entry = readdir(dir)) != NULL) {
        // �������� ���� ('.'�� �����ϴ� ������ �н�)
        if (!show_all && entry->d_name[0] == '.') continue;

        // -l �ɼ��� �ִ� ��� ������ ���
        if (long_format) {
            struct stat st;
            char fullpath[1024];
            snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

            // ���� ���� ���� ��������
            if (stat(fullpath, &st) == 0) {
                print_permissions(st.st_mode);        // ���� ���
                printf("%5ld ", st.st_size);          // ũ�� ���
            }
        }

        // --color �ɼ�: ���丮�� �Ķ������� ǥ��
        if (color && entry->d_type == DT_DIR) {
            printf("\033[1;34m%s\033[0m", entry->d_name);  // �Ķ��� ���
        } else {
            printf("%s", entry->d_name); // �Ϲ� ���
        }

        // -l �ɼ��̸� �� �ٲ�, �ƴϸ� �� �ٿ� ���
        if (long_format) printf("\n");
        else printf("  ");
    }

    closedir(dir);  // ���丮 �ݱ�
    printf("\n");   // ������ �� ����
}

int main(int argc, char *argv[]) {
    int show_all = 0;     // -a �ɼ�
    int long_format = 0;  // -l �ɼ�
    int color = 0;        // --color �ɼ�

    //�ɼ� �Ľ� ����
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) show_all = 1;
        else if (strcmp(argv[i], "-l") == 0) long_format = 1;
        else if (strcmp(argv[i], "--color") == 0) color = 1;
        else {
            fprintf(stderr, "�� �� ���� �ɼ�: %s\n", argv[i]);
            exit(1);
        }
    }

    // ���� ���丮 �������� ����
    ls(".", show_all, long_format, color);
    return 0;
}

