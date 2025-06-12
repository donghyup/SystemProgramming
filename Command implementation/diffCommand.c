#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 4096
#define MAX_LINE_LEN 1024

// ������ ���� ������ �о� �޸𸮿� ����
int read_lines(const char *filename, char lines[][MAX_LINE_LEN], int *count) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;
    int n = 0;
    while (fgets(lines[n], MAX_LINE_LEN, fp)) {
        n++;
        if (n >= MAX_LINES) break;
    }
    fclose(fp);
    *count = n;
    return 1;
}

// �⺻ diff: ���κ��� �ܼ� ��
void simple_diff(char a[][MAX_LINE_LEN], int n1, char b[][MAX_LINE_LEN], int n2) {
    int n = n1 > n2 ? n1 : n2;
    for (int i = 0; i < n; i++) {
        if (i >= n1) {
            printf("> %s", b[i]); // b���� �ִ� ��
        } else if (i >= n2) {
            printf("< %s", a[i]); // a���� �ִ� ��
        } else if (strcmp(a[i], b[i]) != 0) {
            printf("< %s", a[i]);
            printf("> %s", b[i]);
        }
    }
}

// ���� diff(unified diff): -u �ɼ�
void unified_diff(char a[][MAX_LINE_LEN], int n1, char b[][MAX_LINE_LEN], int n2) {
    printf("--- file1\n");
    printf("+++ file2\n");
    int i = 0, j = 0;
    while (i < n1 || j < n2) {
        if (i < n1 && j < n2 && strcmp(a[i], b[j]) == 0) {
            // ������ ��
            printf(" %s", a[i]);
            i++; j++;
        } else {
            // ���� �߻�: ����/�߰�/����
            if (i < n1) printf("-%s", a[i]);
            if (j < n2) printf("+%s", b[j]);
            i++; j++;
        }
    }
}

int main(int argc, char *argv[]) {
    int unified = 0;
    char *file1 = NULL, *file2 = NULL;

    // ����� ���� �Ľ�
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-u") == 0) unified = 1;
        else if (!file1) file1 = argv[i];
        else if (!file2) file2 = argv[i];
    }

    if (!file1 || !file2) {
        fprintf(stderr, "����: %s [-u] ����1 ����2\n", argv[0]);
        return 1;
    }

    char lines1[MAX_LINES][MAX_LINE_LEN];
    char lines2[MAX_LINES][MAX_LINE_LEN];
    int n1 = 0, n2 = 0;

    if (!read_lines(file1, lines1, &n1) || !read_lines(file2, lines2, &n2)) {
        fprintf(stderr, "������ ���� �� �����ϴ�.\n");
        return 1;
    }

    if (unified)
        unified_diff(lines1, n1, lines2, n2);
    else
        simple_diff(lines1, n1, lines2, n2);

    return 0;
}
/*
read_lines: ������ �� ������ �о� �迭�� ����

simple_diff: �⺻ diff ���, �� �� �� �� �ٸ��� <, >�� ǥ��

unified_diff: -u �ɼ� ��, ���� diff ����(---, +++, -, +, ����)���� ���

main: �ɼ�/���ϸ� �Ľ�, ������ �� �Լ� ȣ��
*/

