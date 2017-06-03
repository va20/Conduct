#include <conduct.h>

int main(int argc, char const *argv[]) {
	struct sockaddr_un address;
	int fd_socket=socket(AF_UNIX,SOCK_STREAM,0);
	char buff[500];
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
	sleep(5);
	read(0,buff,500);
	int ecr=write(fd_socket,buff,sizeof(char)*strlen(buff));
	memset(buff, 0,sizeof(char)*strlen(buff));
	printf("ecrire %d\n",ecr);
	sleep(5);

	close(fd_socket);
	return 0;
}
