#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <unistd.h>   // mkstemp, unlink, rename
#include <libgen.h>   // dirname, basename

#define MAX_LINE 4096

// sed�� s/����/��ü/ ���¸� �Ľ��ϴ� �Լ�
int parse_subst_command(const char *cmd, char *pattern, char *replacement) {
    if (cmd[0] != 's') return 0;
    char delim = cmd[1];
    const char *p = cmd + 2;
    const char *pat_start = p;
    const char *pat_end = strchr(p, delim);
    if (!pat_end) return 0;
    size_t pat_len = pat_end - pat_start;
    strncpy(pattern, pat_start, pat_len);
    pattern[pat_len] = '\0';

    const char *rep_start = pat_end + 1;
    const char *rep_end = strchr(rep_start, delim);
    if (!rep_end) return 0;
    size_t rep_len = rep_end - rep_start;
    strncpy(replacement, rep_start, rep_len);
    replacement[rep_len] = '\0';

    return 1;
}

// �� �ٿ��� ���Խ� ġȯ(ù ��Ī��)
void regex_substitute_line(char *line, const regex_t *regex, const char *replacement, char *result) {
    regmatch_t match;
    char *src = line;
    char *dst = result;
    result[0] = '\0';

    while (regexec(regex, src, 1, &match, 0) == 0) {
        int prefix_len = match.rm_so;
        strncpy(dst, src, prefix_len);
        dst += prefix_len;
        strcpy(dst, replacement);
        dst += strlen(replacement);
        src += match.rm_eo;
        // �� �� �� ���� ��Ī ��� ġȯ�Ϸ��� �Ʒ� continue �ּ� ����
        // continue;
        break; // �⺻ sed�� �� �ٿ� ù ��Ī�� ġȯ
    }
    strcpy(dst, src);
}

int main(int argc, char *argv[]) {
    int in_place = 0;
    char *subst_cmd = NULL;
    char *filename = NULL;

    // ����� ���� �Ľ�
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            in_place = 1;
        } else if (!subst_cmd) {
            subst_cmd = argv[i];
        } else {
            filename = argv[i];
        }
    }

    if (!subst_cmd || !filename) {
        fprintf(stderr, "����: %s [-i] 's/����/��ü/' ����\n", argv[0]);
        return 1;
    }

    char pattern[256], replacement[256];
    if (!parse_subst_command(subst_cmd, pattern, replacement)) {
        fprintf(stderr, "�߸��� ġȯ ��� �����Դϴ�.\n");
        return 1;
    }

    regex_t regex;
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        fprintf(stderr, "���Խ� ������ ����\n");
        return 1;
    }

    FILE *in_fp = fopen(filename, "r");
    if (!in_fp) {
        perror("������ �� �� �����ϴ�");
        regfree(&regex);
        return 1;
    }

    FILE *out_fp = stdout;
    char tmpfile[4096] = "";
    int tmpfd = -1;

    // -i �ɼ�: �ӽ� ���� ����
    if (in_place) {
        snprintf(tmpfile, sizeof(tmpfile), "%s.sedtmpXXXXXX", filename);
        tmpfd = mkstemp(tmpfile);
        if (tmpfd == -1) {
            perror("�ӽ� ���� ���� ����");
            fclose(in_fp);
            regfree(&regex);
            return 1;
        }
        out_fp = fdopen(tmpfd, "w");
        if (!out_fp) {
            perror("�ӽ� ���� ���� ����");
            close(tmpfd);
            unlink(tmpfile);
            fclose(in_fp);
            regfree(&regex);
            return 1;
        }
    }

    char line[MAX_LINE], result[MAX_LINE * 2];
    while (fgets(line, sizeof(line), in_fp)) {
        regex_substitute_line(line, &regex, replacement, result);
        fputs(result, out_fp);
    }

    fclose(in_fp);
    regfree(&regex);

    if (in_place) {
        fclose(out_fp);
        // ���� ���� ��� �� �ӽ� ������ ���� ���ϸ����� rename
        if (rename(tmpfile, filename) != 0) {
            perror("���� ����� ����");
            unlink(tmpfile);
            return 1;
        }
    }

    return 0;
}
/*
-i �ɼ��� ������ �ӽ� ������ �����Ͽ� ġȯ ����� �����ϰ�,
�Ϸ� �� ���� ������ �ӽ� ���Ϸ� ���(rename)

POSIX regex�� ���� ��Ī �� ġȯ

ġȯ ��ɾ�� sed ��Ÿ��(s/����/��ü/)�� ����

���ϸ��� �������� ������ ����, ǥ�� �Է� ������ �ʿ�� Ȯ�� ����
*/
