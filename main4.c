#include "conduct.h"

void* print_func(void*);

int main(int argc, char const *argv[]) {
    struct conduct *cond1;
    cond1=conduct_open("saif2");
    char tab[20]={'\0'};
    strcpy(tab,"putain de fils de pu");
    printf("tab %s\n",tab);
    ssize_t rep=conduct_write(cond1,(void*)tab, 12);
    printf("write 2 %ld\n",rep);
    printf("buffer write %s\n",cond1->buff);
    conduct_close(cond1);
    return 0;
}
