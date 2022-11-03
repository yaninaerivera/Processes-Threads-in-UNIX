#include <stdio.h>
#include <stdlib.h>
#include <sys/msg.h>
#include "semcolamen.h"

#include <errno.h>

void cm_wait(int IDcolaMensajes, int tipoMensaje) {
	mess_t msg;
    if (msgrcv(IDcolaMensajes, &msg, sizeof(int), tipoMensaje, 0) == -1) {
        printf("Error %i en la lectura del mensaje: %s\n", errno, strerror(errno));
        exit(1);
    }
}

void cm_signal(int IDcolaMensajes, int tipoMensaje) {
	mess_t msg;
    msg.mtype = tipoMensaje;
    msg.dato = 1;
    if (msgsnd(IDcolaMensajes, &msg, sizeof(int), 0) == -1) {
        printf("Error %i en la escritura del mensaje: %s\n", errno, strerror(errno));
        exit(1);
    }
}

int cm_tryWait(int IDcolaMensajes, int tipoMensaje) {
	mess_t msg;
    int respuesta = 0;
    if (msgrcv(IDcolaMensajes, &msg, sizeof(int), tipoMensaje, IPC_NOWAIT) == -1) {
        respuesta = -1;
    }
    return respuesta;
}

void cm_clear(int IDcolaMensajes, int tipoMensaje) {
	mess_t msg;
	int vacio = 0;
	while (!vacio) {
		if (msgrcv(IDcolaMensajes, &msg, sizeof(int), tipoMensaje, IPC_NOWAIT) == -1) {
			vacio = 1;
		}
	}
}
