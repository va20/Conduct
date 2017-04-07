#include "conduct.h"

struct conduct *conduct_create(const char *name,size_t c,size_t a){
    struct conduct* conduit=NULL;
    if(name!=NULL){
        conduit->fd=open(name,O_RDWR);
        if(conduit->fd==-1){
            perror("Ouverture du fichier a echoue");
            exit(3);
        }
        conduit=mmap(NULL,sizeof(struct conduct),PROT_READ|PROT_WRITE,MAP_SHARED ,conduit->fd, 0);
        if(conduit==MAP_FAILED){
            perror("Mapping failed");
            exit(1);
        }
        conduit->buff=malloc(sizeof(char)*c);
        if(conduit->buff==NULL){
            perror("Allocation dynamique buff a echoue");
            exit(2);
        }
        conduit->capacity=c;
        conduit->atomicity=a;
    }
    else if(name==NULL){
        conduit=mmap(NULL,sizeof(struct conduct),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1, 0);
        if(conduit==MAP_FAILED){
            perror("Mapping failed");
            exit(1);
        }
    }
    return conduit;
}
