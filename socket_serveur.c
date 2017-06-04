#include "conduct.h"

#define NB_MAX 20


void handler(int sig){
    unlink("tube");
    _exit(EXIT_SUCCESS);
}


int main(int argc, char const *argv[]) {

    struct sigaction sa;
    int rc;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    rc = sigaction(SIGINT, &sa, NULL);
    if(rc < 0) {
        perror("sigaction");
        exit(1);
    }

    clock_t start,end;
    double time_used;
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
    int lue=0;
    while(1){
        size=sizeof(struct sockaddr_un);
        fd_accept=accept(fd_socket,(struct sockaddr*)&addr_client,&size);
        start=clock();
        while((lue=read(fd_accept,buff,300))!=0){
            buff[lue]=0;
        }
        end=clock();
        time_used=((double)(end-start))/CLOCKS_PER_SEC;
        printf("Temps utilisé pour lire dans le tube : %f\n",time_used);
        printf("J'ai lu %d\n",lue);
    }
    close(fd_accept);
    close(fd_socket);

    return 0;
}
