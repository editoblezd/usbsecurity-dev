#include <usb-gui.h>
#include <storedevice.h>
#include <libintl.h>
#include <locale.h>
#include <config.h>
#include <gettext.h>
void
fatal_error_signal (int sig)
{
	unlink(LOCK);
	if (fatal_error_in_progress)
		raise (sig);
	fatal_error_in_progress = 1;
	if ((fopen(LOCK, "r") != NULL)) {
		unlink(LOCK);
	}
	signal (sig, SIG_DFL);
	raise (sig);
}
static gboolean
delete_marked (GtkWidget *widget,
        		  GdkEvent  *event,
        		  gpointer   user_data)
{
	 GtkWidget *dialog;
	 gint response;
	 dialog = gtk_message_dialog_new (GTK_WINDOW (window),
	 				   GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
	 				   GTK_MESSAGE_INFO,
	 				   GTK_BUTTONS_OK_CANCEL,
	 				   gettext ("Are you sure to delete the device?"));
	 response = gtk_dialog_run (GTK_DIALOG (dialog));
	 if (response == GTK_RESPONSE_OK)
	 {
		 remove_item(window, NULL, treeview);
	 }
	 gtk_widget_destroy (dialog);
	return TRUE;
}
static void
fixed_toggled (GtkCellRendererToggle *cell,
               gchar                 *path_str,
               gpointer               data)
{
  GtkTreeModel *model = (GtkTreeModel *)data;
  GtkTreeIter  iter;
  GtkTreePath *path = gtk_tree_path_new_from_string (path_str);
  gboolean fixed;
  gtk_tree_model_get_iter (model, &iter, path);
  gtk_tree_model_get (model, &iter, COLUMN_MARKED, &fixed, -1);
  fixed ^= 1;
  if (fixed) ref ++;
  else ref --;
  update_tool_bar();
  gtk_list_store_set (GTK_LIST_STORE (model), &iter, COLUMN_MARKED, fixed, -1);
  gtk_tree_path_free (path);
}
void update_tool_bar () 
{
  gtk_widget_set_sensitive (GTK_WIDGET(delete), ref);
  gtk_widget_set_sensitive (GTK_WIDGET(new), ref);
}
static GtkTreeModel *
create_model (void)
{
  gint i, len;
  GtkListStore *store;
  GtkTreeIter iter;
  /* create list store */
  store = gtk_list_store_new (NUM_COLUMNS,
                              G_TYPE_BOOLEAN,
                              G_TYPE_STRING,
                              G_TYPE_STRING);
  GList *list = get_all_device ();
  device *tmp;
  len = g_list_length (list);                             
	                            
  /* add data to the list store */
	for (i = 0; i < len; i++)
    {
		tmp = (device *) g_list_nth_data (list, i);
		gtk_list_store_append (store, &iter);
		gtk_list_store_set (store, &iter,
                          COLUMN_MARKED, FALSE,
                          COLUMN_ID, tmp->devidname, 
                          COLUMN_SERIAL, tmp->devserialname,
                          -1);
    }
  
  g_list_free_full (list, (GDestroyNotify) callback_destroy_g_list);
  return GTK_TREE_MODEL (store);
}
void
remove_item(GtkWidget * widget, GdkEvent * event , gpointer data)
{
    GtkTreeIter iter, *tmp;
    gboolean valid;
    valid = gtk_tree_model_get_iter_first (model, &iter);
    tmp = g_malloc (sizeof (GtkTreeIter));
    gboolean marked;
    gchar * id;
    gchar * serial;
    GList *list = NULL;
    while (valid)
    {
        gtk_tree_model_get (model, &iter,
                          COLUMN_MARKED, &marked,
                          COLUMN_ID, &id,
                          COLUMN_SERIAL, &serial,
                          -1);
        *tmp = iter;
		valid = gtk_tree_model_iter_next (model, &iter);
        if(marked)
        {
			list = g_list_append (list, g_strdup(serial));
		}
		ref --;
        g_free (serial);
        g_free (id);
    }
    g_free (tmp);
    /**Delete elements from DB*/
    gint i, len;
	len = g_list_length (list);
	printf ("There are %d item to delete and the las seria is :%s\n", len, serial);
	serial = NULL;
	for (i = 0; i < len; i++)
	{
		serial = (gpointer) g_list_nth_data (list, i);
		delete_device (serial);
		g_free (serial);
	}
	g_list_free (list);
	update_tool_bar();
}
gchar *
get_new_device_id(gchar *old_id)
{
    GtkWidget *dialog;
    GtkWidget *hbox, *vbox;
    GtkWidget *stock;
    GtkWidget *table;
    GtkWidget *local_entry1;
    GtkWidget *label;
    GtkWidget *note;
    gint response;
    gchar * result = NULL;

    dialog = gtk_dialog_new_with_buttons (gettext ("Add Device"),
                    GTK_WINDOW (window),
                    GTK_DIALOG_MODAL| GTK_DIALOG_DESTROY_WITH_PARENT,
                    GTK_STOCK_OK,
                    GTK_RESPONSE_OK,GTK_STOCK_CANCEL,GTK_RESPONSE_CANCEL,
                    NULL);

    hbox = gtk_hbox_new (FALSE, 8);
    vbox = gtk_vbox_new (FALSE, 2);

    gtk_box_pack_start (GTK_BOX (GTK_DIALOG (dialog)->vbox), vbox, FALSE, FALSE, 0);

    gtk_container_set_border_width (GTK_CONTAINER (hbox), 8);
    gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);

    stock = gtk_image_new_from_stock (GTK_STOCK_DIALOG_QUESTION, GTK_ICON_SIZE_DIALOG);
    gtk_box_pack_start (GTK_BOX (hbox), stock, FALSE, FALSE, 0);

    table = gtk_table_new (2, 2, FALSE);
    gtk_table_set_row_spacings (GTK_TABLE (table), 4);
    gtk_table_set_col_spacings (GTK_TABLE (table), 4);
    gtk_box_pack_start (GTK_BOX (hbox), table, TRUE, TRUE, 0);
    label = gtk_label_new_with_mnemonic (gettext ("Identifier of device"));
    gtk_table_attach_defaults (GTK_TABLE (table),label, 0, 1, 0, 1);
    local_entry1 = gtk_entry_new ();
    gtk_entry_set_text (GTK_ENTRY(local_entry1), old_id);
    gtk_table_attach_defaults (GTK_TABLE (table), local_entry1, 1, 2, 0, 1);

    note = gtk_label_new(gettext ("Note: We recommend don't repeat identifiers."));
    gtk_box_pack_end (GTK_BOX (vbox), note, FALSE, FALSE, 0);

    gtk_widget_show_all (vbox);
    response = gtk_dialog_run (GTK_DIALOG (dialog));
    puts ("Antes del resultado");
        result = g_strdup(gtk_entry_get_text (GTK_ENTRY (local_entry1)));
    puts ("Despues del resultado");
    gtk_widget_destroy (dialog);
    return result;
}
void add_item(GtkWidget * widget, GdkEvent * event , gpointer data)
{
    GtkTreeIter iter;
    gboolean valid;
    valid = gtk_tree_model_get_iter_first (model, &iter);
        gboolean marked;
        gchar * id;
        gchar * new_id;
        gchar * serial;
    new_id = g_strdup(get_new_device_id(id));
    while (valid)
    {
        gtk_tree_model_get (model, &iter,
                          COLUMN_MARKED, &marked,
                          COLUMN_ID, &id,
                          COLUMN_SERIAL, &serial,
                          -1);
        if(marked)
        {
            if(new_id && g_strcmp0(new_id,""))
            {
               change_idname (new_id, serial);
               g_free (new_id);
               mountUSB (serial);
            }
            g_free (serial);
			g_free (id);
			return;
        }
        else
        {
			g_free (serial);
			g_free (id);
		}
        valid = gtk_tree_model_iter_next (model, &iter);
    }
}
static void
add_columns (GtkTreeView *treeview)
{

  GtkTreeModel *model = gtk_tree_view_get_model (treeview);
  renderer = gtk_cell_renderer_toggle_new ();
  g_signal_connect (renderer, "toggled",
                    G_CALLBACK (fixed_toggled), model);

  column = gtk_tree_view_column_new_with_attributes ("",
                                                     renderer,
                                                     "active", COLUMN_MARKED,
                                                     NULL);
  gtk_tree_view_column_set_sizing (GTK_TREE_VIEW_COLUMN (column),
                                   GTK_TREE_VIEW_COLUMN_FIXED);
  gtk_tree_view_column_set_fixed_width (GTK_TREE_VIEW_COLUMN (column), 50);
  gtk_tree_view_append_column (treeview, column);
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes (gettext ("Identifier"),
                                                     renderer,
                                                     "text",
                                                     COLUMN_ID,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, COLUMN_ID);
  gtk_tree_view_append_column (treeview, column);
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Serial",
                                                     renderer,
                                                     "text",
                                                     COLUMN_SERIAL,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, COLUMN_SERIAL);
  gtk_tree_view_append_column (treeview, column);
}
static gboolean
window_closed (GtkWidget *widget,
               GdkEvent  *event,
               gpointer   user_data)
{
   unlink(LOCK);
    model = NULL;
    window = NULL;
    if (timeout != 0)
    {
        g_source_remove (timeout);
        timeout = 0;
    }
    gtk_main_quit();
    g_free(count);
    return FALSE;
}
GtkWidget *
do_list_store ()
{
    if (!window)
    {
      static GtkWidget *vbox;
      static GtkWidget *toolbar;
      window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
      gtk_window_set_title (GTK_WINDOW (window), gettext ("Devices Manager"));
      g_signal_connect (window, "destroy",
                        G_CALLBACK (gtk_widget_destroyed), &window);
      gtk_container_set_border_width (GTK_CONTAINER (window), 8);
      vbox = gtk_vbox_new (FALSE, 8);
      gtk_container_add (GTK_CONTAINER (window), vbox);
      toolbar = gtk_toolbar_new();
      gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);
      gtk_container_set_border_width(GTK_CONTAINER(toolbar), 2);
      new = gtk_tool_button_new_from_stock(GTK_STOCK_ADD);
      gtk_toolbar_insert(GTK_TOOLBAR(toolbar), new, -1);
      gtk_tool_item_set_tooltip_markup (new, gettext ("Allow Device"));
      g_signal_connect(G_OBJECT(new), "clicked",
                       G_CALLBACK(add_item), (gpointer)treeview);
      delete = gtk_tool_button_new_from_stock(GTK_STOCK_DELETE);
      gtk_toolbar_insert(GTK_TOOLBAR(toolbar), delete, -1);
      gtk_tool_item_set_tooltip_markup (delete, gettext ("Deny Device"));
      g_signal_connect(G_OBJECT(delete), "clicked",
                       G_CALLBACK(delete_marked), (gpointer)treeview);
	  update_tool_bar();
      gtk_box_pack_start (GTK_BOX (vbox), toolbar, FALSE, FALSE, 0);
      sw = gtk_scrolled_window_new (NULL, NULL);
      gtk_scrolled_window_set_shadow_type (GTK_SCROLLED_WINDOW (sw),
                                           GTK_SHADOW_ETCHED_IN);
      gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (sw),
                                      GTK_POLICY_NEVER,
                                      GTK_POLICY_AUTOMATIC);
      gtk_box_pack_start (GTK_BOX (vbox), sw, TRUE, TRUE, 0);
      model = create_model ();
      treeview = (GtkTreeView *)gtk_tree_view_new_with_model (model);
      gtk_tree_view_set_rules_hint (GTK_TREE_VIEW (treeview), TRUE);
      gtk_tree_view_set_search_column (GTK_TREE_VIEW (treeview),
                                       COLUMN_SERIAL);
      g_object_unref (model);
      gtk_container_add (GTK_CONTAINER (sw), (GtkWidget *)treeview);
      add_columns (GTK_TREE_VIEW (treeview));
      gtk_window_set_default_size (GTK_WINDOW (window), 280, 450);
      g_signal_connect (window, "delete-event",
                        G_CALLBACK (window_closed), NULL);
    }
    gtk_widget_show_all (window);
    return window;
}
void refresh ()
{
	GtkListStore *store = (GtkListStore *)gtk_tree_view_get_model (treeview);
	gtk_list_store_clear (store);
    GtkTreeIter iter;
    gint i, len;
	GList *list = get_all_device ();
	len = g_list_length (list);
	device *tmp;
	for (i = 0; i < len; i++)
	{
		tmp = (device *) g_list_nth_data (list, i);
		gtk_list_store_append (store, &iter);
		gtk_list_store_set (store, &iter,
                          COLUMN_MARKED, FALSE,
                          COLUMN_ID, tmp->devidname,
                          COLUMN_SERIAL, tmp->devserialname,
                          -1);
	}
	g_list_free_full (list, (GDestroyNotify) callback_destroy_g_list);
	ref = 0;
	update_tool_bar ();
    return;
}
gpointer 
thread (gpointer data)
{
	int length, i = 0;
	int fd;
	int wd;
	char buffer[EVENT_BUF_LEN];
  fd = inotify_init();
  if ( fd < 0 ) 
  {
    perror( "inotify_init" );
  }
  fd_set rfds;
  while (1)
  {
	wd = inotify_add_watch( fd, DB, IN_MODIFY | IN_CREATE | IN_DELETE | IN_DELETE_SELF );
	FD_ZERO (&rfds);
	FD_SET (fd, &rfds);
	select (FD_SETSIZE, &rfds, NULL, NULL, NULL);
	read( fd, buffer, EVENT_BUF_LEN);
	struct inotify_event *event = (struct inotify_event *) &buffer[i]; 
	if (event->mask & IN_MODIFY)
	{
		refresh ();
	}
	else if (event->mask & IN_CREATE)
		refresh ();
	else
		continue;
  }
}
int main (int argc, char **argv )
{
	setlocale( LC_ALL, "" );
	bindtextdomain(PACKAGE, LOCALEDIR);
	textdomain(PACKAGE);
	puts ("Roger, para que veas que funciona");
	ref = 0;
	FILE *file;
	file = fopen(LOCK, "r");
	if(file)
	{
		printf ("Ya existe una instancia de la aplicación\n");
		return 0;
	}
	file = NULL;
	signal(SIGTERM , fatal_error_signal);
	signal(SIGINT, fatal_error_signal);
	signal(SIGQUIT, fatal_error_signal);
	signal(SIGHUP, fatal_error_signal);
	signal(SIGSEGV, fatal_error_signal);
	file = fopen(LOCK, "w");
	fclose(file);
	count = g_malloc(sizeof(int));
	*count = 0;
	g_thread_init (NULL);
    gtk_init(&argc, &argv);
    g_thread_create (thread, NULL, FALSE, NULL);
    if (getuid() == 0)
    {
    	do_list_store();
    }
    else{
    	g_printf (gettext ("You aren't root user\nNo eres usuario root(Administrador)\n"));
    	unlink(LOCK);
    	return -1;
    }
    gtk_main();
   return 0;
}
