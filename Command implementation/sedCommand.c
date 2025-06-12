#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <unistd.h>   // mkstemp, unlink, rename
#include <libgen.h>   // dirname, basename

#define MAX_LINE 4096

// sed의 s/패턴/대체/ 형태를 파싱하는 함수
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

// 한 줄에서 정규식 치환(첫 매칭만)
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
        // 한 줄 내 여러 매칭 모두 치환하려면 아래 continue 주석 해제
        // continue;
        break; // 기본 sed는 한 줄에 첫 매칭만 치환
    }
    strcpy(dst, src);
}

int main(int argc, char *argv[]) {
    int in_place = 0;
    char *subst_cmd = NULL;
    char *filename = NULL;

    // 명령줄 인자 파싱
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
        fprintf(stderr, "사용법: %s [-i] 's/패턴/대체/' 파일\n", argv[0]);
        return 1;
    }

    char pattern[256], replacement[256];
    if (!parse_subst_command(subst_cmd, pattern, replacement)) {
        fprintf(stderr, "잘못된 치환 명령 형식입니다.\n");
        return 1;
    }

    regex_t regex;
    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        fprintf(stderr, "정규식 컴파일 실패\n");
        return 1;
    }

    FILE *in_fp = fopen(filename, "r");
    if (!in_fp) {
        perror("파일을 열 수 없습니다");
        regfree(&regex);
        return 1;
    }

    FILE *out_fp = stdout;
    char tmpfile[4096] = "";
    int tmpfd = -1;

    // -i 옵션: 임시 파일 생성
    if (in_place) {
        snprintf(tmpfile, sizeof(tmpfile), "%s.sedtmpXXXXXX", filename);
        tmpfd = mkstemp(tmpfile);
        if (tmpfd == -1) {
            perror("임시 파일 생성 실패");
            fclose(in_fp);
            regfree(&regex);
            return 1;
        }
        out_fp = fdopen(tmpfd, "w");
        if (!out_fp) {
            perror("임시 파일 열기 실패");
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
        // 원본 파일 백업 후 임시 파일을 원본 파일명으로 rename
        if (rename(tmpfile, filename) != 0) {
            perror("파일 덮어쓰기 실패");
            unlink(tmpfile);
            return 1;
        }
    }

    return 0;
}
/*
-i 옵션이 있으면 임시 파일을 생성하여 치환 결과를 저장하고,
완료 후 원본 파일을 임시 파일로 덮어씀(rename)

POSIX regex로 패턴 매칭 및 치환

치환 명령어는 sed 스타일(s/패턴/대체/)만 지원

파일명을 지정하지 않으면 에러, 표준 입력 지원은 필요시 확장 가능
*/
