AC_PREREQ(2.54)

AC_INIT([usb-control], [0.1])
AC_CONFIG_AUX_DIR([libltdl/config])
AM_INIT_AUTOMAKE

AC_PROG_CC
AC_CHECK_HEADERS([unistd.h])
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.18.1])
AC_CONFIG_HEADERS([config.h])
AM_PROG_CC_C_O

## ----------------------- ##
## Libtool initialisation. ##
## ----------------------- ##
LT_INIT
AC_SUBST([LIBTOOL_DEPS])


## ------------------------------ ##
## Libraries required by depdemo. ##
## ------------------------------ ##
LT_LIB_SQLITE3


#Variable declarations
dnl BEGIN ------------------------------------------------------------------
AC_PREFIX_DEFAULT([/usr])
AC_SUBST(UDISKS)
AC_SUBST(PKGCONFIG)
dnl END ------------------------------------------------------------------

dnl ==============================================
dnl Check that we meet the dependencies
dnl ==============================================

GLIB_REQUIRED_VERSION=2.26.1
GTK_REQUIRED_VERSION=2.22.0
dnl for gsettings DESKTOP_SCHEMAS_REQUIRED_VERSION=3.0.2
dnl PA_REQUIRED_VERSION=0.9.16
dnl CANBERRA_REQUIRED_VERSION=0.13
dnl GDKPIXBUF_REQUIRED_VERSION=2.23.0
dnl POLKIT_REQUIRED_VERSION=0.103
GSD_REQUIRED_VERSION=3.3.91
dnl NETWORK_MANAGER_REQUIRED_VERSION=0.8.992
dnl When it use notification LIBNOTIFY_REQUIRED_VERSION=0.7.3
dnl GNOME_DESKTOP_REQUIRED_VERSION=3.1.91
dnl no for usb-control program SCHEMAS_REQUIRED_VERSION=3.3.0
dnl no necessary LIBWACOM_REQUIRED_VERSION=0.3

UDEV_REQUIRED_VERSION=151
UDISKS_REQUIRED_VERSION=1.0.1

COMMON_MODULES="glib-2.0 
				udev >= $UDEV_REQUIRED_VERSION 
				udisks >= $UDISKS_REQUIRED_VERSION" 

USB_GUI_MODULES="gtk+-2.0 >= $GTK_REQUIRED_VERSION
				 gthread-2.0
				 gio-2.0
				 gio-unix-2.0
				 gobject-2.0"
 
#Check PKG-CONFIG MODULES
dnl BEGIN ------------------------------------------------------------------
PKG_CHECK_MODULES(USB_COMMON, $COMMON_MODULES)

AC_SUBST(USB_COMMON_CFLAGS)
AC_SUBST(USB_COMMON_LIBS)

PKG_CHECK_MODULES(USB_GUI, $COMMON_MODULES $USB_GUI_MODULES)

AC_SUBST(USB_GUI_CFLAGS)
AC_SUBST(USB_GUI_LIBS)
dnl END ------------------------------------------------------------------

#Check needed PROGRAMS
dnl BEGIN ------------------------------------------------------------------
AC_CHECK_PROG(PKGCONFIG, pkg-config, yes, no)
if test "x$PKGCONFIG" == xno; then 
		AC_MSG_ERROR("Error finding pkg-config program"); fi
dnl END ------------------------------------------------------------------

#Check needed HEADER
dnl BEGIN --------------------------------------------------------------
AC_CHECK_HEADERS([unistd.h stdio.h stdlib.h string.h dnl
					syslog.h sys/types.h err.h locale.h libudev.h],,dnl
				[AC_MSG_ERROR(["Error, there are missing header"])], )
				
dnl Check for log				
AC_SUBST(CAN_HAVE_LOG)
AC_CHECK_HEADERS([utmp.h time.h])

if test "x$ac_cv_header_utmp_h" == xyes -a "x$ac_cv_header_time_h" == xyes; then
	CAN_HAVE_LOG=yes
else
	CAN_HAVE_LOG=no
fi

AC_ARG_ENABLE(logdir, [AS_HELP_STRING([[--enable-logdir[=DIR]]], [use log dir (default /var/log)])], 
[AS_CASE($enableval,
 [no], [AC_MSG_RESULT([* keep log feature disable.])], 
 [yes], [AC_SUBST([PATH_LOG_DIR], [/var/log])], 
 [/*], [AC_SUBST([PATH_LOG_DIR], [$enableval])],
 [*], [AC_MSG_ERROR([The path of log directory must be absolute])])],
[AC_MSG_RESULT([* keep log feature disable.])]
)

if test -n "$PATH_LOG_DIR";then
 	if test "x$CAN_HAVE_LOG" == xno;then
 		AC_MSG_WARN([** Disable log feature beacause there is no headers utmp.h or time.h])
 		PATH_LOG_DIR="";
 	else 
 		if test -d "$PATH_LOG_DIR";then
 			AC_DEFINE_UNQUOTED(PATH_LOG_DIR, ["$PATH_LOG_DIR"], [Path for save the log])
 		else
 			AC_MSG_ERROR([*** The path ("$PATH_LOG_DIR") for logdir no found on this system])
 		fi
 	fi
 fi
dnl exported variable for Makefile 
AM_CONDITIONAL([LOGENABLE], [test -n "$PATH_LOG_DIR"])

 dnl Check for lock files
 AC_SUBST([PATH_LOCK_DIR], [/tmp])
 AC_ARG_ENABLE(lockdir, [AS_HELP_STRING([[--enable-lockdir[=DIR]]], [use lock dir (default /tmp)])], 
[AS_CASE($enableval,
 [no], [AC_MSG_ERROR([*** This directory is necessary for the application])], 
 [yes], [AC_MSG_RESULT(* Will try to use default path (/tmp) as lock directory.)], 
 [/*], [AC_SUBST([PATH_LOCK_DIR], [$enableval])]
 [*], [AC_MSG_ERROR([The path of lock directory must be absolute])])],
[AC_MSG_RESULT([* Will try to use default path (/tmp) as lock directory.])])

if test -n "$PATH_LOCK_DIR";then
	if test -d "$PATH_LOCK_DIR";then
		AC_DEFINE_UNQUOTED(PATH_LOCK_DIR, ["$PATH_LOCK_DIR"], [Path for save the log])
	else
		AC_MSG_ERROR([*** The path ("$PATH_LOCK_DIR") for logdir no found on this system])
	fi
 fi
 
 dnl Check for rules dir
 dnl Important: the files of rules can be either in /etc/udev/rules.d/ or /lib/udev/rules.d/
 AC_ARG_VAR([rulesdir], [This is the place where is installed the rules [[default /etc/udev/rules.d/]]])
 
 if test -n "$rulesdir";then
 	if test ! -d "$rulesdir";then
 		AC_MSG_ERROR([The directory no exists])
 	fi
 else
  rulesdir="/etc/udev/rules.d/"
 fi
 
 if test "x$prefix" == xNONE;then
	prefix=$ac_default_prefix
 fi

 dnl force to put the prefix expansion in a file config.h for sysconfigdir
 eval sysconfig_dir="$sysconfdir"

 AC_DEFINE_UNQUOTED([DB], ["$sysconfig_dir/usb-control/database.db"], [This is the directory for the configurations y database])
 AC_DEFINE_UNQUOTED([LOCK], ["$PATH_LOCK_DIR/usbsecurity.lock"], [This is the directory for the configurations y database])
 if test -n "$PATH_LOG_DIR";then
 	AC_DEFINE_UNQUOTED([LOG], ["$PATH_LOG_DIR/usb-control.log"], [This is the directory for the configurations y database])
 fi
 
dnl Performing Makefiles
AC_CONFIG_FILES([
 Makefile
 src/Makefile
 src/usb-manager/Makefile
 src/common/Makefile
 src/usb-control/Makefile
 src/usb-gui/Makefile
 src/usb-control/70-disks-usb.rules
 doc/Makefile
 doc/usb-control/Makefile
 doc/usb-control/man/Makefile
 doc/usb-control/man/man3/Makefile
 doc/usb-gui/Makefile
 doc/usb-gui/man/Makefile
 doc/usb-gui/man/man3/Makefile
 m4/Makefile
 po/Makefile.in
 ])
AC_OUTPUT

