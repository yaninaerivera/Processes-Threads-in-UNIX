#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t puedeNorte, puedeSur, mutex, cantNorte, cantSur;

void cruzarPuente(char c) {
	if (c == 'n') {
		printf("Puente cruzado desde el norte\n");
	} else {
		printf("Puente cruzado desde el sur\n");
	}
}

void * autoNorte(void * arg);
void * autoSur(void * arg);

int main(int argc, const char* argv[]) {
	
	sem_init(&mutex, 		0, 1);
	sem_init(&puedeNorte,	0, 1);
	sem_init(&cantNorte, 	0, 0);
	sem_init(&puedeSur, 	0, 0);
	sem_init(&cantSur, 		0, 0);
	
	pthread_t hilos[20];
	
	for (int i = 0; i < 10; i++) {
		pthread_create(&hilos[i],NULL,&autoNorte,NULL);
	}
	
	for (int i = 10; i < 20; i++) {
		pthread_create(&hilos[i],NULL,&autoSur,NULL);
	}
	
	for (int i = 0; i < 20; i++) {
		pthread_join(hilos[i], NULL);
	}
	
	return 0;
}

void * autoNorte(void * arg) {
	while (1) {
		sleep(1);
		
		sem_wait(&puedeNorte);
		//sem_wait(&mutex);
		sem_post(&cantNorte);
		sem_post(&puedeNorte);
		//sem_post(&mutex);
		
		cruzarPuente('n');
		
		sem_wait(&mutex);
		sem_wait(&cantNorte);
		if (sem_trywait(&cantNorte) < 0) {
			if (sem_trywait(&puedeNorte) >= 0) {
				sem_post(&puedeSur);
			}
		} else {
			sem_post(&cantNorte);
		}
		sem_post(&mutex);
	}
}

void * autoSur(void * arg) {
	while (1) {
		sleep(1);
		
		sem_wait(&puedeSur);
		//sem_wait(&mutex);
		sem_post(&cantSur);
		sem_post(&puedeSur);
		//sem_post(&mutex);
		
		cruzarPuente('s');
		
		sem_wait(&mutex);
		sem_wait(&cantSur);
		if (sem_trywait(&cantSur) < 0) {
			if (sem_trywait(&puedeSur) >= 0) {
				sem_post(&puedeNorte);
			}
		} else {
			sem_post(&cantSur);
		}
		sem_post(&mutex);
	}
}
