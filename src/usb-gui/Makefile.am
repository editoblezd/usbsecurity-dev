AUTOMAKE_OPTIONS = no-dependencies foreign
AM_CPPFLAGS	 = -I$(top_srcdir)/../.. -I$(top_srcdir)/src/ -I$(top_srcdir)/src/usb-manager -I$(top_srcdir)/src/common


usbsharedir = @datadir@/usb-control-gui
usbappdir =  @datadir@/applications


# Define a C macro LOCALEDIR indicating where catalogs will be installed.
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

# Link time dependencies.
LDADD = @LIBINTL@

dist_usbshare_DATA = usb.png
dist_bin_SCRIPTS = usb-control-gtk
dist_usbapp_DATA = usb-control-gtk.desktop

sbin_PROGRAMS = usbgui
usbgui_SOURCES = usb-gui.c usb-gui.h gettext.h


usbgui_CFLAGS = @USB_GUI_CFLAGS@ -I$(srcdir)/../
usbgui_LDFLAGS = @USB_GUI_LIBS@
usbgui_DEPENDENCIES = $(top_builddir)/src/usb-manager/libstoredevice.la $(top_builddir)/src/common/libusbsecurity.la
usbgui_LDADD = @USB_GUI_LIBS@ $(top_builddir)/src/usb-manager/libstoredevice.la $(top_builddir)/src/common/libusbsecurity.la

