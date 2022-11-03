# Processes-Threads-in-UNIX

## 1.1. Procesos, threads y Comunicacióon

## 1. Planta de Reciclado. Considere una planta que gestiona el reciclado de residuos.
La planta cuenta con recolectores que empaquetan y envían lo que colectan al sector de clasificaci´on de residuos. En este punto, se identifica cada paquete y se derivan a los distintos puntos donde los recicladores los procesan. Los residuos tratados pueden ser vidrio, cart´on, pl´astico o aluminio. Asuma 3 recolectores que empaquetan cualquiera de los 4 tipos de residuos, 2 clasificadores que reciben paquetes de cualquier tipo y envían a los recicladores correspondientes. De estos últimos funcionan uno por tipo. Si el reciclador no tiene nada que reciclar puede optar por colaborar con otro reciclador, o detenerse a tomar mate.

** a) Se resuelve el problema considerando que la forma de comunicaci´on entre los componentes (procesos) es con pipes.
** b) Se resuelve el problema considerando que la forma de comunicaci´on entre los componentes (procesos) es con colas de mensajes.

## 1.2. Sincronizacióon
* 1. Secuencia. Dada la siguiente secuencia:
AB(DoE)AC(DoE)FAB(DoE)AC(DoE)FAB(DoE)AC(DoE)F

** a) Resuelva el problema utilizando hilos (threads) y semáforos para su sincronización. Recuerde hacer uso eficiente de los recursos como por ejemplo la cantidad de semáforos que utiliza.

** b) Implemente el mismo problema utilizando Procesos y pipes para su sincronización.
