#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN 256

// /proc/meminfo���� ���ϴ� �ʵ��� ���� �о�� (kB ����)
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
        perror("/proc/meminfo ���� ����");
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

    long long mem_used = mem_total - mem_free - buffers - cached; // ������ ���
    long long mem_buff_cache = buffers + cached;
    long long swap_used = swap_total - swap_free;

    printf("              total        used        free      shared  buff/cache   available\n");
    printf("Mem:    %10lld %10lld %10lld %10s %10lld %10lld\n",
        mem_total, mem_used, mem_free, "-", mem_buff_cache, mem_available);
    printf("Swap:   %10lld %10lld %10lld\n",
        swap_total, swap_used, swap_free);

    return 0;
}
//������ : gcc -o myfree myfree.c
//���� : gcc -o myfree myfree.c
/*
�ڵ� ���� (�ּ� ����)
/proc/meminfo ������ ����, �� �ʵ�(MemTotal, MemFree, MemAvailable, Buffers, Cached, SwapTotal, SwapFree)�� �Ľ�

used, buff/cache, available ���� free ��ɾ��� ǥ�� ������ ������� ��� �� ���

shared�� ���� ���ÿ����� ����("-"�� ǥ��), �ʿ�� Shmem �ʵ� �߰� ����

������ kB(ų�ι���Ʈ) ����
*/ 
