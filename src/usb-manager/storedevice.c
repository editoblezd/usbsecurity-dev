#include "storedevice.h"
static dbase dbas;
static sqlite3_stmt *res2;

void dbConect()
{	
	int flag = 0;
	FILE *file = fopen (DB, "r");
	if (file) 
	{
		flag = 1;
		fclose (file);
	}
	
	dbas.rc = sqlite3_open(DB, &dbas.db);

	if(dbas.rc != SQLITE_OK)
	{
		printf("Can't open database: %s\n",sqlite3_errmsg(dbas.db));
	}	
	else
	{
		if (!flag)
		{
			char *zSQL = sqlite3_mprintf("CREATE TABLE '%q'('%q','%q' primary key not null);",TABLE,IDNAME,SERIALNAME);
			if(sqlite3_exec(dbas.db, zSQL, 0, 0, 0) != SQLITE_OK)
				printf("%s\n",sqlite3_errmsg(dbas.db));		
			sqlite3_free(zSQL);
		}
	}	
}

void CloseDB()
{
	sqlite3_close(dbas.db);
}

void InsertDevice(char *id_device, char *serial_device)
{
	char *zSQL = sqlite3_mprintf("INSERT INTO devices ('id_device', 'serial_device') VALUES ('%q', '%q');",id_device,serial_device);
	
	if(sqlite3_exec(dbas.db,zSQL,0,0,0) != SQLITE_OK)
		printf("%s\n",sqlite3_errmsg(dbas.db));		
	sqlite3_free(zSQL);
}

int DeleteDevice(char *serial_device)
{
	int result = 1;
	printf ("Serial: %s\n", serial_device);
	char *zSQL = sqlite3_mprintf("DELETE FROM devices WHERE serial_device = '%q';",serial_device);
	
	if(sqlite3_exec(dbas.db,zSQL,0,0,0) != SQLITE_OK)
		printf("%s",sqlite3_errmsg(dbas.db));		
	else
		result = 0;
	
	sqlite3_free(zSQL);
	return result;
}

void ChangeIdDevice(char *idDevice, char *serialDevice)
{
	char *zSQL = sqlite3_mprintf("UPDATE devices SET id_device = '%q' WHERE serial_device = '%q';",idDevice, serialDevice);
	if(sqlite3_exec(dbas.db,zSQL,0,0,0) != SQLITE_OK)
		printf("%s",sqlite3_errmsg(dbas.db));		
	sqlite3_free(zSQL);	
}

void GetAllDevice()
{
	const char *tail;
	char *zSQL = sqlite3_mprintf("SELECT * FROM devices");
	while (sqlite3_prepare_v2(dbas.db,zSQL,-1,&res2,&tail) == SQLITE_BUSY)
		printf("Error in GetAllDevice: %s\n",sqlite3_errmsg(dbas.db));
	sqlite3_free(zSQL);
}

int GetOneDevice (device *dev)
{
	if(sqlite3_step(res2) == SQLITE_ROW)
	{
		(*dev).devidname = strdup (sqlite3_column_text(res2,0));
		(*dev).devserialname =  strdup (sqlite3_column_text(res2,1));		
		return 1;
	}
	return 0;
}

char *version ()
{
	printf ("Version: %s\n", USBMVERSION);
	return USBMVERSION;
}

void ResetStmt ()
{
	sqlite3_reset(res2);
}
