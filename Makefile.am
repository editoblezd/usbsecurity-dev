AUTOMAKE_OPTIONS = no-dependencies foreign
ACLOCAL_AMFLAGS  = -I m4 -Ilibltdl/m4
AM_CPPFLAGS	 = -I$(top_srcdir)/../..

SUBDIRS = src doc po m4

# Define a C macro LOCALEDIR indicating where catalogs will be installed.
DEFS = -DLOCALEDIR=\"$(localedir)\" @DEFS@

# Link time dependencies.
LDADD = @LIBINTL@


libtool: $(LIBTOOL_DEPS)
	$(SHELL) ./config.status --recheck

# Workaround a bug in Autoconf-2.61 and earlier that don't clean up
# file droppings left by many compilers:
distclean-local:
	ac_files="a.out.* a.exe.* a_out.exe.* b.out.* conftest.*"; \
	for ac_file in $$ac_files; do \
	  case $$ac_file in \
	    *.xcoff | *.tds | *.d | *.pdb | *.xSYM | *.bb | *.bbg | *.map | *.inf | *.dSYM ) rm -rf $$ac_file ;; \
	  esac; \
	done

EXTRA_DIST = libltdl/config/config.rpath autogen.sh autoclean.sh
