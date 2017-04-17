#include "conduct.h"

int main(int argc, char const *argv[]) {
    struct conduct *cond1;
    cond1=conduct_create("saif", 20, 10);
    strcpy(cond1->buff,"Hello world!");
    char tab[20];
    ssize_t t=conduct_read(cond1, (char*)tab, 5);
    printf("lu %ld\n",t);
    conduct_destroy(cond1);
    return 0;
}
