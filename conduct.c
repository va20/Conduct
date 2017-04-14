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
        if(ftruncate(fc1,c)==-1){
          perror("ftruncate failed");
          exit(2);
        }
        conduit=mmap(NULL,c,PROT_READ|PROT_WRITE,MAP_SHARED,fc1, 0);
        if(conduit==MAP_FAILED){
            perror("Mapping failed");
            exit(1);
        }

        conduit->capacity=c;
        conduit->atomicity=a;
        conduit->buff=malloc(sizeof(char)*conduit->capacity);
		    conduit->eof=0;
        conduit->place_restant=conduit->capacity;
        conduit->curseur_ecriture=0;
        conduit->curseur_lecture=0;

        pthread_mutex_t verrou;
        pthread_mutex_init(&verrou, NULL);
        conduit->verrou_buff=&verrou;

        pthread_cond_t cond_ecr;
	      pthread_cond_init(&cond_ecr,NULL);
	      pthread_cond_t cond_lec;
	      pthread_cond_init(&cond_lec,NULL);

        conduit->cond_ecrivain=&cond_ecr;
        conduit->cond_lecteur=&cond_lec;

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

  return conduit;

}

ssize_t conduct_read(struct conduct *c,void* buff,size_t count){
  ssize_t lu;
  if(strlen(c->buff)==0){

  }
  else if(strlen(c->buff)>0){
    if(count<=strlen(c->buff)){
      //lu=read(c->fd,buff,count);
      if(lu==-1){
        perror("Conduct reading failed : ");
        return -1;
      }
      return count;
    }
    else if(count > strlen(c->buff)){
      //lu=read(c->fd,buff,count);
      if(lu==-1){
        perror("Conduct reading failed : ");
        return -1;
      }
      return lu;
    }
  }
  return 0;
}

int conduct_write_eof(struct conduct *c){
  c->eof=1;
  return 0;
}

/*Fonction d'écriture dans le conduit */
ssize_t conduct_write(struct conduct *c, const void *buf, size_t count){
    if(strlen(c->buff)==c->capacity){


    }
    if(strlen(c->buff)<c->capacity ){
      int s=sprintf(c->buff, buf);
      if(s==-1){
        perror("Writting Failed");
      }
      return s;

    }



};
