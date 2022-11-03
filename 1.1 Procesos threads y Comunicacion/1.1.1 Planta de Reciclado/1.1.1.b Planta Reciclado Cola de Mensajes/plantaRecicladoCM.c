#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>


//Archivo UNIX necesario para la generacion de la clave
#define ARCH_CLAVE "/etc"

//Establezco los numeros para cada tipo de mensaje
#define TR 6
#define TRV 2
#define TRC 3
#define TRP 4
#define TRA 5

typedef struct{
    long mtype;    /* message type, must be > 0 */
	int num; /* message data */
} mensaje;

key_t generateKey(char c){
	key_t clave_cola=ftok(ARCH_CLAVE,c);
	return clave_cola;
}

int longitud = sizeof(mensaje) - sizeof(long);

void recolector();
void clasificador();
void recicladorVidrio();
void recicladorCarton();
void recicladorPlastico();
void recicladorAluminio();
void tomarMate();
void error();

int main(){
	pid_t pid;
	int colaId;
	
	/*Recolector 1*/
	pid = fork();
	if((int) pid == -1){
		error();
	}
	if(pid == 0){
		recolector();
		exit(0);		
	}
	/*Recolector 2*/
	pid = fork();
	if((int) pid == -1){
		error();
	}
	if(pid == 0){
		recolector();
		exit(0);
	}
	
	/*Recolector 3*/
	pid = fork();
	if((int) pid == -1){
		error();
	}
	if(pid == 0){
		recolector();
		exit(0);
	}
	
	/* Proceso Clasificador 1 */
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		clasificador();
		exit(0);
	}
	
	/* Proceso Clasificador 2 */
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		clasificador();
		exit(0);
	}
	
	/* Proceso Recilador Vidrio */
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		recicladorVidrio();
		exit(0);
	}
	
	/* Proceso Recilador Carton */
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		recicladorCarton();
		exit(0);
	}
	
	/* Proceso Recilador Plastico */
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		recicladorPlastico();
		exit(0);
	}
	
	/* Proceso Recilador Aluminio */
	pid = fork();
	if ((int) pid == -1) {
			error();
	}
	if (pid==0) {
		recicladorAluminio();
		exit(0);
	}
	
	/*Elimino la cola de mensajes previa, si es que existe*/
	colaId = msgget(generateKey('a'), IPC_CREAT | 0666);//Recupero la cola anterior
	msgctl(colaId,IPC_RMID,NULL); //Elimino la cola anterior
	
	colaId = msgget(generateKey('a'), IPC_CREAT | 0666);
	
	int i;
	for(i=0; i<10; i++)
		wait(NULL);
	
	return 0;
}
		
void recolector(){
	int paquete; 
	mensaje * msg = (mensaje *) malloc(sizeof(mensaje));
	int cola = msgget(generateKey('a'), IPC_CREAT | 0666);
	while(1){
		paquete = rand() % 4;
		msg->mtype = TR;
		msg->num = paquete;
		printf("Soy recolector y empaquete un msg con num %d \n",msg->num);
		msgsnd(cola, msg, longitud, 0);	
	}
	
	free(msg);
}

void clasificador(){
	int paquete;
	mensaje * msg = (mensaje *) malloc(sizeof(mensaje));
	int cola = msgget(generateKey('a'), IPC_CREAT | 0666);
	while(1){
		msgrcv(cola, msg,longitud,TR,0);
		printf("Soy clasificador y recibi un material %d\n", msg->num);
		paquete = msg->num;	/* 0:vidrio, 1:carton, 2:plastico y 3:aluminio*/	
		switch(paquete){
			case 0:
				printf("Se clasifico un vidrio\n");
				msg->mtype = TRV;
				msgsnd(cola,msg,longitud,0);
				break;
			case 1:	
				printf("Se clasifico un carton\n");
				msg->mtype = TRC;
				msgsnd(cola,msg,longitud,0);
				break;
			case 2:
				printf("Se clasifico un plastico\n");
				msg->mtype = TRP;
				msgsnd(cola,msg,longitud,0);
				break;
			case 3:
				printf("Se clasifico un aluminio\n");
				msg->mtype = TRA;
				msgsnd(cola,msg,longitud,0);
				break;
			default:
				printf("No se pudo clasificar el residuo\n");
				fflush(stdout);
		}
	}
	free(msg);	
}

void recicladorVidrio(){
int elemento, paquete;
	int proximaTarea;
	mensaje * msg = (mensaje *) malloc(sizeof(mensaje));
	int cola = msgget(generateKey('a'), IPC_CREAT | 0666);
	while(1){
		paquete = msgrcv(cola, msg, longitud, TRV,IPC_NOWAIT);
		if(paquete == -1){ //BUSCO EL MENSAJE de mi tipo 
			printf("Soy reciclador de vidrio, no encontre mensajes de mi tipo en la cola \n");
			proximaTarea = rand() % 2; /*Numero random entre 0 y 1*/
			switch(proximaTarea){
				case 0: 
					tomarMate();
					break;
				case 1:
					paquete = msgrcv(cola,msg,longitud,-5,IPC_NOWAIT); //Recibo de cualquier cola
					if (paquete == -1){
						printf("/////////Soy reciclador de vidrio, NO PUEDO AYUDAR/////////\n");	
						
					}
					else{
						printf("*******************Soy reciclador de vidrio, estoy ayudando y recibi un material %d*********\n", msg->num);	
					}
					break;
				default:
					printf("No hago nada\n");
				}
		} 
		else{
			printf("===========*=====*Soy reciclador de vidrio, recibi un vidrio y lo reciclo*===========*=====\n");
		}
	}
	free(msg);
}
void recicladorCarton(){
int elemento, paquete;
	int proximaTarea;
	mensaje * msg = (mensaje *) malloc(sizeof(mensaje));
	int cola = msgget(generateKey('a'), IPC_CREAT | 0666);
	while(1){
		paquete = msgrcv(cola, msg, longitud, TRC,IPC_NOWAIT);
		if(paquete == -1){ //BUSCO EL MENSAJE de mi tipo 
			printf("Soy reciclador de carton, no encontre mensajes de mi tipo en la cola \n");
			proximaTarea = rand() % 2; /*Numero random entre 0 y 1*/
			switch(proximaTarea){
				case 0: 
					tomarMate();
					break;
				case 1:
					paquete = msgrcv(cola,msg,longitud,-5,IPC_NOWAIT); //Recibo de cualquier cola
					if (paquete == -1){
						printf("/////////Soy reciclador de carton, NO PUEDO AYUDAR/////////\n");	
						
					}
					else{
						printf("*******************Soy reciclador de carton, estoy ayudando y recibi un material %d*********\n", msg->num);	
					}
					break;
				default:
					printf("No hago nada\n");
				}
		} 
		else{
			printf("===========*=====*Soy reciclador de carton, recibi un carton y lo reciclo*===========*=====\n");
		}
	}
	free(msg);
	
	
}
void recicladorPlastico(){
int elemento, paquete;
	int proximaTarea;
	mensaje * msg = (mensaje *) malloc(sizeof(mensaje));
	int cola = msgget(generateKey('a'), IPC_CREAT | 0666);
	while(1){
		paquete = msgrcv(cola, msg, longitud, TRP,IPC_NOWAIT);
		if(paquete == -1){ //BUSCO EL MENSAJE de mi tipo 
			printf("Soy reciclador de plastico, no encontre mensajes de mi tipo en la cola \n");
			proximaTarea = rand() % 2; /*Numero random entre 0 y 1*/
			switch(proximaTarea){
				case 0: 
					tomarMate();
					break;
				case 1:
					paquete = msgrcv(cola,msg,longitud,-5,IPC_NOWAIT); //Recibo de cualquier cola
					if (paquete == -1){
						printf("/////////Soy reciclador de plastico, NO PUEDO AYUDAR/////////\n");	
						
					}
					else{
						printf("*******************Soy reciclador de plastico, estoy ayudando y recibi un material %d*********\n", msg->num);	
					}
					break;
				default:
					printf("No hago nada\n");
				}
		} 
		else{
			printf("===========*=====*Soy reciclador de plastico, recibi un plastico y lo reciclo*===========*=====\n");
		}
	}
	free(msg);
}
	
void recicladorAluminio(){
	int elemento, paquete;
	int proximaTarea;
	mensaje * msg = (mensaje *) malloc(sizeof(mensaje));
	int cola = msgget(generateKey('a'), IPC_CREAT | 0666);
	while(1){
		paquete = msgrcv(cola, msg, longitud, TRA,IPC_NOWAIT);
		if(paquete == -1){ //BUSCO EL MENSAJE de mi tipo 
			printf("Soy reciclador de aluminio, no encontre mensajes de mi tipo en la cola \n");
			proximaTarea = rand() % 2; /*Numero random entre 0 y 1*/
			switch(proximaTarea){
				case 0:
					tomarMate();
					break;
				case 1:
					paquete = msgrcv(cola,msg,longitud,-5,IPC_NOWAIT); //Recibo de cualquier cola
					if (paquete == -1){
						printf("/////////Soy reciclador de aluminio, NO PUEDO AYUDAR/////////\n");	
						
					}
					else{
						printf("*******************Soy reciclador de aluminio, estoy ayudando y recibi un material %d*********\n", msg->num);	
					}
					break;
				default:
					printf("No hago nada\n");
				}
		} 
		else{
			printf("===========*=====*Soy reciclador de aluminio, recibi un aluminio y lo reciclo*===========*=====\n");
		}
	}
	free(msg);
}

void tomarMate(){
	printf("Tomando mate...\n");
	sleep(1);
}

void error(){
	printf("Error al crear el proceso\n");
	exit(1);
}
