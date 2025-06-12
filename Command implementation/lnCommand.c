#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // link, symlink

int main(int argc, char *argv[]) {
    int symbolic = 0;

    // �ּ� ���� Ȯ��
    if (argc < 3) {
        fprintf(stderr, "����: %s [-s] <���> <��ũ>\n", argv[0]);
        return 1;
    }

    int arg_offset = 0;
    if (strcmp(argv[1], "-s") == 0) {
        symbolic = 1;
        arg_offset = 1;
    }

    const char *target = argv[1 + arg_offset];
    const char *linkname = argv[2 + arg_offset];

    // �ɺ��� ��ũ
    if (symbolic) {
        if (symlink(target, linkname) != 0) {
            perror("�ɺ��� ��ũ ���� ����");
            return 1;
        }
        printf("�ɺ��� ��ũ ����: %s �� %s\n", linkname, target);
    }
    // �ϵ� ��ũ
    else {
        if (link(target, linkname) != 0) {
            perror("�ϵ� ��ũ ���� ����");
            return 1;
        }
        printf("�ϵ� ��ũ ����: %s �� %s\n", linkname, target);
    }

    return 0;
}

