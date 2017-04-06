#include "conduct.h"

struct conduct * conduct_create(char *name,size_t c,size_t a){
    struct conduct* conduct=NULL;
    if(name!=NULL){
        conduct=malloc(sizeof(struct conduct));
        conduct->buff=malloc(sizeof(char)*c);
        conduct->capacity=c;
        conduct->atomicity=a;
    }
}
