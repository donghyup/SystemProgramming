#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_LINE 4096

// ���ڿ� �� �κ� ���ڿ�(needle)�� ���ԵǾ� �ִ���(��ҹ��� ����)
int strcasestr_simple(const char *haystack, const char *needle) {
    // �ҹ��ڷ� ��ȯ�Ͽ� strstr ���
    char *h = strdup(haystack);
    char *n = strdup(needle);
    for (char *p = h; *p; ++p) *p = tolower(*p);
    for (char *p = n; *p; ++p) *p = tolower(*p);
    int found = strstr(h, n) != NULL;
    free(h); free(n);
    return found;
}

// �� ���Ͽ��� ���ڿ� �˻�
void grep_file(const char *filename, const char *pattern, int ignore_case, int invert_match) {
    FILE *fp = fopen(filename, "r");
    if (!fp) return;

    char line[MAX_LINE];
    int matched;

    while (fgets(line, sizeof(line), fp)) {
        if (ignore_case)
            matched = strcasestr_simple(line, pattern);
        else
            matched = strstr(line, pattern) != NULL;

        // -v �ɼ�: ��Ī �� �Ǵ� �� ���
        if (invert_match) matched = !matched;

        if (matched) {
            printf("%s:%s", filename, line);
        }
    }
    fclose(fp);
}

// ���丮 ��ȸ(���, -r �ɼ�)
void grep_dir(const char *dirname, const char *pattern, int ignore_case, int invert_match) {
    DIR *dir = opendir(dirname);
    if (!dir) return;

    struct dirent *entry;
    char path[4096];

    while ((entry = readdir(dir)) != NULL) {
        // . �� .. ����
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);

        struct stat st;
        stat(path, &st);
        if (S_ISDIR(st.st_mode)) {
            // ���丮�� ��� ȣ��
            grep_dir(path, pattern, ignore_case, invert_match);
        } else if (S_ISREG(st.st_mode)) {
            // �Ϲ� �����̸� �˻�
            grep_file(path, pattern, ignore_case, invert_match);
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[]) {
    int ignore_case = 0;   // -i �ɼ�
    int invert_match = 0;  // -v �ɼ�
    int recursive = 0;     // -r �ɼ�
    char *pattern = NULL;
    char *target = NULL;

    // ����� ���� �Ľ�
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) ignore_case = 1;
        else if (strcmp(argv[i], "-v") == 0) invert_match = 1;
        else if (strcmp(argv[i], "-r") == 0) recursive = 1;
        else if (!pattern) pattern = argv[i];
        else if (!target) target = argv[i];
    }

    if (!pattern || !target) {
        fprintf(stderr, "����: %s [-i] [-v] [-r] ���� ����|���丮\n", argv[0]);
        return 1;
    }

    struct stat st;
    stat(target, &st);
    if (recursive && S_ISDIR(st.st_mode)) {
        grep_dir(target, pattern, ignore_case, invert_match); // -r ���丮 ��� �˻�
    } else {
        grep_file(target, pattern, ignore_case, invert_match); // �Ϲ� ���� �˻�
    }
    return 0;
}

