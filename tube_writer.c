#include "conduct.h"

int main(int argc, char const *argv[]) {
	clock_t start,end;
	double time_used;
	int fd=open("tube",O_WRONLY);
	if(fd==-1){
		perror("Erreur ouverture pipe");
		exit(1);
	}
	start=clock();
	int w=write(fd, TEXT, TAILLE_TEXT*sizeof(char));
	if(w<=0){
		perror("Erreur ecriture");
		exit(4);
	}
	end=clock();
	time_used=((double)(end-start))/CLOCKS_PER_SEC;
	printf("Temps utilisé pour ecrire dans le tube: %f\n",time_used);
	printf("J'ai ecrit %d\n",w);
	return 0;
}
