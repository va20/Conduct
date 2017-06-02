#include "conduct.h"

int main(int argc, char const *argv[]) {
    struct conduct *cond1;
    cond1=conduct_create("saif2",20,20);
    char tab[20]={'\0'};
    //strcpy(tab,"salutation");
    //printf("buffer conduit avant read %s\n",cond1->buff);
    printf("capacity %ld\n",cond1->capacity);
    printf("atomicity %ld\n",cond1->atomicity);
    printf("eof %d\n",cond1->eof);
    printf("taille buff %ld\n",cond1->taille_buff);
    printf("name %s\n",cond1->name);


    ssize_t rep=conduct_read(cond1,tab, 8);
    //printf("atom read %s\n",(char*)cond1-8);
    printf("tab read -> %s\n",tab);
	printf("dans buff %s\n",(char*)cond1+sizeof(struct conduct));
    printf("read 1 %ld\n",rep);
    //conduct_destroy(cond1);
    conduct_close(cond1);
    return 0;
}
