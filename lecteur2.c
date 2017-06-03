#include "conduct.h"

int main(int argc, char const *argv[]) {
    struct conduct *cond1;
    cond1=conduct_open("saif2");
    char tab[20]={'\0'};
    ssize_t t=conduct_read(cond1, tab, 2);
    printf("lecteur 2 : J'ai lu -> %s\n",tab);
    printf("Nombre d'octets lus lecteur 2 %ld\n",t);
    conduct_close(cond1);
    return 0;
}
