AUTOMAKE_OPTIONS = no-dependencies foreign
AM_CPPFLAGS	 = -I$(top_srcdir)/../.. -I$(top_srcdir)/src/ -I$(top_srcdir)/src/usb-manager -I$(top_srcdir)/src/common

usbconfdir = @sysconfdir@/usb-control
usbudevdir = @rulesdir@
usblogdir = @PATH_LOG_DIR@

#~ dist_usbconf_DATA = database.db
dist_usbudev_DATA = 70-disks-usb.rules
dist_usblog_DATA = usb-control.log

# Link time dependencies.
LDADD = @LIBINTL@

# Define a C macro LOCALEDIR indicating where catalogs will be installed.
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

sbin_PROGRAMS = usbcontrol
usbcontrol_SOURCES = usb-control.c usb-control.h  gettext.h
dist_man_MANS = usbcontrol.1

usbcontrol_CFLAGS = @USB_COMMON_CFLAGS@ -I$(srcdir)/../
usbcontrol_LDADD =  $(top_builddir)/src/usb-manager/libstoredevice.la $(top_builddir)/src/common/libusbsecurity.la
usbcontrol_DEPENDENCIES = $(top_builddir)/src/usb-manager/libstoredevice.la $(top_builddir)/src/common/libusbsecurity.la





