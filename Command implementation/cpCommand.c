#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // access, read, write
#include <fcntl.h>      // open
#include <sys/stat.h>   // stat
#include <dirent.h>     // DIR, opendir, readdir
#include <errno.h>

#define BUF_SIZE 4096

// ���� ���� �Լ�
void copy_file(const char *src, const char *dest, int interactive) {
    // ����� �� Ȯ�� (-i �ɼ�)
    if (interactive && access(dest, F_OK) == 0) {
        char ans;
        printf("cp: '%s'�� ����ðڽ��ϱ�? (y/n): ", dest);
        scanf(" %c", &ans);
        if (ans != 'y' && ans != 'Y') return;
    }

    int in = open(src, O_RDONLY);
    if (in < 0) {
        perror("���� ���� ����(�б�)");
        return;
    }

    int out = open(dest, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (out < 0) {
        perror("���� ���� ����(����)");
        close(in);
        return;
    }

    char buf[BUF_SIZE];
    ssize_t n;
    while ((n = read(in, buf, BUF_SIZE)) > 0) {
        if (write(out, buf, n) != n) {
            perror("���� ����");
            break;
        }
    }

    close(in);
    close(out);
}

// ���丮 ���� �Լ� (���������)
void copy_directory(const char *src, const char *dest, int interactive) {
    DIR *dir = opendir(src);
    if (!dir) {
        perror("���丮 ���� ����");
        return;
    }

    mkdir(dest, 0755);

    struct dirent *entry;
    char src_path[1024], dest_path[1024];

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;

        snprintf(src_path, sizeof(src_path), "%s/%s", src, entry->d_name);
        snprintf(dest_path, sizeof(dest_path), "%s/%s", dest, entry->d_name);

        struct stat st;
        if (stat(src_path, &st) == -1) continue;

        if (S_ISDIR(st.st_mode)) {
            // ���丮�� ��������� ����
            copy_directory(src_path, dest_path, interactive);
        } else {
            copy_file(src_path, dest_path, interactive);
        }
    }

    closedir(dir);
}

// ���� �Լ�
int main(int argc, char *argv[]) {
    int interactive = 0;
    int recursive = 0;
    int optind = 1;

    // �ɼ� �Ľ�
    while (optind < argc && argv[optind][0] == '-') {
        if (strcmp(argv[optind], "-i") == 0) interactive = 1;
        else if (strcmp(argv[optind], "-r") == 0 || strcmp(argv[optind], "-R") == 0) recursive = 1;
        else {
            fprintf(stderr, "�������� �ʴ� �ɼ�: %s\n", argv[optind]);
            exit(1);
        }
        optind++;
    }

    if (argc - optind < 2) {
        fprintf(stderr, "����: %s [-i] [-r] ���� ���\n", argv[0]);
        exit(1);
    }

    char *src = argv[optind];
    char *dest = argv[optind + 1];

    struct stat st;
    if (stat(src, &st) == -1) {
        perror("���� ���� ���� �������� ����");
        exit(1);
    }

    if (S_ISDIR(st.st_mode)) {
        if (!recursive) {
            fprintf(stderr, "cp: '%s'�� ���丮�Դϴ�. -r �ɼ��� �ʿ��ؿ�!\n", src);
            exit(1);
        }
        copy_directory(src, dest, interactive);
    } else {
        copy_file(src, dest, interactive);
    }

    return 0;
}

