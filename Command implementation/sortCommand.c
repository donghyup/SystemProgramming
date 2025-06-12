#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 4096
#define MAX_LINE_LEN 1024

// �������� �� �Լ�
int cmp_asc(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

// ��������(����) �� �Լ�
int cmp_desc(const void *a, const void *b) {
    return strcmp(*(const char **)b, *(const char **)a);
}

int main(int argc, char *argv[]) {
    char *lines[MAX_LINES];
    int n = 0;
    int reverse = 0;
    char *filename = NULL;
    FILE *fp;

    // ����� ���� �Ľ�
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-r") == 0) {
            reverse = 1;
        } else {
            filename = argv[i];
        }
    }

    // ���� ����(������ ǥ�� �Է�)
    if (filename) {
        fp = fopen(filename, "r");
        if (!fp) {
            perror("������ �� �� �����ϴ�");
            return 1;
        }
    } else {
        fp = stdin;
    }

    // �� ������ �б�
    char buf[MAX_LINE_LEN];
    while (fgets(buf, sizeof(buf), fp)) {
        lines[n] = strdup(buf);
        n++;
        if (n >= MAX_LINES) break;
    }
    if (fp != stdin) fclose(fp);

    // ����
    qsort(lines, n, sizeof(char *), reverse ? cmp_desc : cmp_asc);

    // ���
    for (int i = 0; i < n; i++) {
        printf("%s", lines[i]);
        free(lines[i]);
    }
    return 0;
}
/*
���� �迭�� ������ ��, qsort�� ����(��������/��������)

-r �ɼ��� ������ ��������, ������ ��������

���ϸ��� �������� ������ ǥ�� �Է¿��� ����

�޸� �������� ó��
*/
