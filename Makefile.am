SUBDIRS = include
ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I$(top_srcdir)/include $(JANSSON_CFLAGS)
AM_CPPFLAGS += -I$(top_srcdir)/compat

AUTOMAKE_OPTIONS=foreign subdir-objects

bin_PROGRAMS=bgpq4
dist_man8_MANS=bgpq4.8

bgpq4_LDADD = $(PLATFORM_LDADD) $(PROG_LDADD) $(JANSSON_LIBS)

if !HAVE_STRLCPY
SUBDIRS += compat
bgpq4_LDADD += $(top_builddir)/compat/libcompat.la
check_PROGRAMS = test_rasa test_rasa_integration test_rasa_comprehensive
test_rasa_SOURCES = tests/test_rasa.c rasa.c rasa.h
test_rasa_LDADD = $(JANSSON_LIBS)
test_rasa_integration_SOURCES = tests/test_rasa_integration.c expander.c rasa.c extern.h rasa.h sx_prefix.c sx_report.c sx_slentry.c
test_rasa_integration_LDADD = $(JANSSON_LIBS) $(PLATFORM_LDADD)
test_rasa_comprehensive_SOURCES = tests/test_rasa_comprehensive.c rasa.c rasa.h
test_rasa_comprehensive_LDADD = $(JANSSON_LIBS)
    sx_slentry.c

check_PROGRAMS = test_rasa test_rasa_integration
test_rasa_SOURCES = tests/test_rasa.c rasa.c rasa.h
test_rasa_LDADD = $(JANSSON_LIBS)
test_rasa_integration_SOURCES = tests/test_rasa_integration.c expander.c rasa.c extern.h rasa.h sx_prefix.c sx_report.c sx_slentry.c
test_rasa_integration_LDADD = $(JANSSON_LIBS) $(PLATFORM_LDADD)

EXTRA_DIST=bootstrap README.md CHANGES

MAINTAINERCLEANFILES=configure aclocal.m4 compile \
                     install-sh missing Makefile.in depcomp \
                     stamp-h1 compat/Makefile.in \
		     config.guess config.sub include/Makefile.in \
		     ltmain.sh

maintainer-clean-local:
	-rm -rf m4 autom4te.cache

check:
	./bgpq4 -v
	@echo
	-if [ -s /etc/resolv.conf ]; then \
		./bgpq4 -ddd -6 AS15562:AS-SNIJDERS ; \
	else \
		echo "No or empty /etc/resolv.conf, skipping online test"; \
	fi
