#include "conduct.h"

int main(int argc, char const *argv[]) {
    struct conduct *cond1;
    cond1=conduct_open("saif2");
    char tab[20]={'\0'};
    strcpy(tab,"salutation");
    ssize_t rep=conduct_write(cond1, tab, 10);
    printf("ecrivain 1 %ld\n",rep);
    conduct_close(cond1);
    return 0;
}
