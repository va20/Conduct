#include "conduct.h"

int main(int argc, char const *argv[]) {
    struct conduct *cond1;
    cond1=conduct_create("saif", 20, 10);
    strcpy(cond1->buff,"Hello world!");
    char tab[20]={'\0'};
    printf("taille tab %ld\n",strlen(tab));
    ssize_t t=conduct_read(cond1, (char*)tab, 3);
    ssize_t t2=conduct_read(cond1, (char*)tab, 5);
    ssize_t t3=conduct_read(cond1, (char*)tab, 5);
    ssize_t t4=conduct_read(cond1, (char*)tab, 1);
    printf("lu %ld\n",t);
    printf("lu %ld\n",t2);
    printf("lu %ld\n",t3);
    printf("lu %ld\n",t4);

    conduct_destroy(cond1);
    return 0;
}
