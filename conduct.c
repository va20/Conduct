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
        if(ftruncate(fc1,sizeof(struct conduct))==-1){
          perror("ftruncate failed");
          exit(2);
        }
        conduit=mmap(NULL,sizeof(struct conduct),PROT_READ|PROT_WRITE,MAP_SHARED,fc1, 0);
        if(conduit==MAP_FAILED){
            perror("Mapping failed");
            exit(1);
        }

        conduit->capacity=c;
        conduit->atomicity=a;
        //conduit->buff=malloc(conduit->capacity*sizeof(char));
        conduit->buff[conduit->capacity];
        memset(conduit->buff,0,sizeof(char)*conduit->capacity);
        conduit->name=malloc(strlen(name)*sizeof(char));
        if(strcpy(conduit->name, name)==NULL){
          printf("Error copy name\n");
          exit(6);
        }

		    conduit->eof=0;
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
  // Prendre le verrou
  pthread_mutex_lock(c->verrou_buff);

  //si le buffer est vide et sans marque de fin de fichier -> la fonction bloque jusqu'à ce que:
  // 1) le buffer ne soit plus vide
  // 2) une marque de fin de fichier soit y insérée
  while(strlen(c->buff)==0 && c->eof==0){

    // Reveiller les ecrivains qui attendent pour ecrire (s'ils existent)
    pthread_cond_broadcast(c->cond_ecrivain);
    // Bloquer jusqu'à ce que : le buffer ne soit plus vide ou une marque de fin de fichier soit y insérée
    pthread_cond_wait(c->cond_lecteur,c->verrou_buff);

  }
  // si le buffer n'est pas vide

  if(strlen(c->buff)>0){
    // si le nombre d'octets a exraire est inferieur à la taille de la chaine dans le buffer :
    if(count<=strlen(c->buff)){
      // 1) si le cursuer de la lecture n'est pas encore arriver à la fin du buffer
      if(count+c->curseur_lecture <= c->capacity){
        //copier les octets demandes par le lecteur dans buff
        buff=memcpy(buff,c->buff+c->curseur_lecture,count);
        printf("j'ai lu %s\n",buff);
        printf("taille buffer conduit %ld\n",strlen(c->buff));

        //buff[strlen(buff)-1]='\0';
        if(buff==NULL){
          printf("Conduct reading failed\n");
          return -1;
        }
        // effacer les octets lus du buffer de conduit
        memmove(c->buff+c->curseur_lecture, c->buff+count, strlen(c->buff));
        printf("buffer conduit %s\n",c->buff);
        lu=strlen(buff);
        // reveiller les ecrivains pour qu'ils puissent ecrire (s'il y a des ecrivains qui attendent)
        pthread_cond_broadcast(c->cond_ecrivain);
        return lu;
      }
      // 2) si le curseur de la lecture va depasser la capacity(taille du buffer) -> faire 2 lectures
      else if(count + c->curseur_lecture > c->capacity){
        int tmp=count;
        // copier la premiere partie lue dans le buffer
        buff=memcpy(buff,c->buff+c->curseur_lecture,c->capacity - c->curseur_lecture);
        if(buff==NULL){
          printf("Conduct reading failed first part\n");
          return -1;
        }
        // calculer le reste des octets a ajouter dans le buffer de lecture
        tmp=tmp - c->curseur_lecture;
        // effacer la premiere partie lue
        memmove(c->buff+c->curseur_lecture, c->buff+(c->capacity-c->curseur_lecture), strlen(c->buff)-(c->capacity-c->curseur_lecture));
        // revenir au debut du buffer de conduit
        c->curseur_lecture=0;
        char tab[tmp];
        // copier la 2eme partie lue dans un buffer temporaire
        memcpy(tab, c->buff+c->curseur_lecture,tmp);
        printf("j'ai lu %s\n",buff);
        // concatener les 2 buffers
        if(strcat(buff, tab)==NULL){
          printf("Error concatenation \n");
          return -1;
        }
        // effacer la 2eme partie lue
        memmove(c->buff+c->curseur_lecture, c->buff+(tmp+1), strlen(c->buff)-tmp);
        // Reveiller les ecrivains qui attendent pour ecrire (s'ils existent)
        pthread_cond_broadcast(c->cond_ecrivain);
        return count;
      }
    }
    // si le nombre d'octets a extraire du buffer est superieur a la taille de la chaine
    else if(count > strlen(c->buff)){
      // calculer le nombre d'octets restans dans le buffer du conduit
      int n=strlen(c->buff)-c->curseur_lecture;
      // copier les octets lus dans le buffer du lecteur
      buff=memcpy(buff,c->buff+c->curseur_lecture,n);
      printf("j'ai lu %s\n",buff);
      printf("n %d\n",n );
      if(buff==NULL){
        printf("Conduct reading failed\n");
        return -1;
      }
      // effacer ce que le lecteur a lu
      memmove(c->buff+c->curseur_lecture, c->buff+n,n);
      printf("buffer conduit %s\n",c->buff);
      printf("taille buffer conduit %ld\n",strlen(c->buff));

      lu=strlen(buff);
      // Reveiller les ecrivains qui attendent pour ecrire (s'ils existent)
      pthread_cond_broadcast(c->cond_ecrivain);
      return lu;
    }
  }
  // Relacher le verrou
  pthread_mutex_unlock(c->verrou_buff);
  return 0;
}


int conduct_write_eof(struct conduct *c){
  // mettre la variable eof à 1 (pour dire que la marque de fin de fichier est insérée)
  c->eof=1;
  // Reveiller les lecteurs qui bloquent (buffer vide + pas de marque de fin de fichier)
  pthread_cond_broadcast(c->cond_lecteur);
  return 0;
}

void conduct_close(struct conduct *c){
  //liberer la structure sans detruire le conduit
  int res=munmap(c,sizeof(struct conduct));
  if(res==-1){
    perror("Deleting failed: ");
    exit(3);
  }
}

void conduct_destroy(struct conduct *c){
  // Detruire le conduit
  int res=unlink(c->name);
  conduct_close(c);
  if(res==-1){
    perror("File deleting failed : ");
    exit(4);
  }
}

/*Fonction d'écriture dans le conduit */
ssize_t conduct_write(struct conduct *c, const void *buf, size_t count){
    ssize_t octets_ecrits;


    pthread_mutex_lock(c->verrou_buff);
    printf("here\n");
    /*Conduit plein*/
    if(strlen(c->buff)==c->capacity){
      printf("here\n");

        /*Je reveille les lecteurs pour qu'ils me libèrent de la place */
        if(pthread_cond_broadcast(c->cond_lecteur)==-1){
          perror("Broadcast doesn't work");
          return -1;
        };
        if(pthread_cond_wait(c->cond_ecrivain,c->verrou_buff)==-1){
          perror("Wait doesn't work");
          return -1;
        };

        /*Les Lecteurs m'ont réveillé , il y a de la place dispo */
        /*Ecriture atomique*/
        if(count<=c->atomicity){
            while(count>c->capacity-strlen(c->buff)){
                if(pthread_cond_wait(c->cond_ecrivain,c->verrou_buff)==-1){
                  perror("Wait doesn't work");
                  return -1;
                };
              }
              memcpy(c->buff+strlen(c->buff), buf, count);
              octets_ecrits=(ssize_t) count;
              return octets_ecrits;
            }

        /*Ecriture non atomique*/
          memcpy(c->buff+strlen(c->buff), buf,c->capacity-strlen(c->buff));
          octets_ecrits=(ssize_t)c->capacity-strlen(c->buff);
          return octets_ecrits;
    }

    /*Il y a de la place */

    /*Ecriture atomique*/
    printf("here\n");

    if(count<=c->atomicity){
      printf("here place ecriture\n");
      while(count>c->capacity-strlen(c->buff)){
          if(pthread_cond_wait(c->cond_ecrivain,c->verrou_buff)==-1){
              perror("Wait doesn't work");
              return -1;
            }
          }
          memcpy(c->buff+strlen(c->buff), buf, count);
          octets_ecrits=(ssize_t) count;
          return octets_ecrits;
        }

    /*Ecriture non atomique*/
      octets_ecrits=c->capacity-strlen(c->buff);
      memcpy(c->buff+strlen(c->buff), buf,c->capacity-strlen(c->buff));
      return octets_ecrits;
  }
