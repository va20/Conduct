#include "conduct.h"

int main(int argc, char const *argv[]) {
    struct conduct *cond1;
    cond1=conduct_open("saif");
    char tab[20]={'\0'};
    strcpy(tab,"putain de fils de pu");
    ssize_t rep=conduct_write(cond1, tab, 12);
    printf("write %ld\n",rep);
    //conduct_destroy(cond1);
    return 0;
}
