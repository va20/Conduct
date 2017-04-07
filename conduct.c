#include "conduct.h"





struct conduct * conduct_create(char *name,size_t c,size_t a){
    struct conduct* conduct;
    if(name!=NULL){
        conduct=malloc(sizeof(struct conduct));
        conduct->buff=malloc(sizeof(char)*c);
        conduct->capacity=c;
        conduct->atomicity=a;
    }
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
