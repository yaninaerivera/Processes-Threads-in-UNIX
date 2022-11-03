#ifndef SEMCOLAMEN_H
#define SEMCOLAMEN_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <string.h>

/**
 * @struct mensaje
 * @var mensaje::mtype el tipo del mensaje
 * @var mensaje::dato el dato del mensaje
 */
typedef struct mensaje {
    long mtype;
    int dato;
} mess_t;

/**
 * @brief Simula el comportamiento de la operacion wait de semaforos usando una cola de mensajes
 * @param IDcolaMensajes el identificador de la cola de mensajes sobre la cual efectuar la operacion
 * @param tipoMensaje el tipo de mensaje sobre el cual efectuar la operacion
 */
void cm_wait(int IDcolaMensajes, int tipoMensaje);

/**
 * @brief Simula el comportamiento de la operacion signal de semaforos usando una cola de mensajes
 * @param IDcolaMensajes el identificador de la cola de mensajes sobre la cual efectuar la operacion
 * @param tipoMensaje el tipo de mensaje sobre el cual efectuar la operacion
 */
void cm_signal(int IDcolaMensajes, int tipoMensaje);

/**
 * @brief Simula el comportamiento de la operacion try_wait de semaforos usando una cola de mensajes
 * @param IDcolaMensajes el identificador de la cola de mensajes sobre la cual efectuar la operacion
 * @param tipoMensaje el tipo de mensaje sobre el cual efectuar la operacion
 * @return 0 si se pudo leer, -1 si no
 */
int cm_tryWait(int IDcolaMensajes, int tipoMensaje);

/**
 * @brief Vacia la cola de mensajes del tipo pasado como parametro
 * @param IDcolaMensajes el identificador de la cola de mensajes sobre la cual efectuar la operacion
 * @param tipoMensaje el tipo de mensaje sobre el cual efectuar la operacion
 */
void cm_clear(int IDcolaMensajes, int tipoMensaje);

#endif // SEMCOLAMEN_H
