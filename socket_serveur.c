#include "conduct.h"

#define NB_MAX 20

int main(int argc, char const *argv[]) {
    unlink("socket");
    struct sockaddr_un address;
    struct sockaddr_un addr_client;
    socklen_t size;
    char buff[500];
    int fd_accept;
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
    int b=bind(fd_socket,(struct sockaddr*)&address, sizeof(struct sockaddr_un));
    if(b==-1){
        perror("bind erreur");
        exit(3);
    }
    if(listen(fd_socket,NB_MAX)==-1){
        perror("erreur listen");
        exit(4);
    }
    int i=0;
    while(i<10){
        size=sizeof(struct sockaddr_un);
        fd_accept=accept(fd_socket,(struct sockaddr*)&addr_client,&size);

        size_t lu=read(fd_accept,buff,300);
        printf("lu %ld\n",lu);
        printf("buff %s\n",buff);
        i++;
		sleep(2);
    }
    close(fd_accept);
    close(fd_socket);

    return 0;
}
