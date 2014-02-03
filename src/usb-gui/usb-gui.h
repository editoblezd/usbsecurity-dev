/**
 * @file usb-gui.h
 * @author Edilberto Blez D.
 * @author Abel Alfonso F.D.
 */
#ifndef USB_GUI_H_
#define USB_GUI_H_
#include <gtk/gtk.h>

#include <libusbsecurity.h>

static gchar *user;
static int tmp_cont = 0;
static int ref;

/// A device
typedef struct
{
  const gchar    *identifier; ///< name that it identify
  const gchar    *serial; ///< device's serial number
}
DEV;

/// field for the GtkListStore
enum
{
  COLUMN_MARKED, ///< indicate if is marked (Value = 0)
  COLUMN_ID, ///< identifier (Value = 1)
  COLUMN_SERIAL, ///< serial number (Valued = 2)
  NUM_COLUMNS ///< Number of the field, in this case Value = 3
};

///Used in signal manage
volatile sig_atomic_t fatal_error_in_progress = 0;

///Represent the main window.
static GtkWidget *window = NULL;
///The model for the GtkTreeView.
static GtkTreeModel *model = NULL;
///Used en the main loop of GTK for know if there is event for to do.
static guint timeout = 0;
///Number of element of de model.
static int *count;
///Graphic component for showing the GtkListStore.
static GtkTreeView *treeview;
/// Grphic component that allow to treeview to be scrollable.
static GtkWidget *sw;
///Used to add column to treeview.
static GtkCellRenderer *renderer;
///Column for treeview
static GtkTreeViewColumn *column;
///Icon of a toolbar that represent delete action.
static GtkToolItem *delete;
///Icon of a toolbar that represent add action.
static GtkToolItem *new;

/**
 * @brief main function.
 * This is the begin of the program.
 * @param argc Number of the arguments.
 * @param argv List of the arguments.
 * @return
 */
int main(int argc, char **argv );
/**
 * @brief Modify status of toolbar.
 * Change the status of the component for add and delete row in the treeview.
 */
void update_tool_bar();
/**
 * @brief Create a model model.
 * Call to do_list_store function to retrieve the data and put these in a model
 * @return A GtkListStore as GtkTreeModel
 */
static GtkTreeModel *
create_model (void);

/**
 * @brief Function that creates the main window.
 * Creates all the component that is present in the main window and itself.
 * @return Main window
 */
GtkWidget *
do_list_store ();

/**
 * @brief Create a list of DEV *
 * Read the database normaly in $(confdir)/database.db and add to DEV * list the device there are.
 * @return DEV * pointer
 */
DEV * read_devices_list ();

/**
 * @brief Delete rows from treeview.
 * Search all rows these are marked and delete it.
 * @param widget The object the represent the component.
 * @param event is the action that was performed.
 * @param data other data that is passed to method.
 */
void
remove_item(GtkWidget * widget, GdkEvent * event , gpointer data);

/**
 * @brief The last function that is execute.
 * This function does the clean work in the program.
 * It deletes temporal files and finish the main loop, after this method only
 * gtk_main () is called.
 * @param widget The main window.
 * @param event The action that is taken place.
 * @param user_data The data that is passed to method.
 * @return Whether the actions were performed.
 */
static gboolean
window_closed (GtkWidget *widget,
               GdkEvent  *event,
               gpointer   user_data);

/**
 * @brief Add the columns for the treeview.
 * This fuctions adds all columns that have the treeview.
 * @param treeview The graphic component.
 */
static void
add_columns (GtkTreeView *treeview);

/**
 * @brief Check the checkbox component status.
 * When status change this function is called and the value change together with the rest component.
 * @param cell a cell that represent the column of the this component.
 * @param path_str is a path in a treeview (in a model).
 * @param data data passed to function.
 */
static void
fixed_toggled (GtkCellRendererToggle *cell,
               gchar                 *path_str,
               gpointer               data);
/**
 * @brief Delete all checkbox marked.
 * Delete rows of treeview. This show a confirmation message.
 * @param widget Graphic component.
 * @param event Action.
 * @param user_data data passed to function.
 * @return If the action was done.
 */
static gboolean
delete_marked (GtkWidget *widget,
        		  GdkEvent  *event,
        		  gpointer   user_data);

/**
 * @brief Add a device to a database.
 * This function essentially add a new entry in a database with the data of dev.
 * @param dev A device.
 */
void authorize_device (DEV *dev);

/**
 * @brief handler for exceptions.
 * This function is called when a exception is take place in the system.
 * When arrive a signal for to close the program this keep the signal in a
 * circule since all clean task is performed. When all is done it set the default handler
 * for that signal and allow to close the program.
 * @param sig A in that identify the arrived signal.
 *
 */
void
fatal_error_signal (int sig);

/**
 * @brief Add a item to treeview.
 * @param widget Component.
 * @param event Action.
 * @param data Data passed to function.
 */
void
add_item(GtkWidget * widget, GdkEvent * event , gpointer data);

/**
 * @brief Get a identify new for a row.
 * This is a auxiliary function which is called by add_item.
 * This show a confirmation message.
 * @return Identify's device.
 */
gchar *
get_new_device_id();

/**
 * @brief funcion unique for updating the model of the treeview.
 * This function is used only for refilled the model 
 * in the treeview component.
 * @return Model treeview's model
 * */
void
refresh ();

/**
 * @brief handler for thread's fun
 * Function that is executed when a thread is lanch
 * @param gponinter data
 * @return gpointer
 *
 *
 */
gpointer 
thread (gpointer data);

/**
 * @brief function to write to database 
 * This function write to a database the changes
 *  hechos en la interfaz.
 */
void writeToDB ();

/**
 * @brief Mount authorized device
 * When is a authorized device, we need
 * mount it on system.
 * 
 */
void mountAuthorized ();
#endif /* USB_GUI_H_ */

