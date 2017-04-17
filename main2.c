#include "conduct.h"

int main(int argc, char const *argv[]) {
    struct conduct *cond1;

    cond1=conduct_create("saif2",20,20);
    char tab[20]={'\0'};
    //strcpy(tab,"salutation");
    printf("atom %s\n",cond1->buff);
    printf("atom %s\n",cond1->capacity);

    ssize_t rep=conduct_write(cond1, tab, 8);
    //printf("write 1 %ld\n",rep);
    conduct_close(cond1);
    return 0;
}
