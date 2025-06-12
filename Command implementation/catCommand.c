#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_file(FILE *fp, int show_line_number) {
    char buffer[1024];
    int line = 1;

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (show_line_number) {
            printf("%6d  %s", line++, buffer);  // �� ��ȣ ���
        } else {
            printf("%s", buffer);  // �׳� ���� ���
        }
    }
}

int main(int argc, char *argv[]) {
    int show_line_number = 0;
    int start_idx = 1;

    // �ɼ� Ȯ��
    if (argc > 1 && strcmp(argv[1], "-n") == 0) {
        show_line_number = 1;
        start_idx = 2;
    }

    if (argc <= start_idx) {
        fprintf(stderr, "����: %s [-n] <����1> [����2 ...]\n", argv[0]);
        return 1;
    }

    // ���� ���� ��ȸ
    for (int i = start_idx; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (!fp) {
            perror(argv[i]);
            continue;
        }

        print_file(fp, show_line_number);
        fclose(fp);
    }

    return 0;
}

