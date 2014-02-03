#include <usb-control.h>

FILE *dg;
#define DG "/tmp/DG"

shellOption op = {FALSE, FALSE, NULL, NULL, NULL, NULL};
char *syspath = NULL;

static GOptionEntry entries[] =
{
  { "list", 'l', 0, G_OPTION_ARG_NONE, &op.list, "Show all device", NULL },
  { "version", 'v', 0, G_OPTION_ARG_NONE, &op.version, "Show the usbcontrol's version", NULL },
  { "delete", 'd', 0, G_OPTION_ARG_STRING, &op.delete, "Delete the device which have S as Serial", "S" },
  { "get", 'g', 0, G_OPTION_ARG_STRING, &op.get, "Select information about a device from its serial S", "S" },
  { "name", 'n', 0, G_OPTION_ARG_STRING, &op.name, "Set N name, in combination with --get=S, to selected device ", "N" },
  { "udev", 'u', 0, G_OPTION_ARG_STRING, &op.udev, "Used by udev where PATH is the device's way in sys(%S%p)", "P" },
  { NULL }
};
int main(int argc, char *argv[])
{
	syspath = g_strdup (argv [2]);
	dg = fopen (DG, "w+");
	fprintf (dg, "aqui esta, vamos a ver si llega\n");
	printf ("aqui esta, vamos a ver si llega\n");
	setlocale( LC_ALL, "" );
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
	fprintf (dg, "no llega\n");
	//~ printf ("%s\n", gettext ("Look Neyvis, this is true"));
	GError *error = NULL;
	GOptionContext *context;

	context = g_option_context_new ("- USBSecurity's help");
	g_option_context_add_main_entries (context, entries, PACKAGE);
	
	if (!g_option_context_parse (context, &argc, &argv, &error))
    {
      g_print (gettext ("option parsing failed: %s\n"), error->message);
      exit (EXIT_FAILURE);
    }
	if(op.version)
	{
		printf (gettext ("Version: usbcontrol 0.1\n"));
	}
	if(op.delete)
	{
		if (is_admin() && !is_locked_db ())
		{
			delete_device (op.delete);
		}
	}
	if(op.name)
	{
		if (is_admin () && !is_locked_db () && op.get)
		{
			change_idname (op.name, op.get);
			exit (EXIT_FAILURE);
		}
		else
		{
			puts ("paso por aqui");
			printf ("%s\n", g_option_context_get_help (context, TRUE, NULL));
		}
	}
	if(op.get) 
	{
		printf ("%-10s%-25s%-25s\n", gettext ("ID."), gettext ("NAME"), gettext ("SERIAL"));
		gchar *name = NULL;
		if (get_id_name (&name, op.get))
		{
			printf ("%-10s%-25s%-25s\n", "1", name, op.get);
			g_free (name);
		}
	}
	if(op.udev) 
	{
		fprintf (dg, "Llamo con el parametro a udev\n");
		info.serial = NULL;
		if (!init_values())
		{
			fail ();
			fclose (dg);
			return 1;
		}
		gboolean dev_auth = authorized (info.serial);
		if (!dev_auth)
		{
			InsertDevice ("__NA__", info.serial);
			fail ();
		}
		fprintf (dg, "Serial: %s\n", info.serial);
	}
	if (op.list)
	{
		list ();
	}
	fclose (dg);
	return !info.authorized;
}
gboolean init_values ()
{
		fprintf (dg, "PATH : %s\n", syspath);
		//~ getserial (path);
		fprintf (dg, "Serial: %s\n", getserial (syspath));
		
	//~ if (!(info.serial = g_getenv(ENV_SERIAL)))
	//~ {
		//~ return FALSE;
	//~ }
	//~ if (!(info.devtype = g_getenv(ENV_DEVTYPE)))
	//~ {
		//~ return FALSE;
	//~ }
	//~ 
		//~ getserial (&info.serial, path);
		info.serial = NULL;
		info.serial = getserial (syspath);
		info.date = get_date();
		info.user = getuser();
		
		return (info.serial != NULL);
}
int fail ()
{
	detachUSB (syspath);
	notify ();
	return 1;
}
void list ()
{
	printf ("%-10s%-25s%-25s\n", gettext ("ID."), gettext ( "NAME"), gettext ("SERIAL"));
	gint i, len;
	GList *list = get_all_device ();
	len = g_list_length (list);
	device *tmp;
	for (i = 0; i < len; i++)
	{
		tmp = (device *) g_list_nth_data (list, i);
		printf ("%-10d%-25s%-25s\n", i + 1, tmp->devidname, tmp->devserialname);
	}
	g_list_free_full (list, (GDestroyNotify) callback_destroy_g_list);
}



