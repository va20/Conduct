#include "conduct.h"

struct conduct{
  size_t atomicity;
  size_t capacity;
  int eof;
  int ernum;
  pthread_mutex_t verrou_buff;
  pthread_cond_t cond_ecrivain;
  pthread_cond_t cond_lecteur;
  size_t taille_buff;
  char name[256];
};

struct conduct *conduct_create(const char *name,size_t a,size_t c){
    struct conduct* conduit=NULL;
    int fc1;
    if(name!=NULL){
        fc1=open(name,O_CREAT|O_RDWR,0666);
        if(fc1==-1){
            perror("Ouverture du fichier a echoue : ");
            return NULL;
        }
        if(ftruncate(fc1,sizeof(struct conduct)+c)==-1){
          perror("ftruncate failed : ");
          return NULL;
        }
        conduit=mmap(NULL,sizeof(struct conduct)+c,PROT_READ|PROT_WRITE,MAP_SHARED,fc1, 0);
        if(conduit==MAP_FAILED){
            perror("The memory mapping of create conduct failed : ");
            return NULL;
        }
        memset(conduit,0,sizeof(struct conduct)+c);
        if(strcpy(conduit->name, name)==NULL){
          printf("Error copy name \n");
          return NULL;
        }
    }
    else{
      conduit=mmap(NULL,sizeof(struct conduct)+c,PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1, 0);
      if(conduit==MAP_FAILED){
        perror("The memory mapping of create conduct failed : ");
        return NULL;
      }
      memset(conduit,0,sizeof(struct conduct)+c);
    }
    conduit->capacity=c;
    conduit->atomicity=a;
	  conduit->eof=0;
    conduit->taille_buff=0;
    pthread_mutexattr_t attr;
    pthread_condattr_t attr_cond;

    pthread_mutexattr_init(&attr);
    pthread_condattr_init(&attr_cond);
    pthread_condattr_setpshared(&attr_cond, PTHREAD_PROCESS_SHARED);
    pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&conduit->verrou_buff, &attr);
    pthread_cond_init(&conduit->cond_ecrivain,&attr_cond);
	  pthread_cond_init(&conduit->cond_lecteur,&attr_cond);
    return conduit;
  }



/*Ouverture d'un conduit nommé */
struct conduct * conduct_open(const char *name){
  struct conduct * conduit;

  struct stat file;
  int fc2=open(name,O_RDWR);
  if(fc2<0){
    perror("File doesn't open : ");
    exit(EXIT_FAILURE);
  }

  if(fstat(fc2,&file)==-1){
    perror("Problem with fstat");
    exit(EXIT_FAILURE);
  }

  conduit=mmap(NULL,file.st_size,PROT_READ|PROT_WRITE,MAP_SHARED,fc2,0);
  if(conduit==MAP_FAILED){
    perror("The memory mapping of open conduct failed : ");
    exit(EXIT_FAILURE);
  }
  return conduit;
}

ssize_t conduct_read(struct conduct *c,void* buff,size_t count){
  if(pthread_mutex_lock(&c->verrou_buff)!=0){
    perror("Mutex doesn't work");
    return -1;
    }

  ssize_t lu;

  //si le buffer est vide et sans marque de fin de fichier -> la fonction bloque jusqu'à ce que:
  // 1) le buffer ne soit plus vide
  // 2) une marque de fin de fichier soit y insérée
  while(c->taille_buff==0 && c->eof==0){
    // Bloquer jusqu'à ce que : le buffer ne soit plus vide ou une marque de fin de fichier soit y insérée
    pthread_cond_wait(&c->cond_ecrivain,&c->verrou_buff);


  }
  // si le buffer n'est pas vide

  if(c->taille_buff>0){
    // si le nombre d'octets a exraire est inferieur à la taille de la chaine dans le buffer :
    if(count<=c->taille_buff){
    // 1) si le cursuer de la lecture n'est pas encore arriver à la fin du buffer
    //copier les octets demandes par le lecteur dans buff
      if(memmove(buff,(void*)c+sizeof(struct conduct),count)==NULL){
        printf("Read failed\n");
        return -1;
      }
      // effacer les octets lus du buffer de conduit
      memmove((void*)c+sizeof(struct conduct),(void*)c+sizeof(struct conduct)+count, c->taille_buff);
      c->taille_buff=c->taille_buff-count;


      if(buff==NULL){
        printf("Conduct reading failed\n");
        //pthread_mutex_unlock(c->verrou_buff);
        return -1;
      }
      lu=count;
      // reveiller les ecrivains pour qu'ils puissent ecrire (s'il y a des ecrivains qui attendent)
      if(pthread_mutex_unlock(&c->verrou_buff)!=0){
        perror("Unlock doesn't work");
        return -1;
      };
      if(pthread_cond_broadcast(&c->cond_lecteur)!=0){
        perror("Broadcast doesn't work");
        return -1;
      };

      return lu;
    }// 2) si le curseur de la lecture va depasser la capacity(taille du buffer) -> faire 2 lectures
    // si le nombre d'octets a extraire du buffer est superieur a la taille de la chaine
    else if(count > c->taille_buff){
      // calculer le nombre d'octets restans dans le buffer du conduit
      // copier les octets lus dans le buffer du lecteur
      memmove(buff,(void*)c+sizeof(struct conduct),c->taille_buff);
      memmove((void*)c+sizeof(struct conduct), (void*)c+sizeof(struct conduct)+c->taille_buff,c->taille_buff);


      lu=c->taille_buff;
      c->taille_buff=0;

      if(buff==NULL){
        printf("Conduct reading failed\n");
        if(pthread_mutex_unlock(&c->verrou_buff)!=0){
          perror("Unlock doesn't work");
          return -1;
        };

        return -1;
      }
      // Reveiller les ecrivains qui attendent pour ecrire (s'ils existent)
      if(pthread_mutex_unlock(&c->verrou_buff)!=0){
        perror("Unlock doesn't work");
        return -1;
      };
      if(pthread_cond_broadcast(&c->cond_lecteur)!=0){
        perror("Broadcast doesn't work");
        return -1;
      };

      return lu;
    }
  }
  // Relacher le verrou
  //
  if(pthread_mutex_unlock(&c->verrou_buff)!=0){
    perror("unlock doesn't work");
    return -1;
  };
  if(pthread_cond_broadcast(&c->cond_lecteur)!=0){
    perror("Wait doesn't work");
    return -1;
  }
  return 0;
}


int conduct_write_eof(struct conduct *c){
  // mettre la variable eof à 1 (pour dire que la marque de fin de fichier est insérée)
  if(pthread_mutex_lock(&c->verrou_buff)!=0){
    perror("Lock doesn't work");
    return -1;
  };

  c->eof=1;
  // Reveiller les lecteurs qui bloquent (buffer vide + pas de marque de fin de fichier)
  if(pthread_mutex_unlock(&c->verrou_buff)!=0){
    perror("Unlock doesn't work");
    return -1;
  };
  if(pthread_cond_broadcast(&c->cond_ecrivain)!=0){
    perror("Broadcast doesn't work");
    return -1;
  };

  return 0;
}

void conduct_close(struct conduct *c){
  //liberer la structure sans detruire le conduit
  int res=munmap(c,sizeof(struct conduct));
  if(res==-1){
    perror("Deleting failed : ");
    exit(EXIT_FAILURE);
  }
}

void conduct_destroy(struct conduct *c){
  // Detruire le conduit
  int res=unlink(c->name);
  conduct_close(c);
  if(res==-1){
    perror("File deleting failed : ");
    exit(EXIT_FAILURE);
  }
}

/*Fonction d'écriture dans le conduit */
ssize_t conduct_write(struct conduct *c, const void *buf, size_t count){
  if(pthread_mutex_lock(&c->verrou_buff)!=0){
    perror("Lock doesn't work");
    return -1;
  };
  int ms=0;
  ssize_t octets_ecrits=0;

  if(c->eof==1){
    errno=EPIPE;
    perror("Conduct has eof");


    if(pthread_mutex_unlock(&c->verrou_buff)!=0){
      perror("Unlock doesn't work");
      return -1;
    };
    return -1;
  }
    /*Conduit plein*/
  if(c->taille_buff==c->capacity){
    if(pthread_cond_wait(&c->cond_lecteur,&c->verrou_buff)!=0){
      perror("Wait doesn't work");
      return -1;
    }
  }
        /*Les Lecteurs m'ont réveillé , il y a de la place dispo */
        /*Ecriture atomique*/
  if(count<=c->atomicity){
    while(count>c->capacity-c->taille_buff){
      //pthread_mutex_unlock(&c->verrou_buff);
      if(pthread_cond_broadcast(&c->cond_ecrivain)!=0){
        perror("Broadcast doesn't work");
        return -1;
      };
      if(pthread_cond_wait(&c->cond_lecteur,&c->verrou_buff)==-1){
        perror("Wait doesn't work");
        return -1;
      }
    }
      memmove((void*)c+sizeof(struct conduct)+c->taille_buff, buf, count);
      if(c->taille_buff+count<=c->capacity){
        c->taille_buff+=count;
      }
      if(strlen(c->name)!=0){
 		     ms=msync(c,sizeof(struct conduct)+c->capacity,MS_SYNC | MS_INVALIDATE);
 	        if(ms<0){
 	           perror("msync failed : ");
 	        }
 	   }
     octets_ecrits=(ssize_t) count;
     pthread_mutex_unlock(&c->verrou_buff);
     pthread_cond_broadcast(&c->cond_ecrivain);
     return octets_ecrits;
	  ms=msync(c,sizeof(struct conduct)+c->capacity,MS_SYNC | MS_INVALIDATE);
      if(ms<0){
        perror("msync failed : ");
      }
      else if(ms==0){
        octets_ecrits=(ssize_t) count;
        if(pthread_mutex_unlock(&c->verrou_buff)!=0){
          perror("Unlock doesn't work");
          return -1;
        };
        if(pthread_cond_broadcast(&c->cond_ecrivain)!=0){
          perror("Broadcast doesn't work");
          return -1;
        };
        return octets_ecrits;
      }
    }
      //ajouter une condition pour tester si l'ecrivain souhaite ecrire d'une maniere atomique
      /*Ecriture non atomique*/
    memmove((void*)c+sizeof(struct conduct)+c->taille_buff, buf,c->capacity-c->taille_buff);
	  /* if(strlen(c->name)!=0){
		     ms=msync(c,sizeof(struct conduct)+c->capacity,MS_SYNC | MS_INVALIDATE);
	        if(ms<0){
	           perror("msync failed : ");
	        }
	   }*/
      octets_ecrits=(ssize_t)c->capacity-c->taille_buff;
      c->taille_buff+=octets_ecrits;
      pthread_mutex_unlock(&c->verrou_buff);
      pthread_cond_broadcast(&c->cond_ecrivain);
      return octets_ecrits;
      if(pthread_mutex_unlock(&c->verrou_buff)!=0){
        perror("unlock doesn't work");
        return -1;
      }
      if(pthread_cond_broadcast(&c->cond_ecrivain)!=0){
        perror("Broadcast doesn't work");
        return -1;
      }
      return octets_ecrits;
    }
