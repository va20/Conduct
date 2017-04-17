#include "conduct.h"

int main(int argc, char const *argv[]) {
    struct conduct *cond1,*cond2,*cond3;
    cond1=conduct_create("saif", 20, 10);
	printf("atomicity %ld\n",cond1->atomicity);
    printf("capacity %ld\n",cond1->capacity);
    printf("buff %s\n",cond1->buff);
    printf("curseur_ecriture %d\n",cond1->curseur_ecriture);
    printf("curseur_lecture %d\n",cond1->curseur_lecture);
    printf("eof %d\n",cond1->eof);
    printf("cond ecri %p\n",cond1->cond_ecrivain);
    printf("cond lec %p\n",cond1->cond_lecteur);
    printf("place dispo %ld\n",cond1->place_restant);
    printf("verrou %p\n",cond1->verrou_buff);
    printf("name %s \n",cond1->name);
    cond2=conduct_open("saif");
    printf("atomicity %ld\n",cond2->atomicity);
    printf("capacity %ld\n",cond2->capacity);
    printf("buff %s\n",cond2->buff);
    printf("curseur_ecriture %d\n",cond2->curseur_ecriture);
    printf("curseur_lecture %d\n",cond2->curseur_lecture);
    printf("eof %d\n",cond2->eof);
    printf("cond ecri %p\n",cond2->cond_ecrivain);
    printf("cond lec %p\n",cond2->cond_lecteur);
    printf("place dispo %ld\n",cond2->place_restant);
    printf("verrou %p\n",cond2->verrou_buff);
    printf("name %s \n",cond2->name);
    cond3=conduct_open("saif");
    conduct_close(cond2);
    conduct_close(cond3);
    conduct_destroy(cond1);
    return 0;
}
