/***********************************************************************
 * Zynk - A rsync based sync utility for GNOME
 * http://hanez.org/zynk.html
 *
 * Author: Johannes Findeisen <you@hanez.org>
 * Copyright 2005
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 **********************************************************************/

void on_mainwindow_destroy (GtkWidget *widget, gpointer data);

void on_button_open_destination_clicked (GtkWidget *widget, gpointer data);

void on_button_open_source_clicked (GtkWidget *widget, gpointer data);

void on_button_open_temp_dir_clicked (GtkWidget *widget, gpointer data);

void on_button_open_part_trans_dir_clicked (GtkWidget *widget, gpointer data);

void on_open_activate (GtkWidget *widget, gpointer data);

void on_quit_activate (GtkWidget *widget, gpointer data);

void run_dry_rsync (GtkWidget *widget, gpointer data);

void on_about_activate (GtkWidget *widget, gpointer data);

void on_toolbutton_quit_clicked (GtkWidget *widget, gpointer data);

void on_toolbutton_sync_clicked (GtkWidget *widget, gpointer data);

void update_gui_settings (GtkWidget *widget);

gboolean out_watch(GIOChannel *source, GIOCondition condition, gpointer data);

gboolean err_watch(GIOChannel *source, GIOCondition condition, gpointer data);

