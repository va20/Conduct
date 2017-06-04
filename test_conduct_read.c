#include "conduct.h"

int main(int argc, char const *argv[]) {
	clock_t start,end;
	double time_used;
	struct conduct *cond1;
	char tab[TAILLE_TEXT];
	cond1=conduct_create("saif2",TAILLE_TEXT,TAILLE_TEXT);
	start=clock();
	ssize_t rep=conduct_read(cond1, tab, TAILLE_TEXT);
	end=clock();
	time_used=((double)(end-start))/CLOCKS_PER_SEC;
	printf("conduct read %ld\n",rep);
	printf("Temps utilisé pour lire dans le conduit %f\n",time_used);
	conduct_destroy(cond1);
	return 0;

}
