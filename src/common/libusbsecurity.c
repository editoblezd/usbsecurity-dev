#include "libusbsecurity.h"

void register_log ()
{
	gchar *line;
	FILE *file = fopen(LOG, "a");
	if (file == NULL)
	{
		error("Error to open log file");
		return;
	}
	line = g_strconcat(info.serial, " --- ",
			 getuser(), " --- ",
			 getmsg(), " --- ",
			 get_date(), NULL);
	syslog(LOG_INFO, "STR = %s", line);
	fprintf(file, "%s", line);
	g_free (line);
	fclose(file);
}
gchar * get_date () 
{
	time_t curtime;
	struct tm *loctime;
	/* Get the current time. */
	curtime = time (NULL);
	/* Convert it to local time representation. */
	loctime = localtime (&curtime);
	/* Print out the date and time in the standard format. */
	return asctime (loctime);
}
gchar *getuser ()
{
	struct utmp *utmp_test;
	setutent();
	utmp_test = getutent();
	while (utmp_test)
	{
		if (utmp_test->ut_line [0] == 't'
			&& utmp_test->ut_line [1] == 't'
			&& utmp_test->ut_line [2] == 'y'
			&&  utmp_test->ut_type == 7)
		{
			if (g_strcmp0 (utmp_test->ut_user, "root"))
			{
				endutent();
				return utmp_test->ut_user;
			}
		}
		utmp_test = getutent();
	}
	endutent();
	return "root\0";
}
pid_t getsession ()
{
	struct utmp *utmp_test;
	setutent();
	utmp_test = getutent();
	while (utmp_test)
	{
		if (utmp_test->ut_line [0] == 't'
			&& utmp_test->ut_line [1] == 't'
			&& utmp_test->ut_line [2] == 'y'
			&&  utmp_test->ut_type == 7)
		{
			endutent();
			return utmp_test->ut_pid;
		}
		utmp_test = getutent();
	}
	endutent();
	return 0;
}
const gchar *getmsg ()
{
	if (!info.authorized)
		return (const gchar *) "conectado (no autorizado)";
	return (const gchar *) "conectado (autorizado)";
}
gboolean detachUSB (const gchar *syspath)
{
	struct udev *udev;
	struct udev_device *dev;
	char *path;
	FILE *file;
	udev = udev_new();
	if (!udev) 
	{
		printf("Can't create udev\n");
		return FALSE;
	}
	dev = udev_device_new_from_syspath(udev, syspath);
	dev = udev_device_get_parent_with_subsystem_devtype(
		       dev,
		       "usb",
		       "usb_device");
	path = g_strconcat ((gchar *) udev_device_get_syspath (dev), "/authorized", NULL);
	if (!path) return FALSE;
	file = fopen (path, "r");
	if (!file)
	{
		udev_device_unref(dev);
		return FALSE;
	}
	fclose (file);
	file = fopen (path, "w");
	if (!file)
	{
		udev_device_unref(dev);
		return FALSE;
	}
	fprintf (file, "%d", 0);
	fclose (file);
	//~ writeInTmp (path, serial);
	free (path);
	udev_device_unref(dev);
	return TRUE;
}
writeInTmp (const gchar *syspath, const gchar *serial)
{
	FILE *file;
	gchar *path = g_strconcat (PATH_LOCK_DIR, "/", serial, NULL);
	file =fopen (path, "w");
	fprintf (file, "%s", syspath);
	fclose (file);
	free (path);
}
void mountUSB (const gchar *serial)
{
	gchar syspath[512], *path;
	FILE *file;
	path = g_strconcat (PATH_LOCK_DIR, "/", serial, NULL);
	file = fopen (path, "r");
	if (!file)
	{
		//~ fclose (file);
		free (path);
		return;
	}
	fscanf (file, "%s", syspath);
	fclose (file);
	
	file = NULL;
	file = fopen (syspath, "w");
	if (!file)
	{
		return;
	}
	fprintf (file, "%d", 1);
	fclose (file);
	remove (path);
	free (path);
}
void notify ()
{
	/*0J0: I must to define a directory for tmp data for to be erase*/
	gchar *user = getuser ();
	//~ gchar *user = "eblez";
	gchar *cmd = g_strconcat (
	" DBUS_SESSION_BUS_ADDRESS=$(grep -z DBUS /proc/`pgrep -u ",
	getuser (),
	" | head -n 1`/environ | sed -e 's/DBUS_SESSION_BUS_ADDRESS=//') ",
	" sudo -E -u ",
	getuser (),
	" sh -c \"notify-send -u normal -t 1000 -i /usr/share/usb-control-gui/usb.png -c Information",
	" 'Dispositivo no autorizado detectado'",
	" 'Para usar el dispositivo en este sistema, por favor, conctacte a un administrador'\"",		
	NULL);
	system (cmd);
	free (cmd);
}
gboolean register_new_device (const gchar *idname, const gchar *serial)
{
	dbConect();
	InsertDevice (idname, serial);
	CloseDB ();
	return TRUE;
}
gboolean delete_device (const gchar *serial)
{
	dbConect();
	DeleteDevice (serial);
	return TRUE;
}
gboolean change_idname (gchar *newidname, const gchar *serial)
{
	dbConect();
	ChangeIdDevice (newidname, serial);
	return TRUE;
	CloseDB();
}
gboolean authorized (const gchar *serial)
{
	dbConect();
	ResetStmt ();
	GetAllDevice ();
	device dev;
	while (GetOneDevice (&dev))
	{
		if (!g_strcmp0 (serial, dev.devserialname))
		{
			ResetStmt ();
			CloseDB();
			return g_strcmp0 (dev.devidname, "__NA__");
		}
	}
	ResetStmt ();
	CloseDB();
	return FALSE;
}
gboolean get_id_name (gchar **idname, const gchar *serial)
{
	dbConect();
	ResetStmt ();
	GetAllDevice ();
	device dev; // free this device
	while (GetOneDevice (&dev))
	{
		if (!g_strcmp0 (serial, dev.devserialname))
		{
			ResetStmt ();
			(*idname) = g_strdup (dev.devidname);
			return TRUE;
		}
	}
	ResetStmt ();
	CloseDB();
	return FALSE;
}
GList * get_all_device ()
{
	dbConect ();
	GList *list = NULL;
	device *dev; // free this device
	ResetStmt ();
	GetAllDevice ();
	dev = g_malloc0 (sizeof (device *));
	while (GetOneDevice (dev))
	{
		list = g_list_append (list, (gpointer) dev);
		dev = g_malloc0 (sizeof (device *));
	}
	g_free (dev);
	ResetStmt ();
	CloseDB();
	return list; //free this list
}
gboolean is_admin ()
{
	return (getuid() == 0);
}
gboolean is_locked_db ()
{
	FILE *file = fopen (LOCK, "r");
	if (file)
	{
		fclose (file);
		return TRUE;
	}
	return FALSE;
}
void callback_destroy_g_list (gpointer data)
{
	g_free (((device *)data)->devidname);
	g_free (((device *)data)->devserialname);
	g_free ((gpointer) data);
}

gchar *getserial (const char *path)
{
	struct udev *udev;
	struct udev_device *dev;
	gchar *serial;
	udev = udev_new();
	if (!udev) {
		printf("Can't create udev\n");
		exit(1);
	}
	dev = udev_device_new_from_syspath(udev, path);
	dev = udev_device_get_parent_with_subsystem_devtype(
														dev,
														"usb",
														"usb_device");
	if (!dev) {
		printf("Unable to find parent usb device.");
		exit(1);
	}
	serial = g_strdup (udev_device_get_sysattr_value(dev, "serial"));
	udev_device_unref(dev);
	udev_unref(udev);
	return serial;
}
