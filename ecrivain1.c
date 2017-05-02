#include "conduct.h"

int main(int argc, char const *argv[]) {
    struct conduct *cond1;
    cond1=conduct_open("saif2");
    char tab[20]={'\0'};
    strcpy(tab,"salutation");
    printf("atom %s\n",tab);
    printf("atom %ld\n",cond1->capacity);
    //printf("buffer conduit avant write %s\n",cond1->buff);

    ssize_t rep=conduct_write(cond1, tab, 8);
    printf("atom %s\n",(char*)cond1+8);
    printf("write 1 %ld\n",rep);
    conduct_close(cond1);
    return 0;
}
