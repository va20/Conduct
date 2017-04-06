#include "conduct.h"

struct conduct *conduct_create(const char *name,size_t c,size_t a){
    struct conduct* conduit=NULL;
    if(name!=NULL){
        conduit=malloc(sizeof(struct conduct));
        if(conduit==NULL){
            perror("Allocation dynamique struct conduct a echoue");
            exit(1);
        }
        conduit->buff=malloc(sizeof(char)*c);
        if(conduit->buff==NULL){
            perror("Allocation dynamique buff a echoue");
            exit(2);
        }
        conduit->capacity=c;
        conduit->atomicity=a;
        conduit->fd=open(name,O_RDWR);
        if(conduit->fd==-1){
            perror("Ouverture du fichier a echoue");
            exit(3);
        }
    }
}
