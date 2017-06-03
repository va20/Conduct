#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <stddef.h>
#include <pthread.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/un.h>


/*Structure Conduit */
struct conduct{
    size_t atomicity; // n > a :pas de garatie qu'on va ecrire n octets parce quéon a demandé plus de a
                    // n <= a  : ecriture avec garantie que l'ecriture soit contigue
    size_t capacity;// borne maximale du buff
    int eof;
    pthread_mutex_t verrou_buff;
    pthread_cond_t cond_ecrivain;
    pthread_cond_t cond_lecteur;
    size_t taille_buff;
    sem_t lecture;
    sem_t ecriture;
    char name[256];
};

struct conduct *conduct_create(const char *name, size_t a, size_t c);
struct conduct *conduct_open(const char *name);
ssize_t conduct_read(struct conduct *c, void *buf, size_t count);
ssize_t conduct_write(struct conduct *c, const void *buf, size_t count);
int conduct_write_eof(struct conduct *c);
void conduct_close(struct conduct *conduct);
void conduct_destroy(struct conduct *conduct);
