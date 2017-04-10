#include "conduct.h"


struct conduct *conduct_create(const char *name,size_t c,size_t a){
    struct conduct* conduit=NULL;
    int fc1;
    if(name!=NULL){
        fc1=open(name,O_CREAT|O_RDWR,0666);

        if(fc1==-1){
            perror("Ouverture du fichier a echoue");
            exit(3);
        }
        if(ftruncate(fc1,c)==1){
          perror("ftruncate failed");
          exit(2);
        }
        conduit=mmap(NULL,c,PROT_READ|PROT_WRITE,MAP_PRIVATE ,fc1, 0);
        if(conduit==MAP_FAILED){
            perror("Mapping failed");
            exit(1);
        }

        conduit->capacity=c;
        conduit->atomicity=a;
        conduit->fd=fc1;
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
  int fc2=open(name,O_RDWR);


  if(fc2<0){
    perror("File doesn't open");
    exit(0);
    }

  if(fstat(fc2,&file)==-1){
    perror("Problem with fstat");
  }

  conduit=mmap(NULL,file.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fc2,0);
  if(conduit==MAP_FAILED){
    perror("MMAP FAILED");
    exit(1);
  }

  conduit->fd=fc2;


  return conduit;

}
