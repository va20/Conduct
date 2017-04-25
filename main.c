#include "conduct.h"

int main(int argc, char const *argv[]) {
    struct conduct *cond1;
    cond1=conduct_create("saif2",20,20);
    char tab[20]={'\0'};
    //strcpy(tab,"salutation");
    printf("atom %s\n",cond1->buff);
    printf("atom %ld\n",cond1->capacity);

    ssize_t rep=conduct_read(cond1, tab, 8);
    printf("atom %s\n",cond1->buff);
    printf("read 1 %ld\n",rep);
    conduct_destroy(cond1);
    return 0;
}
