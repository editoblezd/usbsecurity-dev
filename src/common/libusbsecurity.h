/**
 * @file common.h
 * @author Edilberto Blez D.
 */

#ifndef COMMON_H_
#define COMMON_H_


#include <unistd.h>
#include <glib.h>
#include <glib/gprintf.h>
#include <time.h>
#include <utmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <sys/types.h>
#include <err.h>
#include <locale.h>
//~ #include <config.h>
#include <ctype.h>
#include <sys/inotify.h>

#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <libudev.h>
#include <storedevice.h>


#include <sys/select.h>

///Default identifier for connected device new to system
#define NO_AUTORIZADO "No autorizado"
///Serial number short imported from udev environment
#define ENV_SERIAL "ID_SERIAL_SHORT"
///Devtype imported from udev environment
#define ENV_DEVTYPE "DEVTYPE"
///Name for tmp file for notify-send
#define FILE_NOTIFY "notify.tmp"
///For debug
#define FILE_DEBUG "/tmp/debug"
/// For a lock directory 
#define PATH_LOCK_DIR "/tmp"
/// For a log file 
#define LOG "/var/log/usb-control.log"
/// For a lock file
#define LOCK "/tmp/usbsecurity.lock"




#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

/*
 * @brief All necessary data.
 */
 
typedef struct __request{
	gchar *user; ///< User which connected the device.
	const gchar *serial; ///< Serial of the device.
	const gchar *devtype; ///< device's devtype
	gboolean authorized; ///< either is authorized or no.
	gchar *date; ///< current date of the system.
}request;

request info;

/**
 * @brief Add a entry to log file.
 * Depend of a action this function add a new entry to a log file.
 * @param info contain the all necessary information about the action.
 */
void register_log ();

/**
 * @brief Get a date in gchar * form.
 * This function is used to retrieve the date from the system.
 * @return gchar * with the current date.
 */
gchar * get_date ();

/**
 * @brief Get the current user in the system.
 * Tries to get the user that is current connected to system.
 * As can be connectar more than one user, the application tries to get one
 * if this can, otherwise unknown is returned.
 * @return One current user o unknown if can't.
 */
gchar *getuser ();

/**
 * @brief Name of action.
 * Get a name of the action according with the value of info.autorized.
 * @return a gchar * associate.
 */
const gchar *getmsg ();

/**
 * @brief Change state of a device.
 *  This function is used for take decition about whether a device.
 *  is authorized or no.
 * @param serial Serial of the device
 * @return return TRUE in case of success, FLASE otherwise.
 * 
 */
gboolean detachUSB (const gchar *serial);

/**
 * @brief Mount a device.
 * If a device have not been removed of the port, so 
 * it can be mount without need of removed.
 * @param serial Serial of a device.
 * 
 */
void mountUSB (const gchar *serial);

/**
 * @brief Save in /tmp syspath. 
 * This  function is to save into /tmp directory.
 * the syspath of the device because of it is unauthorirez.
 * @param syspath walk to authorized file into sys
 * @param serial Serial of device.
 */

/**
 * @brief Notice about a device.
 * This function indicate when a device no
 * authorized is connected to PC.
 */
void notify ();

/**
 * @brief Get Session process id.
 * This function is to get the session process id
 * of the current user.
 *  @return pid of the session.
 */
pid_t getsession ();

/**
 * @brief Print in a its syspath.
 * The aim of this function is save tomporaly.
 * the syspath of the device for wethere is authorized
 * it can be mount without to remove a device. For this
 * created in /tmpdir/serial, where serial is device's serial
 * the device's syspath. /tmpdir was define when was compiled 
 * the program. It supose that this file will be erase to close
 * the gui.
 * @param syspath syspath on sys for the device.
 * @param serial the device's serial.
 */
writeInTmp (const gchar *syspath, const gchar *serial);


/**
 * @brief Delete a tuple of the DB.
 * @param serial
 * @return true or false whether the action take place.
 */

gboolean delete_device (const gchar *serial);

/**
 * @brief Change idname for a entry in a DB.
 * @param old idname.
 * @param serial.
 * @return true or false whether the action take place.
 */
gboolean change_idname (gchar *newidname, const gchar *serial);


/**
 * @brief Know whether a device is authorized.
 * @param serial.
 * @return true o false.
 */
 gboolean authorized (const gchar *serial);
 
 /**
  * @brief Get idname by serial.
  * @param idname
  * @param serial.
  * @return true or false whether the action take place.
  */
 gboolean get_id_name (gchar **idname, const gchar *serial);
 
 /**
  * @brief Get all device.
  * @return List of all device struct
  * 
  */
  GList *get_all_device ();
  
  /**
   * @brief Permission for excute a command
   * @return true or false
   */
   gboolean is_admin ();
   
   /**
    * @brief Know whether BD is locked
    * @return true or false
    */
    gboolean is_locked_db ();
    
    /**
     * @brief Callback for GDestroyNotify
     */
     void callback_destroy_g_list (gpointer data);
     
     
     char *getserial (const char *path);
     
     
    
#endif /* COMMON_H_ */
