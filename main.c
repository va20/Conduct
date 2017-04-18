#include "conduct.h"
void *test_conduct(void *ptr);
int cmp=0;
pthread_mutex_t verrou;
int main(int argc, char const *argv[]) {
    srand(time(NULL));
    pthread_mutex_init(&verrou, NULL);
    conduct_create("saif", 20, 10);
    //char tab[20]={'\0'};
    pthread_t thread_tab[50];

    for (int i = 0; i < 50; i++) {
    pthread_t th;
    struct conduct * cond=conduct_open("saif");
    int t = pthread_create(&th, NULL, test_conduct, (void *)cond);
    if (t != 0) {
      perror("Erreur lors de la creation th");
      exit(1);
    }
    thread_tab[i] = th;
  }
  for (int j = 0; j < 50; j++) {
    pthread_join(thread_tab[j], NULL);
  }
    return 0;
}
void * test_conduct(void* ptr){
  struct conduct *c=(struct conduct*)ptr;
  pthread_mutex_lock(&verrou);
  printf("hello \n");
  cmp++;
  printf("cmp %d\n",cmp);
  if(cmp%2==0){
    char tab[30];
      printf("dans pair\n");
      int alea=rand()%31;
      int lu=conduct_read(c,tab,alea);
      printf("j'ai lu %s\n",tab);
      printf("nb octets %d\n",lu);
  }
  else if(cmp%2!=0){
    char tab[]="hello world ! de merde je vous ai fait un prog";
      printf("dans impair\n");
      int alea=rand()%45;
      int ecrit=conduct_write(c, tab, alea);
      printf("j'ai ecrit %s\n",tab);
      printf("nb octets ecrit %d\n",ecrit);
  }
  pthread_mutex_unlock(&verrou);
  return NULL;
}
