/***********************************************************************
 * Zynk - A rsync based sync utility for GNOME
 * http://hanez.org/zynk.html
 *
 * Author: Johannes Findeisen <you@hanez.org>
 * Copyright 2005 - 2009
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 **********************************************************************/
 
#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

//#include <gnome.h>
#include <gtk/gtk.h>
#include <glade/glade.h>

#include "zynk_signals.h"

int main (int argc, char *argv[])
{
	//GtkWidget *mainwindow;
	GladeXML *xml;
	//GtkBuilder *builder;

	#ifdef ENABLE_NLS
		bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
		textdomain (PACKAGE);
	#endif

	//gnome_init (PACKAGE, VERSION, argc, argv);
  gtk_init (&argc, &argv);
	
	//glade_gnome_init ();

	xml = glade_xml_new (GLADEDIR"/zynk.glade", NULL, NULL);

	glade_xml_signal_autoconnect (xml);

	mainwindow = glade_xml_get_widget (xml, "mainwindow");
	
	//gnome_window_icon_set_from_file (mainwindow, PIXMAPDIR"/zynk.png");
	
	gtk_window_set_title (GTK_WINDOW (mainwindow), "Zynk");
	
	gtk_widget_show (mainwindow);

	gtk_main ();
	
	return 0;
}
