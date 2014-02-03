/**
 * @file libusbdmanager.h
 * @author Haniel Caceres N.
 *
 */
 
#ifndef STOREDEVICE_H_
#define STOREDEVICE_H_

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#define TABLE "devices"
#define IDNAME "id_device"
#define SERIALNAME "serial_device"
#define USBMVERSION "1.0"
#define DB "/etc/usb-control/database.db"

/**
 * @brief Estructura utilizada para establecer la coneccion con la 
 * base de datos
 * */

typedef struct {
	int rc;
	sqlite3* db;	
} dbase;

/**
 * @brief Estructura utilizada para almacenar en los datos de un 
 * dispositivo
 * */

typedef struct {
	const char* devidname;
	const char* devserialname;	
} device;

/**
 * @brief Conectar a la base de datos y en caso de no existencia de 
 * la misma crearla. Si la tabla "devices" no está creada, se crea 
 */
void dbConect();

/**
 * @brief Cerrar la base de datos.
 */
void CloseDB();

/**
 * @brief Registrar un dispositivo en la base de datos.
 * @param id_device es el id asignado al dispositivo.
 * @param serial_device es el el numero de serie del dispositivo.
 */
void InsertDevice(char *id_device, char *serial_device);

/**
 * @brief Eliminar un dispositivo de la base de datos a partir del
 * numero de serie.
 * @param id es el el identificador del dispositivo a eliminar.
 * @return retorna 0 sino elimina el dispositivo.
 */
int DeleteDevice(char *serial_device);

/**
 * @brief Prepara la llamada para obtener todas las tuplas de la BD
 */
void GetAllDevice();

/**
 * @brief Cambiar el id de un dispositivo a partir un numero de 
 * serie determinado.
 * @param id_device es el nuevo id asignado al dispositivo.
 * @param serial_device es el el numero de serie del dispositivo.
 */
void ChangeIdDevice(char *idDevice, char *serialDevice);

/**
 * @brief Obtener llamada por llamada una tupla de la BD
 *  @return verdadero en caso de existir la tupla, falso en otro caso
 */ 
int GetOneDevice (device *dev);

/**
 * @brief Obtener e imprimir la version de la biblioteca
 * @return la version de la biblioteca
 */
char *version ();

/**
 * @brief Resetear el puntero a la tabla.
 * 
 */
 void ResetStmt ();
 

#endif
