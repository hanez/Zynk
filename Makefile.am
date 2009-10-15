SUBDIRS = po src pixmaps examples

schema_in_files = zynk.schemas.in
schemadir = @GCONF_SCHEMA_FILE_DIR@
schema_DATA = $(schema_in_files:.schemas.in=.schemas)
@INTLTOOL_SCHEMAS_RULE@

Applicationsdir = $(prefix)/share/applications
Applications_in_files = zynk.desktop.in
Applications_DATA = $(Applications_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

EXTRA_DIST = autogen.sh \
	           CREDITS \
	           intltool-extract.in \
	           intltool-merge.in \
	           intltool-update.in \
	           $(schema_in_files) \
	           $(Applications_in_files) \
             $(Applications_DATA)

DISTCLEANFILES = intltool-extract \
                 intltool-merge \
                 intltool-update           

install-data-local:
	-GCONF_CONFIG_SOURCE=$(GCONF_SCHEMA_CONFIG_SOURCE) $(GCONFTOOL) --makefile-install-rule $(srcdir)/$(schema_DATA)

clean:
	rm -r zynk.desktop
	rm -r src/zynk
	rm -r src/*.o
	rm -r zynk.schemas
	rm -f example.keys
	rm -f po/*.gmo
	rm -f po/.intltool-merge-cache
	
