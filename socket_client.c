#include "conduct.h"

int main(int argc, char const *argv[]) {
	clock_t start,end;
	double time_used;
	struct sockaddr_un address;
	int fd_socket=socket(AF_UNIX,SOCK_STREAM,0);
	if(fd_socket<0){
		perror("error socket");
		exit(2);
	}
	address.sun_family=AF_UNIX;
	if(strcpy(address.sun_path,"socket")==NULL){
        printf("Erreur strcpy\n");
        return -1;
    }

	if(connect(fd_socket,(struct sockaddr*)&address,sizeof(struct sockaddr_un))==-1){
		perror("erreur connect");
		exit(3);
	}
	start=clock();
	int ecr=write(fd_socket,TEXT,sizeof(char)*TAILLE_TEXT);
	if(ecr<=0){
		perror("Erreur ecriture socket");
		exit(4);
	}
	end=clock();
	time_used=((double)(end-start))/CLOCKS_PER_SEC;
	printf("Temps utilisé pour ecrire dans la socket: %f\n",time_used);
	printf("J'ai ecrit %d\n",ecr);
	close(fd_socket);
	return 0;
}
