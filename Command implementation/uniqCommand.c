#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024

int main(int argc, char *argv[]) {
    int count_mode = 0;      // -c �ɼ� ����
    char *filename = NULL;
    FILE *fp;

    // ����� ���� �Ľ�
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-c") == 0) {
            count_mode = 1;
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

    char prev[MAX_LINE_LEN] = "";
    char curr[MAX_LINE_LEN];
    int count = 0;
    int first = 1;

    while (fgets(curr, sizeof(curr), fp)) {
        if (first || strcmp(curr, prev) != 0) {
            // ���� �ٰ� �ٸ���(Ȥ�� ù ��)
            if (!first) {
                if (count_mode)
                    printf("%7d %s", count, prev); // -c �ɼ�: ������ �Բ� ���
                else
                    printf("%s", prev);            // �⺻: �ٸ� ���
            }
            strcpy(prev, curr);
            count = 1;
            first = 0;
        } else {
            // ���� �ٰ� ������ ī��Ʈ ����
            count++;
        }
    }
    // ������ �� ���
    if (!first) {
        if (count_mode)
            printf("%7d %s", count, prev);
        else
            printf("%s", prev);
    }

    if (fp != stdin) fclose(fp);
    return 0;
}
/*
�� �پ� �����鼭 ���� �ٰ� ��, �ٸ��� ���

-c �ɼ��� ������ �ߺ� ������ �Բ� ���, ������ �ٸ� ���

���ϸ��� �������� ������ ǥ�� �Է¿��� ����
*/
