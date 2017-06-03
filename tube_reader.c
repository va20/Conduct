#include "conduct.h"

#define BUFF_SIZE 512

int main(int argc, char const *argv[]) {
    int lue=0;
    if(access("tube",F_OK)==-1){
        int p=mkfifo("tube",0622);//l'utilisateur a le droit de lire + ecrire, protection en lecture parce que si qqn essaie de lire, on peut plus recuperer les messages (lecture sur un pipe consomme les donnees)
        if(p==-1){
            perror("Erreur creation pipe");
            exit(1);
        }

    }
    char buffer[BUFF_SIZE];
    while(1){
        int fd=open("tube",O_RDONLY);
        if(fd==-1){
            perror("Erreur ouverture tube");
            exit(2);
        }
        while((lue=read(fd,buffer,BUFF_SIZE))!=0){
            buffer[lue]=0;
        }
        close(fd);
        memset(buffer, 0, BUFF_SIZE);
    }
    return 0;
}
