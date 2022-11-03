#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/msg.h>
#include "semcolamen.h"

#include <errno.h>

#define KEY1 ((key_t)(11111))

#define PUEDENORTE			1		/**< @brief tipo de mensaje para emular un semaforo puedeNorte */
#define PUEDESUR			2		/**< @brief tipo de mensaje para emular un semaforo puedeSur */
#define CANTNORTE			3		/**< @brief tipo de mensaje para emular un semaforo cantNorte */
#define CANTSUR				4		/**< @brief tipo de mensaje para emular un semaforo cantSur */
#define MUTEX				5		/**< @brief tipo de mensaje para emular un semaforo mutex */
#define SEMCANTPUEDEPASAR	6		/**< @brief tipo de mensaje para emular un semaforo semCantPuedePasar */

#define CANTPUEDEPASAR	5			/**< @brief cantidad de autos que pueden pasar de un lado hasta que se de prioridad a los del otro lado */

/**
 * @brief procedimiento que define el comportamiento de los autos que vienen del norte
 * @param colaMensajes el id de la cola de mensajes a ser utilizada para la sincronizacion
 */
void autoNorte(int colaMensajes);

/**
 * @brief procedimiento que define el comportamiento de los autos que vienen del sur
 * @param colaMensajes el id de la cola de mensajes a ser utilizada para la sincronizacion
 */
void autoSur(int colaMensajes);

/**
 * @brief Consume todos los mensajes de tipo SEMCANTPUEDEPASAR y vuelve a insertar una cantidad fija de ellos
 * @param colaMensajes la cola de mensajes
 */
void resetCantPuedenPasar(int colaMensajes);

/**
 * @brief Imprime por pantalla que auto esta cruzando el puente
 * @param c caracter que representa que auto esta cruzando el puente
 */
void cruzarPuente(char c);

int main(int argc, char **argv) {
    int IDcolaMensajes;
	mess_t msg;
    msg.dato = 1;
    
	IDcolaMensajes = msgget(KEY1, 0666);  				// Recupero la cola anterior
	msgctl(IDcolaMensajes, IPC_RMID, NULL); 			// Elimino la cola anterior
	sleep(0.5);
	printf("cola anterior removida\n");

	IDcolaMensajes = msgget(KEY1, IPC_CREAT | 0666);	// creo la cola
	if (IDcolaMensajes == -1) {							// si hay errores, imprimo y termino
		printf(	"Error %i en la creacion de"
				"la cola de mensajes: %s\n",
				errno, strerror(errno));
		exit(1);
	}
		
	cm_signal(IDcolaMensajes, PUEDENORTE);				//
	cm_signal(IDcolaMensajes, MUTEX);					// Inicializo los "semaforos"
	resetCantPuedenPasar(IDcolaMensajes);				//
	 
	for (int i = 0; i < 10; i++) {						// creo autos que vienen del norte
		if (fork() == 0) {
			autoNorte(IDcolaMensajes);
            exit(0);
        }
	}
	for (int i = 10; i < 20; i++) {						// creo autos que vienen del sur
		if (fork() == 0) {
			autoSur(IDcolaMensajes);
            exit(0);
        }
	}
	
	return 0;
}

void autoNorte(int IDcolaMensajes) {
	int noPuedenPasarMas;
	int nadieQuierePasar;
	sleep(1);
	
	cm_wait(IDcolaMensajes, PUEDENORTE);							// espero para poder pasar
	
	cm_wait(IDcolaMensajes, MUTEX);									// espero la seccion de entrada
	cm_signal(IDcolaMensajes, CANTNORTE);							// aumento la cantidad que se encuentra pasando
	cm_wait(IDcolaMensajes, SEMCANTPUEDEPASAR);						// reduzco la cantidad que pueden pasar
	if (cm_tryWait(IDcolaMensajes, SEMCANTPUEDEPASAR) >= 0) {		// si todavia pueden pasar
		cm_signal(IDcolaMensajes, PUEDENORTE);							// señalizo que pueden pasar
	}
	cm_signal(IDcolaMensajes, MUTEX);								// libero la seccion de entrada

	cruzarPuente('n');

	cm_wait(IDcolaMensajes, MUTEX);									// espero la seccion de salida
	cm_wait(IDcolaMensajes, CANTNORTE);								// reduzco la cantidad que se encuentra pasando
	if (cm_tryWait(IDcolaMensajes, CANTNORTE) < 0) {					// si fue el ultimo
		noPuedenPasarMas =
			(cm_tryWait(IDcolaMensajes, SEMCANTPUEDEPASAR) < 0);		// compruebo si pueden pasar mas
		nadieQuierePasar =
			(cm_tryWait(IDcolaMensajes, PUEDENORTE) >= 0);				// compruebo si alguno ya tomo el semaforo para pasar
		if (noPuedenPasarMas || nadieQuierePasar) {						// si ocurre lo primero, o no lo segundo
			resetCantPuedenPasar(IDcolaMensajes);							// reinicio la cantidad que pueden pasar
			cm_signal(IDcolaMensajes, PUEDESUR);							// señalizo que pueden pasar los del otro lado
		} else {														// si no
			cm_signal(IDcolaMensajes, SEMCANTPUEDEPASAR);					// repongo la cantidad que pueden pasar
		}
	} else {														// si no fue el ultimo
		cm_signal(IDcolaMensajes, CANTNORTE);							// repongo la cantidad que se encuentra pasando
	}
	cm_signal(IDcolaMensajes, MUTEX);								// libero la seccion de salida
}

void autoSur(int IDcolaMensajes) {
	int noPuedenPasarMas;
	int nadieQuierePasar;
	sleep(1);
	
	cm_wait(IDcolaMensajes, PUEDESUR);								// espero para poder pasar
	
	cm_wait(IDcolaMensajes, MUTEX);									// espero la seccion de entrada
	cm_signal(IDcolaMensajes, CANTSUR);								// aumento la cantidad que se encuentra pasando
	cm_wait(IDcolaMensajes, SEMCANTPUEDEPASAR);						// reduzco la cantidad que pueden pasar
	if (cm_tryWait(IDcolaMensajes, SEMCANTPUEDEPASAR) >= 0) {		// si todavia pueden pasar
		cm_signal(IDcolaMensajes, PUEDESUR);							// señalizo que pueden pasar
	}
	cm_signal(IDcolaMensajes, MUTEX);								// libero la seccion de entrada

	cruzarPuente('s');

	cm_wait(IDcolaMensajes, MUTEX);									// espero la seccion de salida
	cm_wait(IDcolaMensajes, CANTSUR);								// reduzco la cantidad que se encuentra pasando
	if (cm_tryWait(IDcolaMensajes, CANTSUR) < 0) {					// si fue el ultimo
		noPuedenPasarMas =
			(cm_tryWait(IDcolaMensajes, SEMCANTPUEDEPASAR) < 0);		// compruebo si pueden pasar mas
		nadieQuierePasar =
			(cm_tryWait(IDcolaMensajes, PUEDESUR) >= 0);				// compruebo si alguno ya tomo el semaforo para pasar
		if (noPuedenPasarMas || nadieQuierePasar) {						// si ocurre lo primero, o no lo segundo
			resetCantPuedenPasar(IDcolaMensajes);							// reinicio la cantidad que pueden pasar
			cm_signal(IDcolaMensajes, PUEDENORTE);							// señalizo que pueden pasar los del otro lado
		} else {														// si no
			cm_signal(IDcolaMensajes, SEMCANTPUEDEPASAR);					// repongo la cantidad que pueden pasar
		}
	} else {														// si no fue el ultimo
		cm_signal(IDcolaMensajes, CANTSUR);								// repongo la cantidad que se encuentra pasando
	}
	cm_signal(IDcolaMensajes, MUTEX);								// libero la seccion de salida
}

void resetCantPuedenPasar(int IDcolaMensajes) {
	cm_clear(IDcolaMensajes, SEMCANTPUEDEPASAR);			// elimino todos los mensjes de tipo SEMCANTPUEDEPASAR
	for (int i = 0; i < CANTPUEDEPASAR; i++) {				// vuelvo a agregar la cantidad correspondiente
		cm_signal(IDcolaMensajes, SEMCANTPUEDEPASAR);
	}
}

void cruzarPuente(char c) {
	if (c == 'n') {
		printf("Puente cruzado desde el norte\n");
	} else {
		printf("Puente cruzado desde el sur\n");
	}
}
