#include "conduct.h"

int main(int argc, char const *argv[]) {
    struct conduct *cond1;
    cond1=conduct_open("saif2");
    char tab[20]={'\0'};
    printf("taille tab %ld\n",strlen(tab));
    ssize_t t=conduct_read(cond1, (char*)tab, 2);
    printf("lu %ld\n",t );
	printf("buffer read %s\n",tab);
    conduct_close(cond1);
    return 0;
}
