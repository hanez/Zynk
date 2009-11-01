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

#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <gnome.h>
#include <glade/glade.h>

#include "settings.h"

#include "xpm/zynk-pixbufs.h"

GString   *shellcommand;
gchar     **argv;
GPid      rsync_pid;
ZConf     run_settings;
ZGui      gui_settings;
gboolean  dry_run;


void 
on_mainwindow_destroy (GtkWidget *widget, gpointer data)
{
  if (rsync_pid) kill(rsync_pid, 1);
  gtk_main_quit();
  return;
}

void
on_mainwindow_show (GtkWidget *widget, gpointer data)
{


}

void
on_zynk_homepage_activate (GtkWidget *widget, gpointer data)
{
  gnome_url_show ("http://hanez.org/zynk.html", NULL);
  return;
}

void
on_rsync_homepage_activate (GtkWidget *widget, gpointer data)
{
  gnome_url_show ("http://rsync.samba.org/", NULL);
  return;
}

void
on_rsync_manpage_activate (GtkWidget *widget, gpointer data)
{
  gnome_url_show ("http://rsync.samba.org/ftp/rsync/rsync.html", NULL);
  return;
}

void 
on_toolbutton_open_clicked (GtkWidget *widget, gpointer data)
{
  open_project_file (widget);
  return;
}

void 
on_open_activate (GtkWidget *widget, gpointer data)
{
  open_project_file (widget);
  return;
}

void 
on_quit_activate (GtkWidget *widget, gpointer data)
{
  if (rsync_pid) kill(rsync_pid, 1);
  gtk_main_quit();
  return;
}

void 
on_toolbutton_save_clicked (GtkWidget *widget, gpointer data)
{
  //gboolean i = write_settings ("/home/hanez/Desktop/zynk.zynk", 
  //    &run_settings);
  return;
}

void
on_toolbutton_cancel_clicked (GtkWidget *widget, gpointer data) {

  if (rsync_pid) kill(rsync_pid, 1);
  return;
}

void 
on_about_activate(GtkWidget *widget, gpointer data)
{
  GdkPixbuf *pixbuf;
  static GtkWidget *dialog = NULL;

  /* Don't create more than one about box */
  if (dialog != NULL) {
    g_assert (GTK_WIDGET_REALIZED (dialog));
    gdk_window_show (dialog->window);
    gdk_window_raise (dialog->window);
  } else {
    const gchar *authors[] = {
        "Johannes Findeisen <you@hanez.org> - Core Development",
        NULL};

    pixbuf = gdk_pixbuf_new_from_inline (-1, zynklogo, FALSE, NULL);

    dialog = gnome_about_new (
        "Zynk", VERSION,
        "(C) 2005-2009 Johannes Findeisen",
        _("A rsync based sync utility for GNOME."),
        authors,
        NULL,
        NULL, 
        pixbuf );

    g_signal_connect (G_OBJECT (dialog), 
        "destroy", 
        G_CALLBACK (gtk_widget_destroyed), 
        &dialog);

    g_object_unref (G_OBJECT(pixbuf));
    gtk_widget_show (dialog);
  }
  return;
}

void 
on_button_open_source_clicked (GtkWidget *widget, gpointer data)
{
  GtkWidget *projectpath;
  GtkWidget *dialog;
  GladeXML *xml;

  xml = glade_get_widget_tree(GTK_WIDGET (widget));

  projectpath = glade_xml_get_widget(xml, "entry_sourcepath");

  gint retval;
  dialog = gtk_file_chooser_dialog_new ("Select source directory", 
      NULL, 
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER, 
      GTK_STOCK_CANCEL, 
      GTK_RESPONSE_CANCEL, 
      GTK_STOCK_OPEN, 
      GTK_RESPONSE_ACCEPT, 
      NULL);
  
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog),
      gtk_entry_get_text((GTK_ENTRY(projectpath))));

  retval = gtk_dialog_run (GTK_DIALOG (dialog));
  if (retval == GTK_RESPONSE_ACCEPT || retval == GTK_RESPONSE_OK) {
    gchar *filename, *tmp;
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    tmp = g_malloc(strlen(filename) + 2);
    strcpy(tmp, filename);
    strcat(tmp, "/");
    gtk_entry_set_text (GTK_ENTRY(projectpath), tmp);
    g_free(filename);
    g_free(tmp);
  }
  gtk_widget_destroy (dialog);
  return;
}

void 
on_button_open_destination_clicked (GtkWidget *widget, gpointer data)
{
  GtkWidget *projectpath;
  GtkWidget *dialog;
  GladeXML *xml;

  xml = glade_get_widget_tree(GTK_WIDGET (widget));

/* FIX THIS STUPID "projectpath" - everything is available! no need for "xml" */
  projectpath = glade_xml_get_widget(xml, "entry_destinationpath");

  gint retval;
  dialog = gtk_file_chooser_dialog_new ("Select destination directory", 
      NULL, 
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      GTK_STOCK_CANCEL, 
      GTK_RESPONSE_CANCEL, 
      GTK_STOCK_OPEN, 
      GTK_RESPONSE_ACCEPT, 
      NULL);
  
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog),
      gtk_entry_get_text((GTK_ENTRY(projectpath))));

  retval = gtk_dialog_run (GTK_DIALOG (dialog));
  if (retval == GTK_RESPONSE_ACCEPT || retval == GTK_RESPONSE_OK) {
    gchar *filename, *tmp;
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    tmp = g_malloc(strlen(filename) + 2);
    strcpy(tmp, filename);
    strcat(tmp, "/");
    gtk_entry_set_text (GTK_ENTRY(projectpath), tmp);
    g_free(filename);
    g_free(tmp);
  }
  gtk_widget_destroy (dialog);
  return;
}

void 
on_button_open_temp_dir_clicked (GtkWidget *widget, gpointer data)
{
  GtkWidget *projectpath;
  GtkWidget *dialog;
  GladeXML *xml;

  xml = glade_get_widget_tree(GTK_WIDGET (widget));

  projectpath = glade_xml_get_widget(xml, 
      "entry_create_temporary_files_in_dir");

  gint retval;
  dialog = gtk_file_chooser_dialog_new ("Select temp directory",
      NULL, 
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      GTK_STOCK_CANCEL, 
      GTK_RESPONSE_CANCEL, 
      GTK_STOCK_OPEN, 
      GTK_RESPONSE_ACCEPT, 
      NULL);

  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog),
      gtk_entry_get_text((GTK_ENTRY(projectpath))));

  retval = gtk_dialog_run (GTK_DIALOG (dialog));
  if (retval == GTK_RESPONSE_ACCEPT || retval == GTK_RESPONSE_OK) {
    gchar *filename, *tmp;
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    tmp = g_malloc(strlen(filename) + 2);
    strcpy(tmp, filename);
    strcat(tmp, "/");
    gtk_entry_set_text (GTK_ENTRY(projectpath), tmp);
    g_free(filename);
    g_free(tmp);
  }
  gtk_widget_destroy (dialog);
  return;
}

void 
on_button_open_part_trans_dir_clicked (GtkWidget *widget, gpointer data)
{
  GtkWidget *projectpath;
  GtkWidget *dialog;
  GladeXML *xml;

  xml = glade_get_widget_tree(GTK_WIDGET (widget));

  projectpath = glade_xml_get_widget(xml, 
      "entry_put_a_partially_transferred_files_into_dir");

  gint retval;
  dialog = gtk_file_chooser_dialog_new ("Select directory for partially transferred files", 
      NULL, 
      GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
      GTK_STOCK_CANCEL, 
      GTK_RESPONSE_CANCEL, 
      GTK_STOCK_OPEN, 
      GTK_RESPONSE_ACCEPT, 
      NULL);
  
  gtk_file_chooser_set_current_folder (GTK_FILE_CHOOSER (dialog),
      gtk_entry_get_text((GTK_ENTRY(projectpath))));

  retval = gtk_dialog_run (GTK_DIALOG (dialog));
  if (retval == GTK_RESPONSE_ACCEPT || retval == GTK_RESPONSE_OK) {
    gchar *filename, *tmp;
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    tmp = g_malloc(strlen(filename) + 2);
    strcpy(tmp, filename);
    strcat(tmp, "/");
    gtk_entry_set_text (GTK_ENTRY(projectpath), tmp);
    g_free(filename);
    g_free(tmp);
  }
  gtk_widget_destroy (dialog);
  return;
}

void
get_widgets_from_xml (GtkWidget *widget, gpointer data)
{
  GladeXML *xml;
  
  xml = glade_get_widget_tree (GTK_WIDGET (widget));

  gui_settings.checkbutton_recurse_into_directories = glade_xml_get_widget (xml, "checkbutton_recurse_into_directories");
  gui_settings.checkbutton_suppres_non_error_messages = glade_xml_get_widget (xml, "checkbutton_suppres_non_error_messages");
  gui_settings.checkbutton_increase_verbosity = glade_xml_get_widget (xml, "checkbutton_increase_verbosity");
  gui_settings.checkbutton_turn_off_relative = glade_xml_get_widget (xml, "checkbutton_turn_off_relative");
  gui_settings.checkbutton_dont_send_implied_dirs_with_relative = glade_xml_get_widget (xml, "checkbutton_dont_send_implied_dirs_with_relative");
  gui_settings.checkbutton_use_relative_path_names = glade_xml_get_widget (xml, "checkbutton_use_relative_path_names");
  gui_settings.checkbutton_transfer_directories_without_recursing = glade_xml_get_widget (xml, "checkbutton_transfer_directories_without_recursing");
  gui_settings.checkbutton_preserve_permissions = glade_xml_get_widget (xml, "checkbutton_preserve_permissions");
  gui_settings.checkbutton_preserve_owner = glade_xml_get_widget (xml, "checkbutton_preserve_owner");
  gui_settings.checkbutton_preserve_group = glade_xml_get_widget (xml, "checkbutton_preserve_group");
  gui_settings.checkbutton_preserve_devices = glade_xml_get_widget (xml, "checkbutton_preserve_devices");
  gui_settings.checkbutton_preserve_times = glade_xml_get_widget (xml, "checkbutton_preserve_times");
  gui_settings.checkbutton_omit_directories_when_preserving_times = glade_xml_get_widget (xml, "checkbutton_omit_directories_when_preserving_times");
  gui_settings.checkbutton_update_destination_files_inplace = glade_xml_get_widget (xml, "checkbutton_update_destination_files_inplace");
  gui_settings.checkbutton_only_update_files_that_already_exist = glade_xml_get_widget (xml, "checkbutton_only_update_files_that_already_exist");
  gui_settings.spinbutton_dont_delete_more_then_num_files = glade_xml_get_widget (xml, "spinbutton_dont_delete_more_then_num_files");
  gui_settings.checkbutton_sent_files_are_removed_from_sender = glade_xml_get_widget (xml, "checkbutton_sent_files_are_removed_from_sender");
  gui_settings.checkbutton_delete_files_that_dont_exist_on_sender = glade_xml_get_widget (xml, "checkbutton_delete_files_that_dont_exist_on_sender");
  gui_settings.checkbutton_also_delete_excluded_files_on_receiver = glade_xml_get_widget (xml, "checkbutton_also_delete_excluded_files_on_receiver");
  gui_settings.checkbutton_delete_even_if_there_are_io_errors = glade_xml_get_widget (xml, "checkbutton_delete_even_if_there_are_io_errors");
  gui_settings.checkbutton_force_deletion_of_dirs_even_if_not_empty = glade_xml_get_widget (xml, "checkbutton_force_deletion_of_dirs_even_if_not_empty");
  gui_settings.checkbutton_dont_skip_files_that_match_size_and_time = glade_xml_get_widget (xml, "checkbutton_dont_skip_files_that_match_size_and_time");
  gui_settings.checkbutton_skip_files_that_match_in_size = glade_xml_get_widget (xml, "checkbutton_skip_files_that_match_in_size");
  gui_settings.spinbutton_dont_transfer_any_file_larger_than_size = glade_xml_get_widget (xml, "spinbutton_dont_transfer_any_file_larger_than_size");
  gui_settings.spinbutton_compare_mod_times_with_reduced_accuracy = glade_xml_get_widget (xml, "spinbutton_compare_mod_times_with_reduced_accuracy");
  gui_settings.checkbutton_skip_based_on_checksum = glade_xml_get_widget (xml, "checkbutton_skip_based_on_checksum");  
  gui_settings.checkbutton_skip_files_that_are_newer_on_the_receiver = glade_xml_get_widget (xml, "checkbutton_skip_files_that_are_newer_on_the_receiver");  
  gui_settings.checkbutton_ignore_files_that_already_exist_on_receiver = glade_xml_get_widget (xml, "checkbutton_ignore_files_that_already_exist_on_receiver");
  gui_settings.checkbutton_autoignore_files_in_the_same_way_cvs_does = glade_xml_get_widget (xml, "checkbutton_autoignore_files_in_the_same_way_cvs_does");  
  gui_settings.checkbutton_dont_cross_filesystem_boundaries = glade_xml_get_widget (xml, "checkbutton_dont_cross_filesystem_boundaries");  
  gui_settings.checkbutton_copy_files_whole = glade_xml_get_widget (xml,  "checkbutton_copy_files_whole");  
  gui_settings.checkbutton_always_use_incremental_rsync_algorithm = glade_xml_get_widget (xml, "checkbutton_always_use_incremental_rsync_algorithm");  
  gui_settings.checkbutton_put_all_updated_files_into_place_at_end = glade_xml_get_widget (xml, "checkbutton_put_all_updated_files_into_place_at_end");  
  gui_settings.checkbutton_dont_map_uid_gid_values_by_user_group_name = glade_xml_get_widget (xml, "checkbutton_dont_map_uid_gid_values_by_user_group_name");
  gui_settings.spinbutton_set_io_timeout_in_seconds = glade_xml_get_widget (xml, "spinbutton_set_io_timeout_in_seconds");
  gui_settings.checkbutton_find_similiar_file_for_basis_if_no_dest_file = glade_xml_get_widget (xml, "checkbutton_find_similiar_file_for_basis_if_no_dest_file");      
  gui_settings.checkbutton_compress_file_data_during_the_transfer = glade_xml_get_widget (xml, "checkbutton_compress_file_data_during_the_transfer");
  gui_settings.checkbutton_keep_partially_transfered_files = glade_xml_get_widget (xml, "checkbutton_keep_partially_transfered_files");
  gui_settings.entry_put_a_partially_transferred_files_into_dir = glade_xml_get_widget (xml, "entry_put_a_partially_transferred_files_into_dir");
  gui_settings.checkbutton_copy_symlinks_as_symlinks = glade_xml_get_widget (xml, "checkbutton_copy_symlinks_as_symlinks");    
  gui_settings.checkbutton_transform_symlink_into_referent_file_dir = glade_xml_get_widget (xml, "checkbutton_transform_symlink_into_referent_file_dir");  
  gui_settings.checkbutton_only_unsafe_symlinks_are_transformed = glade_xml_get_widget (xml, "checkbutton_only_unsafe_symlinks_are_transformed");  
  gui_settings.checkbutton_ignore_symlinks_that_point_outside_the_tree = glade_xml_get_widget (xml, "checkbutton_ignore_symlinks_that_point_outside_the_tree");      
  gui_settings.checkbutton_preserve_hardlinks = glade_xml_get_widget (xml, "checkbutton_preserve_hardlinks");    
  gui_settings.checkbutton_treat_symlinked_dir_on_receiver_as_dir = glade_xml_get_widget (xml, "checkbutton_treat_symlinked_dir_on_receiver_as_dir");
  gui_settings.spinbutton_force_a_fixed_checksum_blocksize = glade_xml_get_widget (xml, "spinbutton_force_a_fixed_checksum_blocksize");
  gui_settings.spinbutton_limit_io_bandwith_kbytes_per_second = glade_xml_get_widget (xml, "spinbutton_limit_io_bandwith_kbytes_per_second");
  gui_settings.entry_custom_args = glade_xml_get_widget (xml, "entry_custom_args");
  gui_settings.checkbutton_specify_the_remote_shell_to_use = glade_xml_get_widget (xml, "checkbutton_specify_the_remote_shell_to_use");
  gui_settings.entry_remote_shell = glade_xml_get_widget (xml, "entry_remote_shell");
  gui_settings.checkbutton_specify_the_rsync_to_run_on_remote_machine = glade_xml_get_widget (xml, "checkbutton_specify_the_rsync_to_run_on_remote_machine");
  gui_settings.entry_remote_rsync = glade_xml_get_widget (xml, "entry_remote_rsync");
  gui_settings.entry_sourcepath = glade_xml_get_widget (xml, "entry_sourcepath");
  gui_settings.entry_hostname = glade_xml_get_widget (xml, "entry_hostname");
  gui_settings.entry_username = glade_xml_get_widget (xml, "entry_username");
  gui_settings.entry_password = glade_xml_get_widget (xml, "entry_password");
  gui_settings.entry_destinationpath = glade_xml_get_widget (xml, "entry_destinationpath");
  gui_settings.checkbutton_sync_to_host = glade_xml_get_widget (xml, "checkbutton_sync_to_host");
  gui_settings.commandoutput = glade_xml_get_widget (xml, "textview_log");
  gui_settings.command = glade_xml_get_widget (xml, "textview_command");
  return;
}

void
update_run_settings (GtkWidget *widget, gpointer data) 
{

  get_widgets_from_xml (widget, data);

  gint i = 0;
  argv = g_new(gchar *, 3000);
  argv[i++] = "rsync";
  shellcommand = g_string_new ("rsync ");

  if(dry_run == TRUE) {
    argv[i++] = "-n";
    shellcommand = g_string_append (shellcommand, "-n ");
  }
  
  run_settings.recurse_into_directories = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (gui_settings.checkbutton_recurse_into_directories));
  if (run_settings.recurse_into_directories != 0) {
    shellcommand = g_string_append (shellcommand, "--recursive ");
    argv[i++] = "--recursive";
  }

  run_settings.suppres_non_error_messages = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (gui_settings.checkbutton_suppres_non_error_messages));
  if (run_settings.suppres_non_error_messages != 0) {
    shellcommand = g_string_append (shellcommand, "--quiet ");
    argv[i++] = "--quiet";
  }

  run_settings.increase_verbosity = gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (gui_settings.checkbutton_increase_verbosity));
  if(run_settings.increase_verbosity != 0) {
    shellcommand = g_string_append (shellcommand, "--verbose ");
    argv[i++] = "--verbose";
  }
    
  run_settings.use_relative_path_names = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_use_relative_path_names));
  if(run_settings.use_relative_path_names != 0) {
    shellcommand = g_string_append (shellcommand, "--relative ");
    argv[i++] = "--relative";
  }
  
  run_settings.turn_off_relative = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_turn_off_relative));
  if(run_settings.turn_off_relative != 0) {
    shellcommand = g_string_append (shellcommand, "--no-relative ");
    argv[i++] = "--no-relative";
  }

  run_settings.dont_send_implied_dirs_with_relative = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_dont_send_implied_dirs_with_relative));
  if(run_settings.dont_send_implied_dirs_with_relative != 0) {
    shellcommand = g_string_append (shellcommand, "--no-implied-dirs ");
    argv[i++] = "--no-implied-dirs";
  }

  run_settings.transfer_directories_without_recursing = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_transfer_directories_without_recursing));
  if(run_settings.transfer_directories_without_recursing != 0) {
    shellcommand = g_string_append (shellcommand, "--dirs ");
    argv[i++] = "--dirs";
  }

  run_settings.preserve_permissions = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_preserve_permissions));
  if(run_settings.preserve_permissions != 0) {
    shellcommand = g_string_append (shellcommand, "--perms ");
    argv[i++] = "--perms";
  }

  run_settings.preserve_owner = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_preserve_owner));
  if(run_settings.preserve_owner != 0) {
    shellcommand = g_string_append (shellcommand, "--owner ");
    argv[i++] = "--owner";
  }

  run_settings.preserve_group = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_preserve_group));
  if(run_settings.preserve_group != 0) {
    shellcommand = g_string_append (shellcommand, "--group ");
    argv[i++] = "--group";
  }

  run_settings.preserve_devices = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_preserve_devices));
  if(run_settings.preserve_devices != 0) {
    shellcommand = g_string_append (shellcommand, "--devices ");
    argv[i++] = "--devices";
  }

  run_settings.preserve_times = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_preserve_times));
  if(run_settings.preserve_times != 0) {
    shellcommand = g_string_append (shellcommand, "--times ");
    argv[i++] = "--times";
  }

  run_settings.omit_directories_when_preserving_times = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_omit_directories_when_preserving_times));
  if(run_settings.omit_directories_when_preserving_times != 0) {
    shellcommand = g_string_append (shellcommand, "--omit-dir-times ");
    argv[i++] = "--omit-dir-times";
  }

  run_settings.update_destination_files_inplace = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_update_destination_files_inplace));
  if(run_settings.update_destination_files_inplace != 0) {
    shellcommand = g_string_append (shellcommand, "--inplace ");
    argv[i++] = "--inplace";
  }

  run_settings.only_update_files_that_already_exist = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_only_update_files_that_already_exist));
  if(run_settings.only_update_files_that_already_exist != 0) {
    shellcommand = g_string_append (shellcommand, "--existing ");
    argv[i++] = "--existing";
  }

  run_settings.dont_delete_more_then_num_files = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gui_settings.spinbutton_dont_delete_more_then_num_files));
  if(run_settings.dont_delete_more_then_num_files > 0) {
    gchar *dont_delete_more_then_num_files;  

    dont_delete_more_then_num_files = g_strdup_printf ("%d", run_settings.dont_delete_more_then_num_files); 

    shellcommand = g_string_append (shellcommand, "--max-delete=");
    shellcommand = g_string_append (shellcommand, dont_delete_more_then_num_files);
    shellcommand = g_string_append (shellcommand, " ");
  
    GString *max_delete = g_string_new("--max-delete=");
    max_delete = g_string_append (max_delete, dont_delete_more_then_num_files);
    argv[i++] = max_delete->str;

    g_free (dont_delete_more_then_num_files);
    dont_delete_more_then_num_files = NULL;
  }

  run_settings.sent_files_are_removed_from_sender = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_sent_files_are_removed_from_sender));
  if(run_settings.sent_files_are_removed_from_sender != 0) {
    shellcommand = g_string_append (shellcommand, "--remove-sent-files ");
    argv[i++] = "--remove-sent-files";
  }

  run_settings.delete_files_that_dont_exist_on_sender = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_delete_files_that_dont_exist_on_sender));
  if(run_settings.delete_files_that_dont_exist_on_sender != 0) {
    shellcommand = g_string_append (shellcommand, "--delete ");
    argv[i++] = "--delete";
  }

  run_settings.also_delete_excluded_files_on_receiver = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_also_delete_excluded_files_on_receiver));
  if(run_settings.also_delete_excluded_files_on_receiver != 0) {
    shellcommand = g_string_append (shellcommand, "--delete-excluded ");
    argv[i++] = "--delete-excluded";
  }
    
  // TODO: these are radiobuttons
  //gboolean receiver_deletes_before_transfer;
  //gboolean receiver_deletes_during_xfer;
  //gboolean receiver_deletes_after_transfer;
  //

  run_settings.delete_even_if_there_are_io_errors = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_delete_even_if_there_are_io_errors));
  if(run_settings.delete_even_if_there_are_io_errors != 0) {
    shellcommand = g_string_append (shellcommand, "--ignore-errors ");
    argv[i++] = "--ignore-errors";
  }

  run_settings.force_deletion_of_dirs_even_if_not_empty = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_force_deletion_of_dirs_even_if_not_empty));
  if(run_settings.force_deletion_of_dirs_even_if_not_empty != 0) {
    shellcommand = g_string_append (shellcommand, "--force ");
    argv[i++] = "--force";
  }

  run_settings.dont_skip_files_that_match_size_and_time = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_dont_skip_files_that_match_size_and_time));
  if(run_settings.dont_skip_files_that_match_size_and_time != 0) {
    shellcommand = g_string_append (shellcommand, "--ignore-times ");
    argv[i++] = "--ignore-times";
  }

  run_settings.skip_files_that_match_in_size = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_skip_files_that_match_in_size));
  if(run_settings.skip_files_that_match_in_size != 0) {
    shellcommand = g_string_append (shellcommand, "--size-only ");
    argv[i++] = "--size-only";
  }

  run_settings.dont_transfer_any_file_larger_than_size = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gui_settings.spinbutton_dont_transfer_any_file_larger_than_size));
  if(run_settings.dont_transfer_any_file_larger_than_size > 0) {
    gchar *dont_transfer_any_file_larger_than_size;  

    dont_transfer_any_file_larger_than_size = g_strdup_printf ("%d", run_settings.dont_transfer_any_file_larger_than_size); 

    shellcommand = g_string_append (shellcommand, "--max-size=");
    shellcommand = g_string_append (shellcommand, dont_transfer_any_file_larger_than_size);
    shellcommand = g_string_append (shellcommand, " ");

    GString *max_size = g_string_new("--max-size=");
    max_size = g_string_append (max_size, dont_transfer_any_file_larger_than_size);
    argv[i++] = max_size->str;

    g_free (dont_transfer_any_file_larger_than_size);
    dont_transfer_any_file_larger_than_size = NULL;
  }

  run_settings.compare_mod_times_with_reduced_accuracy = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gui_settings.spinbutton_compare_mod_times_with_reduced_accuracy));  
  if(run_settings.compare_mod_times_with_reduced_accuracy > 0) {
    gchar *compare_mod_times_with_reduced_accuracy;  

    compare_mod_times_with_reduced_accuracy = g_strdup_printf ("%d", run_settings.compare_mod_times_with_reduced_accuracy); 

    shellcommand = g_string_append (shellcommand, "--modify-window=");
    shellcommand = g_string_append (shellcommand, compare_mod_times_with_reduced_accuracy);
    shellcommand = g_string_append (shellcommand, " ");

    GString *modify_window = g_string_new("--modify-window=");
    modify_window = g_string_append (modify_window, compare_mod_times_with_reduced_accuracy);
    argv[i++] = modify_window->str;
  
    g_free (compare_mod_times_with_reduced_accuracy); 
    compare_mod_times_with_reduced_accuracy = NULL;
  }

  run_settings.skip_based_on_checksum = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_skip_based_on_checksum));
  if(run_settings.skip_based_on_checksum != 0) {
    shellcommand = g_string_append (shellcommand, "--checksum ");
    argv[i++] = "--checksum";
  }

  run_settings.skip_files_that_are_newer_on_the_receiver = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_skip_files_that_are_newer_on_the_receiver));
  if(run_settings.skip_files_that_are_newer_on_the_receiver != 0) {
    shellcommand = g_string_append (shellcommand, "--update ");
    argv[i++] = "--update";
  }

  run_settings.ignore_files_that_already_exist_on_receiver = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_ignore_files_that_already_exist_on_receiver));
  if(run_settings.ignore_files_that_already_exist_on_receiver != 0) {
    shellcommand = g_string_append (shellcommand, "--ignore-existing ");
    argv[i++] = "--ignore-existing";
  }

  run_settings.autoignore_files_in_the_same_way_cvs_does = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_autoignore_files_in_the_same_way_cvs_does));
  if(run_settings.autoignore_files_in_the_same_way_cvs_does != 0) {
    shellcommand = g_string_append (shellcommand, "--cvs-exclude ");
    argv[i++] = "--cvs-exclude";
  }

  run_settings.dont_cross_filesystem_boundaries = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_dont_cross_filesystem_boundaries));  
  if(run_settings.dont_cross_filesystem_boundaries != 0) {
    shellcommand = g_string_append (shellcommand, "--one-file-system ");
    argv[i++] = "--one-file-system";
  }

  run_settings.copy_files_whole = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_copy_files_whole));
  if(run_settings.copy_files_whole != 0) {
    shellcommand = g_string_append (shellcommand, "--whole-file ");
    argv[i++] = "--whole-file";
  }

  run_settings.always_use_incremental_rsync_algorithm = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_always_use_incremental_rsync_algorithm));
  if(run_settings.always_use_incremental_rsync_algorithm != 0) {
    shellcommand = g_string_append (shellcommand, "--no-whole-file ");
    argv[i++] = "--no-whole-file";
  }

  run_settings.put_all_updated_files_into_place_at_end = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_put_all_updated_files_into_place_at_end));
  if(run_settings.put_all_updated_files_into_place_at_end != 0) {
    shellcommand = g_string_append (shellcommand, "--delay-updates ");
    argv[i++] = "--delay-updates";
  }

  run_settings.dont_map_uid_gid_values_by_user_group_name = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_dont_map_uid_gid_values_by_user_group_name));  
  if(run_settings.dont_map_uid_gid_values_by_user_group_name != 0) {
    shellcommand = g_string_append (shellcommand, "--numeric-ids ");
    argv[i++] = "--numeric-ids";
  }

  run_settings.set_io_timeout_in_seconds = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gui_settings.spinbutton_set_io_timeout_in_seconds));
  if(run_settings.set_io_timeout_in_seconds > 0) {
    gchar *set_io_timeout_in_seconds;  

    set_io_timeout_in_seconds = g_strdup_printf ("%d", run_settings.set_io_timeout_in_seconds); 

    shellcommand = g_string_append (shellcommand, "--timeout=");
    shellcommand = g_string_append (shellcommand, set_io_timeout_in_seconds);
    shellcommand = g_string_append (shellcommand, " ");

    GString *timeout = g_string_new("--timeout=");
    timeout = g_string_append (timeout, set_io_timeout_in_seconds);
    argv[i++] = timeout->str;
  
    g_free (set_io_timeout_in_seconds); 
    set_io_timeout_in_seconds = NULL;
  }

  run_settings.find_similiar_file_for_basis_if_no_dest_file = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_find_similiar_file_for_basis_if_no_dest_file));
  if(run_settings.find_similiar_file_for_basis_if_no_dest_file != 0) {
    shellcommand = g_string_append (shellcommand, "--fuzzy ");
    argv[i++] = "--fuzzy";
  }

  run_settings.compress_file_data_during_the_transfer = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_compress_file_data_during_the_transfer));  
  if(run_settings.compress_file_data_during_the_transfer != 0) {
    shellcommand = g_string_append (shellcommand, "--compress ");
    argv[i++] = "--compress";
  }

  run_settings.keep_partially_transfered_files = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_keep_partially_transfered_files));
  run_settings.put_a_partially_transferred_files_into_dir = gtk_entry_get_text(GTK_ENTRY(gui_settings.entry_put_a_partially_transferred_files_into_dir));
  if (run_settings.keep_partially_transfered_files != 0) {
    shellcommand = g_string_append (shellcommand, "--partial ");
    argv[i++] = "--partial";
    if(strlen(run_settings.put_a_partially_transferred_files_into_dir) > 0) {
      shellcommand = g_string_append (shellcommand, "--partial-dir=");
      shellcommand = g_string_append (shellcommand, run_settings.put_a_partially_transferred_files_into_dir);
      shellcommand = g_string_append (shellcommand, " ");

      GString *partial_dir = g_string_new("--partial-dir=");
      partial_dir = g_string_append (partial_dir, run_settings.put_a_partially_transferred_files_into_dir);
      argv[i++] = partial_dir->str;
    }
  }

  run_settings.copy_symlinks_as_symlinks = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_copy_symlinks_as_symlinks));
  if(run_settings.copy_symlinks_as_symlinks != 0) {
    shellcommand = g_string_append (shellcommand, "--links ");
    argv[i++] = "--links";
  }

  run_settings.transform_symlink_into_referent_file_dir = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_transform_symlink_into_referent_file_dir));  
  if(run_settings.transform_symlink_into_referent_file_dir != 0) {
    shellcommand = g_string_append (shellcommand, "--copy-links ");
    argv[i++] = "--copy-links";
  }

  run_settings.only_unsafe_symlinks_are_transformed = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_only_unsafe_symlinks_are_transformed));
  if(run_settings.only_unsafe_symlinks_are_transformed != 0) {
    shellcommand = g_string_append (shellcommand, "--copy-unsafe-links ");
    argv[i++] = "--copy-unsafe-links";
  }

  run_settings.ignore_symlinks_that_point_outside_the_tree = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_ignore_symlinks_that_point_outside_the_tree));
  if(run_settings.ignore_symlinks_that_point_outside_the_tree != 0) {
    shellcommand = g_string_append (shellcommand, "--safe-links ");
    argv[i++] = "--safe-links";
  }

  run_settings.preserve_hardlinks = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_preserve_hardlinks));  
  if(run_settings.preserve_hardlinks != 0) {
    shellcommand = g_string_append (shellcommand, "--hard-links ");
    argv[i++] = "--hard-links";
  }

  run_settings.treat_symlinked_dir_on_receiver_as_dir = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_treat_symlinked_dir_on_receiver_as_dir));
  if(run_settings.treat_symlinked_dir_on_receiver_as_dir != 0) {
    shellcommand = g_string_append (shellcommand, "--keep-dirlinks ");
    argv[i++] = "--keep-dirlinks";
  }

  run_settings.force_a_fixed_checksum_blocksize = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gui_settings.spinbutton_force_a_fixed_checksum_blocksize));
  if(run_settings.force_a_fixed_checksum_blocksize > 0) {
    gchar *force_a_fixed_checksum_blocksize;  

    force_a_fixed_checksum_blocksize = g_strdup_printf ("%d", run_settings.force_a_fixed_checksum_blocksize); 

    shellcommand = g_string_append (shellcommand, "--block-size=");
    shellcommand = g_string_append (shellcommand, force_a_fixed_checksum_blocksize);
    shellcommand = g_string_append (shellcommand, " ");

    GString *block_size = g_string_new("--block-size=");
    block_size = g_string_append (block_size, force_a_fixed_checksum_blocksize);
    argv[i++] = block_size->str;

    g_free (force_a_fixed_checksum_blocksize); 
    force_a_fixed_checksum_blocksize = NULL;
  }

  run_settings.limit_io_bandwith_kbytes_per_second = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(gui_settings.spinbutton_limit_io_bandwith_kbytes_per_second));
  if(run_settings.limit_io_bandwith_kbytes_per_second > 0) {
    gchar *limit_io_bandwith_kbytes_per_second;  

    limit_io_bandwith_kbytes_per_second = g_strdup_printf ("%d", run_settings.limit_io_bandwith_kbytes_per_second); 

    shellcommand = g_string_append (shellcommand, "--bwlimit=");
    shellcommand = g_string_append (shellcommand, limit_io_bandwith_kbytes_per_second);
    shellcommand = g_string_append (shellcommand, " ");

    GString *bwlimit = g_string_new("--bwlimit=");
    bwlimit = g_string_append (bwlimit, limit_io_bandwith_kbytes_per_second);
    argv[i++] = bwlimit->str;

    g_free (limit_io_bandwith_kbytes_per_second); 
    limit_io_bandwith_kbytes_per_second = NULL;
  }

  run_settings.custom_args = gtk_entry_get_text(GTK_ENTRY(gui_settings.entry_custom_args));
  if (strlen(run_settings.custom_args) > 0) {
    shellcommand = g_string_append (shellcommand, run_settings.custom_args);
    shellcommand = g_string_append (shellcommand, " ");
    argv[i++] = run_settings.custom_args;
  }

  run_settings.specify_the_remote_shell_to_use = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_specify_the_remote_shell_to_use));
  run_settings.remote_shell = gtk_entry_get_text(GTK_ENTRY(gui_settings.entry_remote_shell));
  if (run_settings.sync_to_host != 0 && run_settings.specify_the_remote_shell_to_use != 0) {
    shellcommand = g_string_append (shellcommand, "--rsh=\"");
    shellcommand = g_string_append (shellcommand, run_settings.remote_shell);
    shellcommand = g_string_append (shellcommand, "\" ");

    GString *rsh = g_string_new("--rsh=\"");
    rsh = g_string_append (rsh, run_settings.remote_shell);
    rsh = g_string_append (rsh, "\"");
    argv[i++] = rsh->str;
  }

  run_settings.specify_the_rsync_to_run_on_remote_machine = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_specify_the_rsync_to_run_on_remote_machine));
  
  run_settings.remote_rsync = gtk_entry_get_text(GTK_ENTRY(gui_settings.entry_remote_rsync));
  if(run_settings.sync_to_host != 0 && run_settings.specify_the_rsync_to_run_on_remote_machine != 0) {
      shellcommand = g_string_append (shellcommand, "--rsync-path=\"");
      shellcommand = g_string_append (shellcommand, run_settings.remote_rsync);
    shellcommand = g_string_append (shellcommand, "\" ");
    
    GString *rsync_path = g_string_new("--rsync-path=\"");
      rsync_path = g_string_append (rsync_path, run_settings.remote_rsync);
      rsync_path = g_string_append (rsync_path, "\"");
      argv[i++] = rsync_path->str;
  }

  run_settings.sourcepath = gtk_entry_get_text (GTK_ENTRY (gui_settings.entry_sourcepath));
  shellcommand = g_string_append (shellcommand, run_settings.sourcepath);
  shellcommand = g_string_append (shellcommand, " ");
  argv[i++] = run_settings.sourcepath;
  
  run_settings.hostname = gtk_entry_get_text(GTK_ENTRY(gui_settings.entry_hostname));
  run_settings.username = gtk_entry_get_text(GTK_ENTRY(gui_settings.entry_username));
  run_settings.password = gtk_entry_get_text(GTK_ENTRY(gui_settings.entry_password));
  run_settings.destinationpath = gtk_entry_get_text (GTK_ENTRY (gui_settings.entry_destinationpath));
  run_settings.sync_to_host = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_sync_to_host));
  if(run_settings.sync_to_host != 0) {

    // check if username is empty and not add the @ below
    shellcommand = g_string_append (shellcommand, run_settings.username);
    
    // TODO: password missing!

    // TODO: remove "@" when no username is set
    shellcommand = g_string_append (shellcommand, "@");
    
    shellcommand = g_string_append (shellcommand, run_settings.hostname);
    shellcommand = g_string_append (shellcommand, ":");
    shellcommand = g_string_append (shellcommand, run_settings.destinationpath);

    GString *host = g_string_new(run_settings.username);
    host = g_string_append (host, run_settings.username);
    host = g_string_append (host, "@");
    host = g_string_append (host, run_settings.hostname);
    host = g_string_append (host, ":");
    host = g_string_append (host, run_settings.destinationpath);
    argv[i++] = host->str;

  } else {
    shellcommand = g_string_append (shellcommand, run_settings.destinationpath);
    shellcommand = g_string_append (shellcommand, " ");
    argv[i++] = run_settings.destinationpath;
  }  
  argv[i++] = NULL;

  return;
}

void 
on_toolbutton_sync_clicked (GtkWidget *widget, gpointer data)
{
  dry_run = FALSE;
  rsync2 ( widget, data );
  return;
}

void 
on_toolbutton_test_clicked (GtkWidget *widget, gpointer data)
{
  dry_run = TRUE;
  rsync2 ( widget, data );
  return;
}

void 
open_project_file (GtkWidget *widget) 
{
  
  /* TODO: this must be a dialog for opening a .zynk file */
  
    GtkWidget *dialog;
    
    dialog = gtk_file_chooser_dialog_new ("Open Zynk Profile",
                                  NULL,
                                  GTK_FILE_CHOOSER_ACTION_OPEN,
                                  GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                  GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
                                  NULL);
    gchar *projectfile;            
    if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT)
    {
        projectfile = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));      
    
    gboolean c = load_settings(projectfile, &run_settings);
    if (c == TRUE) {
      update_gui_settings (GTK_WIDGET (widget));
    }
    
    g_free(projectfile);
  }

  gtk_widget_destroy (dialog);

  return;
}

void 
update_gui_settings (GtkWidget *widget, gpointer data ) 
{
  get_widgets_from_xml (widget, data);

  gtk_entry_set_text(GTK_ENTRY(gui_settings.entry_sourcepath),
      run_settings.sourcepath);
  gtk_entry_set_text(GTK_ENTRY(gui_settings.entry_destinationpath),
      run_settings.destinationpath);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_increase_verbosity),
      run_settings.increase_verbosity);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_suppres_non_error_messages),
      run_settings.suppres_non_error_messages);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_recurse_into_directories),
      run_settings.recurse_into_directories);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_use_relative_path_names), 
      run_settings.use_relative_path_names);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_turn_off_relative), 
      run_settings.turn_off_relative);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_dont_send_implied_dirs_with_relative), 
      run_settings.dont_send_implied_dirs_with_relative);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_transfer_directories_without_recursing), 
      run_settings.transfer_directories_without_recursing);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_preserve_permissions), 
      run_settings.preserve_permissions);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_preserve_owner), 
      run_settings.preserve_owner);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_preserve_group), 
      run_settings.preserve_group);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_preserve_devices), 
      run_settings.preserve_devices);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_preserve_times), 
      run_settings.preserve_times);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_preserve_times), 
      run_settings.preserve_times);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_omit_directories_when_preserving_times), 
      run_settings.omit_directories_when_preserving_times);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_preserve_hardlinks), 
      run_settings.preserve_hardlinks);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_treat_symlinked_dir_on_receiver_as_dir), 
      run_settings.treat_symlinked_dir_on_receiver_as_dir);
  gtk_entry_set_text(GTK_ENTRY(gui_settings.entry_put_a_partially_transferred_files_into_dir), 
      run_settings.put_a_partially_transferred_files_into_dir);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(gui_settings.spinbutton_set_io_timeout_in_seconds), 
      run_settings.set_io_timeout_in_seconds);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(gui_settings.spinbutton_limit_io_bandwith_kbytes_per_second), 
      run_settings.limit_io_bandwith_kbytes_per_second);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_copy_files_whole), 
      run_settings.copy_files_whole);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_always_use_incremental_rsync_algorithm), 
      run_settings.always_use_incremental_rsync_algorithm);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_put_all_updated_files_into_place_at_end), 
      run_settings.put_all_updated_files_into_place_at_end);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_dont_map_uid_gid_values_by_user_group_name), 
      run_settings.dont_map_uid_gid_values_by_user_group_name);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_find_similiar_file_for_basis_if_no_dest_file), 
      run_settings.find_similiar_file_for_basis_if_no_dest_file);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_compress_file_data_during_the_transfer), 
      run_settings.compress_file_data_during_the_transfer);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_keep_partially_transfered_files), 
      run_settings.keep_partially_transfered_files);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_copy_symlinks_as_symlinks), 
      run_settings.copy_symlinks_as_symlinks);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_transform_symlink_into_referent_file_dir), 
      run_settings.transform_symlink_into_referent_file_dir);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_only_unsafe_symlinks_are_transformed), 
      run_settings.only_unsafe_symlinks_are_transformed);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_ignore_symlinks_that_point_outside_the_tree), 
      run_settings.ignore_symlinks_that_point_outside_the_tree);
  gtk_entry_set_text(GTK_ENTRY(gui_settings.entry_hostname), 
      run_settings.hostname);
  gtk_entry_set_text(GTK_ENTRY(gui_settings.entry_port), 
      run_settings.port);
  gtk_entry_set_text(GTK_ENTRY(gui_settings.entry_username), 
      run_settings.username);
  gtk_entry_set_text(GTK_ENTRY(gui_settings.entry_password), 
      run_settings.password);
  gtk_entry_set_text(GTK_ENTRY(gui_settings.entry_remote_shell), 
      run_settings.remote_shell);
  gtk_entry_set_text(GTK_ENTRY(gui_settings.entry_remote_rsync), 
      run_settings.remote_rsync);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_sync_to_host), 
      run_settings.sync_to_host);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_specify_the_remote_shell_to_use), 
      run_settings.specify_the_remote_shell_to_use);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_specify_the_rsync_to_run_on_remote_machine), 
      run_settings.specify_the_rsync_to_run_on_remote_machine);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(gui_settings.spinbutton_dont_delete_more_then_num_files), 
      run_settings.dont_delete_more_then_num_files);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_update_destination_files_inplace), 
      run_settings.update_destination_files_inplace);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_only_update_files_that_already_exist), 
      run_settings.only_update_files_that_already_exist);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_sent_files_are_removed_from_sender), 
      run_settings.sent_files_are_removed_from_sender);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_delete_files_that_dont_exist_on_sender), 
      run_settings.delete_files_that_dont_exist_on_sender);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_also_delete_excluded_files_on_receiver), 
      run_settings.also_delete_excluded_files_on_receiver);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_delete_even_if_there_are_io_errors), 
      run_settings.delete_even_if_there_are_io_errors);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_force_deletion_of_dirs_even_if_not_empty), 
      run_settings.force_deletion_of_dirs_even_if_not_empty);
  //radiobutton_receiver_deletes_before_transfer
  //radiobutton_receiver_deletes_during_xfer
  //radiobutton_receiver_deletes_after_transfer
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(gui_settings.spinbutton_force_a_fixed_checksum_blocksize), 
      run_settings.force_a_fixed_checksum_blocksize);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(gui_settings.spinbutton_compare_mod_times_with_reduced_accuracy), 
      run_settings.compare_mod_times_with_reduced_accuracy);
  gtk_spin_button_set_value(GTK_SPIN_BUTTON(gui_settings.spinbutton_dont_transfer_any_file_larger_than_size), 
      run_settings.dont_transfer_any_file_larger_than_size);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_dont_skip_files_that_match_size_and_time), 
      run_settings.dont_skip_files_that_match_size_and_time);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_skip_files_that_match_in_size), 
      run_settings.skip_files_that_match_in_size);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_skip_based_on_checksum), 
      run_settings.skip_based_on_checksum);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_skip_files_that_are_newer_on_the_receiver), 
      run_settings.skip_files_that_are_newer_on_the_receiver);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_ignore_files_that_already_exist_on_receiver), 
      run_settings.ignore_files_that_already_exist_on_receiver);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_autoignore_files_in_the_same_way_cvs_does), 
      run_settings.autoignore_files_in_the_same_way_cvs_does);
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(gui_settings.checkbutton_dont_cross_filesystem_boundaries), 
      run_settings.dont_cross_filesystem_boundaries);
  gtk_entry_set_text (GTK_ENTRY( gui_settings.entry_create_temporary_files_in_dir), 
      run_settings.create_temporary_files_in_dir);
  gtk_entry_set_text (GTK_ENTRY (gui_settings.entry_custom_args), 
      run_settings.custom_args);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (gui_settings.checkbutton_archive_mode), 
      run_settings.archive_mode);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (gui_settings.checkbutton_handle_sparse_files_effeciently), 
    run_settings.handle_sparse_files_effeciently);
  //radiobutton_prefer_ipv4
  //radiobutton_prefer_ipv6
  gtk_entry_set_text(GTK_ENTRY(gui_settings.entry_custom_args), 
      run_settings.custom_args);

  return;
}

/* Original Source: grsync */
gboolean
out_watch (GIOChannel *source, GIOCondition condition, gpointer data)
{
  GtkTextBuffer *buffer;
  GtkTextIter iter, iter2;
   GString *str;
  static gchar *carriage = NULL;
  
  buffer = gtk_text_view_get_buffer((GtkTextView*)data);

  if (carriage != NULL) {
    gtk_text_buffer_get_iter_at_line(buffer, &iter, gtk_text_buffer_get_line_count(buffer) - 2);
    gtk_text_buffer_get_end_iter(buffer, &iter2);
    gtk_text_buffer_delete(buffer, &iter, &iter2);
  }
  
  str = g_string_new("");  
  g_io_channel_read_line_string(source, str, NULL, NULL);
  carriage = strchr(str->str, '\r');
  
  gtk_text_buffer_insert_at_cursor(buffer, (gchar*)str->str, -1);
  gtk_text_buffer_get_end_iter(buffer, &iter);
  gtk_text_view_scroll_to_iter((GtkTextView*)data, &iter, 0, TRUE, 0.0, 1.0);
  g_string_free (str, TRUE);

  if (condition == G_IO_HUP) {
     g_io_channel_shutdown (source, FALSE, NULL);
     g_io_channel_unref (source);
    //gtk_button_set_label((GtkButton*)lookup_widget((GtkWidget*) data, "close"), "gtk-close");
    //gtk_button_set_use_stock((GtkButton*)lookup_widget((GtkWidget*) data, "close"), TRUE);
    g_spawn_close_pid(rsync_pid);
    rsync_pid = 0;
    g_free(argv);
    return FALSE;
  }
  return TRUE;
}

/* Original Source: grsync */
gboolean 
err_watch (GIOChannel *source, GIOCondition condition, gpointer data)
{
  GtkTextBuffer *buffer;
  GtkTextIter start, end, iter;
   GString *str;
  
  buffer = gtk_text_view_get_buffer ((GtkTextView*) data);
  
  str = g_string_new ("");  
  
  g_io_channel_read_line_string (source, str, NULL, NULL);

    gtk_text_buffer_get_end_iter (buffer, &iter);
    gtk_text_buffer_insert_with_tags_by_name (buffer, &iter, (gchar*)str->str, -1, "fore-red", NULL);
  
    gtk_text_buffer_get_end_iter (buffer, &iter);
    gtk_text_view_scroll_to_iter ((GtkTextView*)data, &iter, 0, TRUE, 0.0, 1.0);

  g_string_free (str, TRUE);

  if (condition == G_IO_HUP) {
     g_io_channel_shutdown (source, FALSE, NULL);
     g_io_channel_unref (source);
    return FALSE;
  }
  return TRUE;
}

/* Original Source: grsync */
void
rsync2 (GtkWidget *widget, gpointer data)
{
  gint out, err;
  GtkTextBuffer *buffer;
  GtkTextView *view;
  GIOChannel *chout, *cherr;
  GtkTextTag *tag;
  rsync_pid = 0;
  
  GladeXML *xml;

  GtkTextBuffer *command_buffer;
  gchar *xbuffer;

  update_run_settings(widget, data);
  
  command_buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (gui_settings.command));
  
  xbuffer = shellcommand->str;
  gtk_text_buffer_set_text (command_buffer, xbuffer, -1);
  
  if (g_spawn_async_with_pipes (NULL, argv, NULL, G_SPAWN_SEARCH_PATH, NULL, NULL, &rsync_pid, NULL, &out, &err, NULL)) {
     chout = g_io_channel_unix_new (out);
     cherr = g_io_channel_unix_new (err);
    
    xml = glade_get_widget_tree (GTK_WIDGET(widget));
    gui_settings.commandoutput = glade_xml_get_widget (xml, "textview_log");

    view = (GtkTextView*) gui_settings.commandoutput;
    buffer = gtk_text_view_get_buffer(view);
    tag = gtk_text_buffer_create_tag (buffer, "fore-red", "foreground", "red", NULL);
    g_io_add_watch_full(chout, G_PRIORITY_DEFAULT_IDLE, G_IO_IN | G_IO_HUP, out_watch, view, NULL);
    g_io_add_watch_full(cherr, G_PRIORITY_DEFAULT_IDLE, G_IO_IN | G_IO_HUP, err_watch, view, NULL);
  }
  return;
}
