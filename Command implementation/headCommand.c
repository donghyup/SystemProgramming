#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_LINE_COUNT 10    // �⺻ ��� �� ��

int main(int argc, char *argv[]) {
    int n = DEFAULT_LINE_COUNT;  // ����� �� ��, �⺻�� 10
    char *filename = NULL;       // ���ϸ� ���� ����
    FILE *fp;
    char buffer[1024];           // �� �پ� ���� ����
    int line_count = 0;          // ����� �� �� ī��Ʈ

    // ����� ���� �Ľ�
    for (int i = 1; i < argc; i++) {
        // -n �ɼ� ó��
        if (argv[i][0] == '-' && argv[i][1] == 'n') {
            if (i + 1 < argc) {
                n = atoi(argv[i + 1]); // �ɼǰ��� ������ ��ȯ
                i++;                   // �ɼǰ� �ε��� �ǳʶ�
            }
        } else {
            filename = argv[i];        // ���ϸ� ����
        }
    }

    // ���� ����, ���ϸ��� ������ ǥ�� �Է� ���
    if (filename) {
        fp = fopen(filename, "r");
        if (!fp) {
            perror("������ �� �� �����ϴ�");
            return 1;
        }
    } else {
        fp = stdin;
    }

    // ���Ͽ��� �� �پ� �о ���, ������ �� ����ŭ��
    while (fgets(buffer, sizeof(buffer), fp)) {
        printf("%s", buffer);
        line_count++;
        if (line_count >= n) break; // ������ �� ����ŭ ����ϸ� ����
    }

    if (fp != stdin) fclose(fp); // ���� �ݱ�
    return 0;
}


