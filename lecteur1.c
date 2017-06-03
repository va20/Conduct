#include "conduct.h"

int main(int argc, char const *argv[]) {
    struct conduct *cond1;
    cond1=conduct_create("saif2",20,20);
    char tab[20]={'\0'};
    ssize_t rep=conduct_read(cond1,tab, 8);
    printf("lecteur 1 : J'ai lu -> %s\n",tab);
    printf("Nombre d'octets lus lecteur 1 %ld\n",rep);
    conduct_close(cond1);
    return 0;
}
