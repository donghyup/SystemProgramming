#include <stdio.h>
int main() {
        char name[100];
        int number_id;
        scanf("%d",&number_id);
        scanf("%s", name);
        printf("학번 : %d, 이름 : %s",number_id, name);
        return 0;
}
