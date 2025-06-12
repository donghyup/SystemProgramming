#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     // sleep �Լ� ���
#include <sys/stat.h>   // ���� ũ�� Ȯ�ο�

#define DEFAULT_LINE_COUNT 10
#define MAX_LINE_LENGTH 1024
#define MAX_LINES 4096  // tail���� �ִ� ������ �� ��

// tail -n N: ���� ������ N�� ���
void tail_lines(const char *filename, int n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("������ �� �� �����ϴ�");
        return;
    }

    // ��ȯ ���۷� ������ N�� ����
    char *lines[MAX_LINES];
    int count = 0;
    char buffer[MAX_LINE_LENGTH];

    for (int i = 0; i < MAX_LINES; i++) lines[i] = NULL;

    while (fgets(buffer, sizeof(buffer), fp)) {
        if (lines[count % n]) free(lines[count % n]);
        lines[count % n] = strdup(buffer); // �� �� ���� ����
        count++;
    }

    int start = count > n ? count % n : 0;
    int total = count < n ? count : n;

    for (int i = 0; i < total; i++) {
        printf("%s", lines[(start + i) % n]);
        free(lines[(start + i) % n]);
    }

    fclose(fp);
}

// tail -f: ���� ���������� �ǽð� ���(�α� ����͸�)
void tail_follow(const char *filename, int n) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("������ �� �� �����ϴ�");
        return;
    }

    // ���� ������ n�� ���
    tail_lines(filename, n);

    // ���� ������ �̵�
    fseek(fp, 0, SEEK_END);

    struct stat st;
    long last_size = 0;
    stat(filename, &st);
    last_size = st.st_size;

    char buffer[MAX_LINE_LENGTH];

    // ���� ����: ���� ũ�� ���� ���� �� �� ���� ���
    while (1) {
        stat(filename, &st);
        if (st.st_size > last_size) {
            // ���� �߰��� �κ� �б�
            while (fgets(buffer, sizeof(buffer), fp)) {
                printf("%s", buffer);
            }
            last_size = st.st_size;
        }
        fflush(stdout);
        sleep(1); // 1�ʸ��� üũ
    }
    fclose(fp);
}

int main(int argc, char *argv[]) {
    int n = DEFAULT_LINE_COUNT; // ����� �� ��
    int follow = 0;             // -f �ɼ� ����
    char *filename = NULL;

    // ����� ���� �Ľ�
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            n = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-f") == 0) {
            follow = 1;
        } else {
            filename = argv[i];
        }
    }

    if (!filename) {
        fprintf(stderr, "����: %s [-n �ټ�] [-f] ���ϸ�\n", argv[0]);
        return 1;
    }

    if (follow) {
        tail_follow(filename, n); // tail -f
    } else {
        tail_lines(filename, n);  // tail -n N
    }
    return 0;
}

