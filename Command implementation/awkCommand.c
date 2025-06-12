#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 4096
#define MAX_FIELDS 128

// �ʵ� �и� �Լ�
int split(char *line, char *fields[], const char *delim) {
    int count = 0;
    char *token = strtok(line, delim);
    while (token && count < MAX_FIELDS) {
        fields[count++] = token;
        token = strtok(NULL, delim);
    }
    return count;
}

int main(int argc, char *argv[]) {
    char delim[16] = " \t\n"; // �⺻ ������: ����, ��, ����
    char *script = NULL;
    char *filename = NULL;
    FILE *fp;

    // ����� ���� �Ľ�
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-F") == 0 && i + 1 < argc) {
            strncpy(delim, argv[++i], sizeof(delim)-1);
        } else if (!script) {
            script = argv[i]; // ������ ��ũ��Ʈ(��: $2)
        } else {
            filename = argv[i];
        }
    }

    if (!script) {
        fprintf(stderr, "����: %s [-F ������] '��ũ��Ʈ' [����]\n", argv[0]);
        return 1;
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

    char line[MAX_LINE];
    char *fields[MAX_FIELDS];
    int nf;

    while (fgets(line, sizeof(line), fp)) {
        // �� �� ���� ����
        size_t len = strlen(line);
        if (len && line[len-1] == '\n') line[len-1] = '\0';

        // �ʵ� �и�
        nf = split(line, fields, delim);

        // ������ ��ũ��Ʈ: $N ���ĸ� ����
        if (script[0] == '$') {
            int idx = atoi(script + 1);
            if (idx >= 1 && idx <= nf)
                printf("%s\n", fields[idx-1]);
        } else if (strcmp(script, "{print}") == 0) {
            // ��� �� ���
            for (int i = 0; i < nf; i++)
                printf("%s%c", fields[i], (i == nf-1 ? '\n' : ' '));
        }
        // �� ������ ��ũ��Ʈ�� �ʿ信 ���� Ȯ�� ����
    }

    if (fp != stdin) fclose(fp);
    return 0;
}
/*
split �Լ�: �Է� ���� �����ڷ� �и��� �ʵ� �迭�� ����

-F �ɼ�: ������ ����(�⺻�� ����/��/����)

��ũ��Ʈ: $N(N��° �ʵ� ���), {print}(��� �ʵ� ���) ����

���ϸ� ������ ǥ�� �Է� ���
*/
