#include "conduct.h"

int main(int argc, char const *argv[]) {
    struct conduct *cond1;
    cond1=conduct_open("saif2");
    char tab[21]={'\0'};
    strcpy(tab,"c'est juste un teste plus long");
    ssize_t rep=conduct_write(cond1,tab, 14);
    printf("ecrivain 2 %ld\n",rep);
    conduct_close(cond1);
    return 0;
}
