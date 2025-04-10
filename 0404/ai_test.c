#include <stdio.h>

int main() {
    int num;
    int count = 0;

    // 사용자로부터 정수 입력 받기
    printf("0부터 255 사이의 정수를 입력하세요: ");
    scanf("%d", &num);

    // 유효성 검사
    if (num < 0 || num > 255) {
        printf("입력한 숫자가 범위를 벗어났습니다.\n");
        return 1;
    }

    printf("이진수: ");
    for (int i = 7; i >= 0; i--) {
        int bit = (num >> i) & 1;
        printf("%d", bit);
        if (bit == 1) {
            count++;
        }
    }

    printf("\n1의 개수: %d\n", count);

    return 0;
}
