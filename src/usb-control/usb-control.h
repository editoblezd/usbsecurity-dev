/**
 * @file usb-control.h
 * @author Edilberto Blez D.
 *
 */

#ifndef USB_CONTROL_H_
#define USB_CONTROL_H_

#include <libudev.h>
#include <libusbsecurity.h>
#include <libintl.h>
#include <locale.h>
#include <config.h>
#include <gettext.h>


#define LOCALEDIR "/usr/share/locale/"

typedef struct __shellOption
{
	gboolean list;
	gboolean version;
	
	gchar *delete;
	gchar *get;
	gchar *name;
	gchar *udev;
}shellOption;



/**
 * @brief shut down the interface of the partition's device.
 * If this function is called that is because the device isi'n authorized
 * and it must to shut down.
 * @param dev is a name of the device node.
 * @return a value non-zero
 */
int fail ();

/**
 * @brief init the values in this file.
 * This is one of the primary function that is called to connect a device.
 * @return either TRUE if all could be done or FALSE otherwise.
 */
gboolean init_values ();

/**
 * @brief Check if the current device is authorized.
 * This function retrieves information about the device.
 * If the device has never been put in the system it get value 0.
 * if has been put but its identify is "No Autorizado" it get value 2.
 * if has been authorized it get value 1.
 * @return either 1 if is authorized or (0 or 2) otherwise.
 */
int is_authorized ();

/**
 * @brief Put a new connected device in database.
 * When you connect for first time a device it is added to the
 * database with the "No Autorizado" identify.
 */
void register_a_new_device ();

/**
 * @brief Show the brief description about the program's commands.
 * To invoke this program with the --help or -h option take place that
 * this function is called. Here is showed the possible commands that support
 * the program for a quick search of the necessary option.
 */
void help();
/**
 * @brief Show all the information into database.
 * enumerate each element into the database and puts a number for each one.
 */
void list ();

/**
 * @brief Remove a device from database.
 * This function start for looking the device which have number as identifier
 * and after delete it.
 *
 * @param number This is the id for the element into database.
 */
void delete (int number);

/**
 * @brief Change the name for a item into database.
 * This function start for looking the device which have number as identifier
 * and after change its name.
 *
 * @param number This is the id for the element into database.
 * @param name This is the name for the element into database.
 */
void change (int number, char *name);
/**
 * @brief Get information only of the device with id as number .
 * This function start for looking the device which have number as identifier
 * and after show information about itself.
 *
 * @param number This is the id for the element into database.
 */
void getinfo (int number);
#endif /* USB_CONTROL_H_ */




