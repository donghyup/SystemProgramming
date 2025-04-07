#include <stdio.h>

void print_binary(int n) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (n >> i) & 1);
    }
}

int count_ones(int n) {
    int count = 0;
    for (int i = 0; i < 8; i++) {
        if ((n >> i) & 1) {
            count++;
        }
    }
    return count;
}

int main() {
    int num;

    // 사용자로부터 0~255 사이의 숫자 입력 받기
    printf("0~255 사이의 숫자를 입력하세요: ");
    scanf("%d", &num);

    // 입력값이 0~255 사이인지 확인
    if (num < 0 || num > 255) {
        printf("입력 값이 범위를 벗어났습니다. 0~255 사이의 값을 입력해주세요.\n");
        return 1;
    }

    // 2진수 출력
    printf("2진수: ");
    print_binary(num);
    printf("\n");

    // 1의 개수 출력
    printf("1의 개수: %d\n", count_ones(num));

    // 상위 4비트 출력
    printf("상위 4비트: ");
    for (int i = 7; i >= 4; i--) {
        printf("%d", (num >> i) & 1);
    }
    printf("\n");

    return 0;
}
