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



/*Ouverture d'un conduit nommé */
struct conduct * conduct_open(const char *name){
  struct conduct * conduit;
  struct stat file;
  int fd=open(name,O_RDWR);

  if(fd<0){
    perror("File doesn't open");
    exit(0);
    }

  if(fstat(fd,&file)==-1){
    perror("Problem with fstat");
  };

  conduit=mmap(NULL,file.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
  if(conduit==MAP_FAILED){
    perror("MMAP FAILED");
    exit(1);
  }
  return conduit;

}
