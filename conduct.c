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
    return conduit;
  }



/*Ouverture d'un conduit nommé */
struct conduct * conduct_open(char *name){
  struct conduct * conduct;
  struct stat file;
  int fd=open(name,O_RDWR);

  if(fd<0){
    perror("File doesn't open");
    exit(0);
    }

  if(fstat(fd,&file)==-1){
    perror("Problem with fstat");
  };

  conduct=mmap(NULL,file.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
  if(conduct==MAP_FAILED){
    perror("MMAP FAILED");
    exit(1);
  }



}
