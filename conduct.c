#include "conduct.h"


struct conduct *conduct_create(const char *name,size_t c,size_t a){
    struct conduct* conduit=NULL;
    int fd;
    if(name!=NULL){
        fd=open(name,O_CREAT|O_RDWR,0666);
        if(fd==-1){
            perror("Ouverture du fichier a echoue");
            exit(3);
        }
        ftruncate(fd,sizeof(struct conduct));
        conduit=mmap(NULL,c,PROT_READ|PROT_WRITE,MAP_SHARED ,fd, 0);
        if(conduit==MAP_FAILED){
            perror("Mapping failed");
            exit(1);
        }

        conduit->capacity=c;
        conduit->atomicity=a;
        conduit->fd=fd;
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
  }

  conduit=mmap(NULL,file.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
  if(conduit==MAP_FAILED){
    perror("MMAP FAILED");
    exit(1);
  }
  return conduit;

}
