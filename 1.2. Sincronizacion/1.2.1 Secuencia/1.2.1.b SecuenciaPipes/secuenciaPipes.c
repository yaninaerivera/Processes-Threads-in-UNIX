#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#define SIZE 1			// Tamaño del buffer

/* Defino las funciones */
void letraA();
void letraB();
void letraC();
void letraD();
void letraE();
void letraF();
void error();

/* Declaro los pipes para cada letra */
int pipeA[2],pipeB[2],pipeC[2],pipeDE[2],pipeF[2];
int buffer;

int main(int argc, char const *argv[]) {
		
	pid_t pid;

	/* Inicializo los pipes */
	if(pipe(pipeA) + pipe(pipeB) + pipe(pipeC) + pipe(pipeDE) + pipe(pipeF) < 0)
		error();

	write(pipeA[1], &buffer, SIZE);
	write(pipeA[1], &buffer, SIZE);
	write(pipeB[1], &buffer, SIZE);

	/* Letra A */
	pid = fork();	
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		letraA();
		exit(0);
	}

	/* Letra B */
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		letraB();
		exit(0);
	}

	/* Letra C */
	pid = fork();
	if ((int) pid == -1) {
			error();		
	}
	if (pid==0) {
		letraC();
		exit(0);
	}	

	/* Letra D */
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		letraD();
		exit(0);
	}

	/* Letra E */
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		letraE();
		exit(0);
	}
	
	/* Letra F */
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		letraF();
		exit(0);
	}
	
	close(pipeA[0]);
	close(pipeA[1]);
	close(pipeB[0]);
	close(pipeB[1]);
	close(pipeC[0]);
	close(pipeC[1]);
	close(pipeDE[0]);
	close(pipeDE[1]);
	close(pipeF[0]);
	close(pipeF[1]);
	

	/* Padre espera a los hijos que terminen */
	int i;
	for(i=0; i<6; i++)
		wait(NULL);

	return 0;
}

/* Letra A */
void letraA(){
	/*Cierro los pipes que no se usa*/
	
	//close(pipeA[0]);
	close(pipeA[1]);
	close(pipeB[0]);
	//close(pipeB[1]);
	close(pipeC[0]);
	//close(pipeC[1]);
	close(pipeDE[0]);
	close(pipeDE[1]);
	close(pipeF[0]);
	close(pipeF[1]);
	
	while(1){
		read(pipeA[0],&buffer,SIZE); 
		read(pipeA[0],&buffer,SIZE); 
		printf("A ");
		fflush(stdout);
		write(pipeB[1],&buffer,SIZE);
		write(pipeC[1],&buffer,SIZE);
	}
}

/* Letra B */
void letraB(){
	/*Cierro los pipes que no se usa*/
	close(pipeA[0]);
	//close(pipeA[1]);
	//close(pipeB[0]);
	close(pipeB[1]);
	close(pipeC[0]);
	close(pipeC[1]);
	close(pipeDE[0]);
	//close(pipeDE[1]);
	close(pipeF[0]);
	close(pipeF[1]);

	while(1){
		read(pipeB[0],&buffer,SIZE); 
		read(pipeB[0],&buffer,SIZE); 
		printf("B ");
		fflush(stdout);
		write(pipeDE[1],&buffer,SIZE);
		write(pipeA[1],&buffer,SIZE); 
	}
}

/* Letra C */
void letraC(){
	/*Cierro los pipes que no se usa*/
	close(pipeA[0]);
	close(pipeA[1]);
	close(pipeB[0]);
	close(pipeB[1]);
	//close(pipeC[0]);
	close(pipeC[1]);
	close(pipeDE[0]);
	//close(pipeDE[1]);
	close(pipeF[0]);
	close(pipeF[1]);

	while(1){
		read(pipeC[0],&buffer,SIZE); 
		read(pipeC[0],&buffer,SIZE); 
		printf("C ");
		fflush(stdout);
		write(pipeDE[1],&buffer,SIZE);
	}
}

/* Letra D */
void letraD(){
	/*Cierro los pipes que no se usa*/
	close(pipeA[0]);
	//close(pipeA[1]);
	close(pipeB[0]);
	close(pipeB[1]);
	close(pipeC[0]);
	close(pipeC[1]);
	//close(pipeDE[0]);
	close(pipeDE[1]);
	close(pipeF[0]);
	//close(pipeF[1]);

	while(1){
		read(pipeDE[0],&buffer,SIZE); 	
		printf("D ");
		fflush(stdout);
		write(pipeA[1],&buffer,SIZE);
		write(pipeF[1],&buffer,SIZE);
	}
}

/* Letra E */
void letraE(){
	/*Cierro los pipes que no se usa*/
	close(pipeA[0]);
	//close(pipeA[1]);
	close(pipeB[0]);
	close(pipeB[1]);
	close(pipeC[0]);
	close(pipeC[1]);
	//close(pipeDE[0]);
	close(pipeDE[1]);
	close(pipeF[0]);
	//close(pipeF[1]);
	
	while(1){
		read(pipeDE[0],&buffer,SIZE); 
		printf("E ");
		fflush(stdout);
		write(pipeA[1],&buffer,SIZE);
		write(pipeF[1],&buffer,SIZE);
	}
}

/* Letra F */
void letraF(){
	/*Cierro los pipes que no se usa*/
	close(pipeA[0]);
	//close(pipeA[1]);
	close(pipeB[0]);
	close(pipeB[1]);
	close(pipeC[0]);
	close(pipeC[1]);
	close(pipeDE[0]);
	close(pipeDE[1]);
	//close(pipeF[0]);
	close(pipeF[1]);

	while(1){
		read(pipeF[0],&buffer,SIZE);
		read(pipeF[0],&buffer,SIZE); 
		printf("F ");
		fflush(stdout);
		write(pipeA[1],&buffer,SIZE);
	}
}


/* Función para devolver un error en caso de que ocurra */
void error(){
    /* Escribe el número del último error y el nombre de dicho error */
    printf("Error: %d (%s)\n", errno, strerror(errno));
    exit(1);
}
