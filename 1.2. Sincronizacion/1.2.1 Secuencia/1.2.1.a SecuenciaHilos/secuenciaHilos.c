#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semA, semB, semC, semDE, semF;

void * letraA(){
	while(1){
		sem_wait(&semA);
		sem_wait(&semA);
		printf("A ");
		sem_post(&semB);
		sem_post(&semC);
	}
	
}

void * letraB(){
	while(1){
		sem_wait(&semB);
		sem_wait(&semB);
		printf("B ");
		sem_post(&semDE);
		sem_post(&semA);
	}
}

void * letraC(){
	while(1){
		sem_wait(&semC);
		sem_wait(&semC);
		printf("C ");
		sem_post(&semDE);
	}
}

void * letraD(){
	while(1){
		sem_wait(&semDE);
		printf("D ");
		sem_post(&semA);
		sem_post(&semF);
	}
}

void * letraE(){
	while(1){
		sem_wait(&semDE);	
		printf("E ");
		sem_post(&semA);
		sem_post(&semF);
	}
}

void * letraF(){
	while(1){
		sem_wait(&semF);
		sem_wait(&semF);
		printf("F ");
		sem_post(&semA);
	}
}

int main(){
	
	printf("inicializa los semaforos\n");
	sem_init(&semA,1,2);
	sem_init(&semB,1,1);
	sem_init(&semC,1,0);
	sem_init(&semDE,1,0);
	sem_init(&semF,1,0);
	
	pthread_t thread[6];
	printf("Creando los semaforos\n");
	pthread_create(&(thread[0]),NULL,(void*)letraA,NULL);
	pthread_create(&(thread[1]),NULL,(void*)letraB,NULL);
	pthread_create(&(thread[2]),NULL,(void*)letraC,NULL);
	pthread_create(&(thread[3]),NULL,(void*)letraD,NULL);
	pthread_create(&(thread[4]),NULL,(void*)letraE,NULL);
	pthread_create(&(thread[5]),NULL,(void*)letraF,NULL);
	
	for(int i=0; i< 6; i++){
		pthread_join(thread[i],NULL);
	}

}
