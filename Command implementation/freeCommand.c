#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN 256

// /proc/meminfo에서 원하는 필드의 값을 읽어옴 (kB 단위)
long long get_meminfo(const char *key, FILE *fp) {
    char line[LINE_LEN];
    long long value = -1;
    rewind(fp);
    while (fgets(line, sizeof(line), fp)) {
        if (strncmp(line, key, strlen(key)) == 0) {
            sscanf(line + strlen(key), ":%lld", &value);
            break;
        }
    }
    return value;
}

int main() {
    FILE *fp = fopen("/proc/meminfo", "r");
    if (!fp) {
        perror("/proc/meminfo 열기 실패");
        return 1;
    }

    long long mem_total = get_meminfo("MemTotal", fp);
    long long mem_free = get_meminfo("MemFree", fp);
    long long mem_available = get_meminfo("MemAvailable", fp);
    long long buffers = get_meminfo("Buffers", fp);
    long long cached = get_meminfo("Cached", fp);
    long long swap_total = get_meminfo("SwapTotal", fp);
    long long swap_free = get_meminfo("SwapFree", fp);

    fclose(fp);

    long long mem_used = mem_total - mem_free - buffers - cached; // 전통적 방식
    long long mem_buff_cache = buffers + cached;
    long long swap_used = swap_total - swap_free;

    printf("              total        used        free      shared  buff/cache   available\n");
    printf("Mem:    %10lld %10lld %10lld %10s %10lld %10lld\n",
        mem_total, mem_used, mem_free, "-", mem_buff_cache, mem_available);
    printf("Swap:   %10lld %10lld %10lld\n",
        swap_total, swap_used, swap_free);

    return 0;
}
//컴파일 : gcc -o myfree myfree.c
//실행 : gcc -o myfree myfree.c
/*
코드 설명 (주석 참고)
/proc/meminfo 파일을 열고, 각 필드(MemTotal, MemFree, MemAvailable, Buffers, Cached, SwapTotal, SwapFree)를 파싱

used, buff/cache, available 등은 free 명령어의 표와 동일한 방식으로 계산 및 출력

shared는 간단 예시에서는 생략("-"로 표시), 필요시 Shmem 필드 추가 가능

단위는 kB(킬로바이트) 기준
*/ 
