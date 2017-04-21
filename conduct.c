#include "conduct.h"


struct conduct *conduct_create(const char *name,size_t a,size_t c){
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
        //conduit->buff[conduit->capacity];
        conduit->buff=malloc(c*sizeof(void));
        memset(conduit->buff,0,sizeof(char)*conduit->capacity);
        conduit->name=malloc(strlen(name)*sizeof(char));
        if(strcpy(conduit->name, name)==NULL){
          printf("Error copy name\n");
          exit(6);
        }

		    conduit->eof=0;
        conduit->curseur_ecriture=0;
        conduit->curseur_lecture=0;
        conduit->taille_buff=0;

        pthread_mutex_t verrou;
        pthread_mutex_init(&verrou, NULL);
        conduit->verrou_buff=&verrou;

        pthread_cond_t cond_ecr;
	      pthread_cond_init(&cond_ecr,NULL);
	      pthread_cond_t cond_lec;
	      pthread_cond_init(&cond_lec,NULL);

        conduit->cond_ecrivain=&cond_ecr;
        conduit->cond_lecteur=&cond_lec;
        return conduit;
    }

    conduit=mmap(NULL,sizeof(struct conduct),PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1, 0);
    if(conduit==MAP_FAILED){
      perror("Mapping failed");
      exit(1);
    }
    conduit->capacity=c;
    conduit->atomicity=a;
    conduit->buff=malloc(c*sizeof(void));
    //conduit->buff[conduit->capacity];
    memset(conduit->buff,0,sizeof(void)*conduit->capacity);

		conduit->eof=0;
    conduit->curseur_ecriture=0;
    conduit->curseur_lecture=0;
    conduit->taille_buff=0;

    //pthread_mutex_t verrou;
    conduit->verrou_buff=malloc(sizeof(pthread_mutex_t));

    conduit->cond_ecrivain=malloc(sizeof(pthread_cond_t));
    conduit->cond_lecteur=malloc(sizeof(pthread_cond_t));

    pthread_mutex_init(conduit->verrou_buff, NULL);
	  pthread_cond_init(conduit->cond_ecrivain,NULL);
	  pthread_cond_init(conduit->cond_lecteur,NULL);


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
  pthread_mutex_lock(c->verrou_buff);
  ssize_t lu;

  //si le buffer est vide et sans marque de fin de fichier -> la fonction bloque jusqu'à ce que:
  // 1) le buffer ne soit plus vide
  // 2) une marque de fin de fichier soit y insérée
  while(c->taille_buff==0 && c->eof==0){
    // Reveiller les ecrivains qui attendent pour ecrire (s'ils existent)
    //pthread_cond_broadcast(c->cond_ecrivain);
    // Bloquer jusqu'à ce que : le buffer ne soit plus vide ou une marque de fin de fichier soit y insérée
    pthread_cond_wait(c->cond_ecrivain,c->verrou_buff);
  }
  // si le buffer n'est pas vide

  if(c->taille_buff>0){
    // si le nombre d'octets a exraire est inferieur à la taille de la chaine dans le buffer :
    if(count<=c->taille_buff){
    // 1) si le cursuer de la lecture n'est pas encore arriver à la fin du buffer
    //copier les octets demandes par le lecteur dans buff
      memcpy(buff,c->buff+c->curseur_lecture,count);
      // effacer les octets lus du buffer de conduit
      memmove(c->buff+c->curseur_lecture, c->buff+count, c->taille_buff);

      c->taille_buff=c->taille_buff-count;


      //buff[strlen(buff)-1]='\0';
      if(buff==NULL){
        printf("Conduct reading failed\n");
        //pthread_mutex_unlock(c->verrou_buff);
        return -1;
      }
      //c->curseur_lecture=c->curseur_lecture+count;
      lu=count;
      // reveiller les ecrivains pour qu'ils puissent ecrire (s'il y a des ecrivains qui attendent)
      pthread_cond_broadcast(c->cond_lecteur);
      pthread_mutex_unlock(c->verrou_buff);

      return lu;
    }// 2) si le curseur de la lecture va depasser la capacity(taille du buffer) -> faire 2 lectures
    // si le nombre d'octets a extraire du buffer est superieur a la taille de la chaine
    else if(count > c->taille_buff){
      // calculer le nombre d'octets restans dans le buffer du conduit
      // copier les octets lus dans le buffer du lecteur
      memcpy(buff,c->buff+c->curseur_lecture,c->taille_buff);

      memmove(c->buff+c->curseur_lecture, c->buff+c->taille_buff,c->taille_buff);
    //  printf("taille buffer conduit %ld\n",strlen(c->buff));

      lu=c->taille_buff;
      c->taille_buff=0;

      printf("j'ai lu %s\n",(char*)buff);
      if(buff==NULL){
        printf("Conduct reading failed\n");
        pthread_mutex_unlock(c->verrou_buff);

        return -1;
      }
      // Reveiller les ecrivains qui attendent pour ecrire (s'ils existent)
      pthread_cond_broadcast(c->cond_lecteur);
      pthread_mutex_unlock(c->verrou_buff);

      return lu;
    }
  }
  // Relacher le verrou
  //
  pthread_cond_broadcast(c->cond_lecteur);
  pthread_mutex_unlock(c->verrou_buff);
  return 0;
}


int conduct_write_eof(struct conduct *c){
  // mettre la variable eof à 1 (pour dire que la marque de fin de fichier est insérée)
  pthread_mutex_lock(c->verrou_buff);

  c->eof=1;
  // Reveiller les lecteurs qui bloquent (buffer vide + pas de marque de fin de fichier)
  pthread_cond_broadcast(c->cond_ecrivain);
  pthread_mutex_unlock(c->verrou_buff);

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
  pthread_mutex_lock(c->verrou_buff);
  ssize_t octets_ecrits;


  if(c->eof==1){
    perror("Conduct has eof");
    pthread_mutex_unlock(c->verrou_buff);
    return -1;
  }
    /*Conduit plein*/
  if(c->taille_buff==c->capacity){
        /*Je reveille les lecteurs pour qu'ils me libèrent de la place */
        if(pthread_cond_broadcast(c->cond_ecrivain)==-1){
          perror("Broadcast doesn't work");
          return -1;
        }
    if(pthread_cond_wait(c->cond_lecteur,c->verrou_buff)==-1){
      perror("Wait doesn't work");
      return -1;
    }
  }
        /*Les Lecteurs m'ont réveillé , il y a de la place dispo */
        /*Ecriture atomique*/
    if(count<=c->atomicity){
      while(count>c->capacity-c->taille_buff){
        pthread_cond_broadcast(c->cond_ecrivain);
        if(pthread_cond_wait(c->cond_lecteur,c->verrou_buff)==-1){
          perror("Wait doesn't work");
          return -1;
        }
      }
      memcpy(c->buff+c->taille_buff, buf, count);
      if(c->taille_buff+count<=c->capacity){
        c->taille_buff+=count;
      }

      octets_ecrits=(ssize_t) count;
      pthread_cond_broadcast(c->cond_ecrivain);
      pthread_mutex_unlock(c->verrou_buff);
      return octets_ecrits;
    }
      //ajouter une condition pour tester si l'ecrivain souhaite ecrire d'une maniere atomique
      /*Ecriture non atomique*/
    memcpy(c->buff+c->taille_buff, buf,c->capacity-c->taille_buff);
    octets_ecrits=(ssize_t)c->capacity-c->taille_buff;
    c->taille_buff+=octets_ecrits;
    pthread_cond_broadcast(c->cond_ecrivain);
    pthread_mutex_unlock(c->verrou_buff);
    return octets_ecrits;
  }
