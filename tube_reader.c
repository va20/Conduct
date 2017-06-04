#include "conduct.h"

#define BUFF_SIZE 512

void handler(int sig){
    unlink("tube");
}

int main(int argc, char const *argv[]) {

    struct sigaction sa;
    int rc;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handler;
    sa.sa_flags = 0;
    rc = sigaction(SIGINT, &sa, NULL);
    if(rc < 0) {
        perror("sigaction");
        exit(1);
    }

    int lue=0;
    clock_t start,end;
    double time_used;
    if(access("tube",F_OK)==-1){
        int p=mkfifo("tube",0622);//l'utilisateur a le droit de lire + ecrire, protection en lecture parce que si qqn essaie de lire, on peut plus recuperer les messages (lecture sur un pipe consomme les donnees)
        if(p==-1){
            perror("Erreur creation pipe");
            exit(1);
        }

    }
    char buffer[TAILLE_TEXT];
    while(1){
        int fd=open("tube",O_RDONLY);
        if(fd==-1){
            perror("Erreur ouverture tube");
            exit(2);
        }
        start=clock();
        lue=read(fd,buffer,TAILLE_TEXT);
        end=clock();
        time_used=((double)(end-start))/CLOCKS_PER_SEC;
        printf("Temps utilisé pour lire dans le tube : %f\n",time_used);
        printf("J'ai lu %d\n",lue);
        close(fd);
        memset(buffer, 0, TAILLE_TEXT);
    }
    return 0;
}
