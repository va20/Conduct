#include "conduct.h"

int main(int argc, char const *argv[]) {

	int fd=open("tube",O_WRONLY);
	if(fd==-1){
		perror("Erreur ouverture pipe");
		exit(1);
	}
	char *mess=malloc(sizeof(char)*strlen(argv[1])+1);
	strcpy(mess,argv[1]);
	mess[strlen(mess)]='\0';
	int w=write(fd, mess, strlen(mess)*sizeof(char));
	if(w<=0){
		perror("Erreur ecriture");
		exit(4);
	}
	free(mess);
	return 0;
}
