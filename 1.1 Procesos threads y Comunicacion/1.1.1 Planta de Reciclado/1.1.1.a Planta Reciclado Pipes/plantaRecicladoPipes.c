#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#define SIZE sizeof(int)

void recolector(int nro);
void clasificador(int nro);
void recicladorVidrio();
void recicladorCarton();
void recicladorPlastico();
void recicladorAluminio();
void error();
void tomarMate();
int ayudarReciclador(char idReciclador, int pipeMaterial[2]);

int pipeEmp[2], pipeClasV[2], pipeClasC[2], pipeClasP[2], pipeClasA[2];

int main(){
	
	pid_t pid;
	
	/* Inicializo los pipes */
	if(pipe(pipeEmp) + pipe(pipeClasV) + pipe(pipeClasC) + pipe(pipeClasP) + pipe(pipeClasA) < 0)
		error();

	/* Proceso Recolector 1 */
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		recolector(1);
		exit(0);
	}

	/* Proceso Recolector 2 */
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		recolector(2);
		exit(0);
	}

	/* Proceso Recolector 3*/
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		recolector(3);
		exit(0);
	}

	/* Proceso Recolector 4 */
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		recolector(4);
		exit(0);
	}

	/* Proceso Clasificador 1 */
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		clasificador(1);
		exit(0);
	}
	
	/* Proceso Clasificador 2 */
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		clasificador(2);
		exit(0);
	}
	/* Proceso Clasificador 3 */
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		clasificador(3);
		exit(0);
	}
	
	/* Proceso Recilador Vidrio */
	if (fcntl(pipeClasV[0], F_SETFL, O_NONBLOCK) < 0)
        exit(2);
	
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		recicladorVidrio();
		exit(0);
	}
	
	/* Proceso Recilador Carton */
	if (fcntl(pipeClasC[0], F_SETFL, O_NONBLOCK) < 0)
        exit(2);
	
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		recicladorCarton();
		exit(0);
	}
	
	/* Proceso Recilador Plastico */
	if (fcntl(pipeClasP[0], F_SETFL, O_NONBLOCK) < 0)
        exit(2);
	
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		recicladorPlastico();
		exit(0);
	}
	
	/* Proceso Recilador Aluminio */
	if (fcntl(pipeClasA[0], F_SETFL, O_NONBLOCK) < 0)
        exit(2);
	
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		recicladorAluminio();
		exit(0);
	}

	close(pipeEmp[0]);
	close(pipeEmp[1]);
	close(pipeClasV[0]);
	close(pipeClasV[1]);
	close(pipeClasC[0]);
	close(pipeClasC[1]);
	close(pipeClasP[0]);
	close(pipeClasP[1]);
	close(pipeClasA[0]);
	close(pipeClasA[1]);
	/* Padre espera a los hijos que terminen */
	int i;
	for(i=0; i<10; i++)
		wait(NULL);

	return 0;	
}	
	
void recolector(int nro){
	int paquete;
	close(pipeEmp[0]);
	//close(pipeEmp[1]);
	close(pipeClasV[0]);
	close(pipeClasV[1]);
	close(pipeClasC[0]);
	close(pipeClasC[1]);
	close(pipeClasP[0]);
	close(pipeClasP[1]);
	close(pipeClasA[0]);
	close(pipeClasA[1]);
	
	while(1){
		paquete = rand() % 4; /*Genera un numero random entre 0 y 3*/
		printf("Soy recolector %d y voy a empaquetar un: %d\n", nro, paquete);
		write(pipeEmp[1], &paquete, SIZE);
		printf("Soy recolector %d y empaquete un: %d\n", nro, paquete);
		fflush(stdout);
		sleep(1);
	}
}

void clasificador(int nro){
	
	int tipo; /* 0:vidrio, 1:carton, 2:plastico y 3:aluminio*/ 
	//close(pipeEmp[0]);
	close(pipeEmp[1]);
	close(pipeClasV[0]);
	//close(pipeClasV[1]);
	close(pipeClasC[0]);
	//close(pipeClasC[1]);
	close(pipeClasP[0]);
	//close(pipeClasP[1]);
	close(pipeClasA[0]);
	//close(pipeClasA[1]);
	
	while(1){
		read(pipeEmp[0],&tipo,SIZE);
		printf("Soy un clasificador %d y recibi un : %d\n",nro, tipo);
		fflush(stdout);
		switch(tipo){
			case 0:
				printf("Se clasifico un vidrio\n");
				fflush(stdout);
				write(pipeClasV[1],&tipo,SIZE);
				break;
			case 1:	
				printf("Se clasifico un carton\n");
				fflush(stdout);
				write(pipeClasC[1],&tipo,SIZE);
				break;
			case 2:
				printf("Se clasifico un plastico\n");
				fflush(stdout);
				write(pipeClasP[1],&tipo,SIZE);
				break;
			case 3:
				printf("Se clasifico un aluminio\n");
				fflush(stdout);
				write(pipeClasA[1],&tipo,SIZE);
				break;
			default:
				printf("No se pudo clasificar el residuo\n");
				fflush(stdout);
		}
	}	
}

void recicladorVidrio(){
	int vidrio, decision;
	int hayAluminio, hayVidrio, hayCarton, hayPlastico;

	close(pipeEmp[0]);
	close(pipeEmp[1]);
	//close(pipeClasV[0]);
	close(pipeClasV[1]);
	//close(pipeClasC[0]);
	close(pipeClasC[1]);
	//close(pipeClasP[0]);
	close(pipeClasP[1]);
	//close(pipeClasA[0]);
	close(pipeClasA[1]);
	
	while(1){
		printf("Soy reciclador de vidrio y trato de recibir vidrio\n");
		hayVidrio= read(pipeClasV[0],&vidrio,SIZE);
		if(hayVidrio == -1){
			printf("Soy reciclador de vidrio y no pude recibir vidrio\n"); 
			
			decision = rand() % 2; /*Genera un numero random entre 0 y 1. Si es 1 ayudo a otro reciclador y si es 0 tomo mate*/
			if(decision){
				hayCarton = ayudarReciclador('V',pipeClasC);
				if(hayCarton == -1){ 
					printf("Soy reciclador de carton y no pude recibir carton\n");
					hayPlastico = ayudarReciclador('V',pipeClasP);
					if(hayPlastico == -1){
						printf("Soy reciclador de carton y no pude recibir plastico\n");
						hayAluminio = ayudarReciclador('V',pipeClasA);
						if (hayAluminio == -1){
							printf("Soy reciclador de carton y no pude recibir aluminio\n");
						}
					}
				}
			}
		else{
			printf("Soy reciclador de plastico y NO PUDE AYUDAR\n"); 
			tomarMate();
			}
		}
		else{
			printf("Soy reciclador de plastico y PUDE RECIBIR plastico\n"); 
		}
	}
	
}

void recicladorCarton(){
	int carton, decision;
	int hayAluminio, hayVidrio, hayCarton, hayPlastico;
	close(pipeEmp[0]);
	close(pipeEmp[1]);
	//close(pipeClasV[0]);
	close(pipeClasV[1]);
	//close(pipeClasC[0]);
	close(pipeClasC[1]);
	//close(pipeClasP[0]);
	close(pipeClasP[1]);
	//close(pipeClasA[0]);
	close(pipeClasA[1]);
	
	while(1){
		printf("Soy reciclador de carton y trato de recibir carton\n");
		hayCarton= read(pipeClasC[0],&carton,SIZE);
		if(hayCarton == -1){
			printf("Soy reciclador de carton y no pude recibir carton\n"); 
			
			decision = rand() % 2; /*Genera un numero random entre 0 y 1. Si es 1 ayudo a otro reciclador y si es 0 tomo mate*/
			if(decision){
				hayVidrio = ayudarReciclador('C',pipeClasV);
				if(hayVidrio == -1){ 
					printf("Soy reciclador de carton y no pude recibir vidrio\n");
					hayCarton = ayudarReciclador('C',pipeClasP);
					if(hayPlastico == -1){
						printf("Soy reciclador de carton y no pude recibir plastico\n");
						hayAluminio = ayudarReciclador('C',pipeClasA);
						if (hayAluminio == -1){
							printf("Soy reciclador de carton y no pude recibir aluminio\n");
						}
					}
				}
			}
		else{
			printf("Soy reciclador de plastico y NO PUDE AYUDAR\n"); 
			tomarMate();
			}
		}
		else{
			printf("Soy reciclador de plastico y PUDE RECIBIR plastico\n"); 
		}
	}
}

void recicladorPlastico(){
	int plastico, decision;
	int hayAluminio, hayVidrio, hayCarton, hayPlastico;
	close(pipeEmp[0]);
	close(pipeEmp[1]);
	//close(pipeClasV[0]);
	close(pipeClasV[1]);
	//close(pipeClasC[0]);
	close(pipeClasC[1]);
	//close(pipeClasP[0]);
	close(pipeClasP[1]);
	//close(pipeClasA[0]);
	close(pipeClasA[1]);
	
	while(1){
		
		printf("Soy reciclador de plastico y trato de recibir plastico\n");
		hayPlastico= read(pipeClasP[0],&plastico,SIZE);
		if(hayPlastico == -1){
			printf("Soy reciclador de plastico y no pude recibir plastico\n"); 
			
			decision = rand() % 2; /*Genera un numero random entre 0 y 1. Si es 1 ayudo a otro reciclador y si es 0 tomo mate*/
			if(decision){
				hayVidrio = ayudarReciclador('P',pipeClasV);
				if(hayVidrio == -1){ 
					printf("Soy reciclador de plastico y no pude recibir vidrio\n");
					hayCarton = ayudarReciclador('P',pipeClasC);
					if(hayCarton == -1){
						printf("Soy reciclador de plastico y no pude recibir carton\n");
						hayAluminio = ayudarReciclador('P',pipeClasA);
						if (hayAluminio == -1){
							printf("Soy reciclador de plastico y no pude recibir aluminio\n");
						}
					}
				}
			}
		else{
			printf("Soy reciclador de plastico y NO PUDE AYUDAR\n"); 
			tomarMate();
			}
		}
		else{
			printf("Soy reciclador de plastico y PUDE RECIBIR plastico\n"); 
		}
	}
}

void recicladorAluminio(){
	int aluminio, decision;
	int hayAluminio, hayVidrio, hayCarton, hayPlastico;
	close(pipeEmp[0]);
	close(pipeEmp[1]);
	//close(pipeClasV[0]);
	close(pipeClasV[1]);
	//close(pipeClasC[0]);
	close(pipeClasC[1]);
	//close(pipeClasP[0]);
	close(pipeClasP[1]);
	//close(pipeClasA[0]);
	close(pipeClasA[1]);
	
	while(1){
		printf("Soy reciclador de aluminio y trato de recibir aluminio\n");
		hayAluminio = read(pipeClasA[0],&aluminio,SIZE);
		if(hayAluminio == -1){
			printf("Soy reciclador de aluminio y no pude recibir aluminio\n"); 
			
			decision = rand() % 2; /*Genera un numero random entre 0 y 1. Si es 1 ayudo a otro reciclador y si es 0 tomo mate*/
			
			if(decision){
				hayVidrio = ayudarReciclador('A',pipeClasV);
				if(hayVidrio == -1){ 
					printf("Soy reciclador de aluminio y no pude recibir vidrio\n");
					hayCarton = ayudarReciclador('A',pipeClasC);
					if(hayCarton == -1){
						printf("Soy reciclador de aluminio y no pude recibir carton\n");
						hayPlastico = ayudarReciclador('A',pipeClasP);
						if (hayPlastico == -1){
							printf("Soy reciclador de aluminio y no pude recibir plastico\n");
						}
					}
				}
			}
			else{
				printf("Soy reciclador de aluminio y NO PUDE ayudar\n");
				tomarMate();
			}
			
		}
		else{
			printf("Soy reciclador de aluminio y PUDE RECIBIR aluminio\n"); 
		}
	}
}

void error(){
	/* Escribe el número del último error y el nombre de dicho error */
    printf("Error: %d (%s)\n", errno, strerror(errno));
    exit(1);
}

void tomarMate(){
	printf("Tomando mate\n");
	sleep(1);	
}


	
int ayudarReciclador(char idReciclador, int pipeMaterial[2]){
	int material, nread;
	 
    while (1) {
        nread = read(pipeMaterial[0],&material,SIZE);
        switch (nread) {
        case -1:
            if (errno == EAGAIN) {
                break;
            }
 
            else {
                perror("read");
                break;
            }
        case 0:
             break;
        default:
			printf("===========*=====*=======*====\n"); 
			printf("Soy reciclador con id %c , estoy ayudando y recibi un material %d\n", idReciclador, material);  
			printf("===========*=====*=======*====\n"); 
        }
    }
    
    return nread;
		
}
	
