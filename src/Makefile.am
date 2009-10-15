gladedir = $(datadir)/zynk/glade
SUBDIRS = xpm

INCLUDES = \
	$(WARN_CFLAGS) \
	@ZYNK_CFLAGS@ \
	-DG_LOG_DOMAIN=\"Zynk\" \
	-DGNOMELOCALEDIR=\""$(datadir)/locale"\" \
	-DPIXMAPDIR=\""$(datadir)/pixmaps"\" \
	-DGLADEDIR=\""$(datadir)/zynk/glade"\"

bin_PROGRAMS = zynk

zynk_LDFLAGS = -export-dynamic

zynk_SOURCES = \
    zynk_signals.c \
    zynk.c \
    settings.c \
    rsync.c \
    profiles_signals.c \
    profiles.c \
    preferences_signals.c \
    preferences.c \
    panel.c \
    zynk_signals.h \
    zynk.h \
    settings.h \
    rsync.h \
    profiles_signals.h \
    profiles.h \
    preferences_signals.h \
    preferences.h \
    panel.h

glade_DATA = \
	zynk.glade \
	preferences.glade

zynk_LDADD = @ZYNK_LIBS@

EXTRA_DIST = $(glade_DATA)
