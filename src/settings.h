/******************************************************************************
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
 ******************************************************************************/
 
typedef struct _ZConf {
  gchar *sourcepath;
  gchar *destinationpath;
  gboolean increase_verbosity;
  gboolean suppres_non_error_messages;
  gboolean recurse_into_directories;
  gboolean use_relative_path_names;
  gboolean turn_off_relative;
  gboolean dont_send_implied_dirs_with_relative;
  gboolean transfer_directories_without_recursing;
  gboolean preserve_permissions;
  gboolean preserve_owner;
  gboolean preserve_group;
  gboolean preserve_devices;
  gboolean preserve_times;
  gboolean omit_directories_when_preserving_times;
  gint dont_delete_more_then_num_files;
  gboolean sent_files_are_removed_from_sender;
  gboolean delete_files_that_dont_exist_on_sender;
  gboolean receiver_deletes_before_transfer;
  gboolean receiver_deletes_during_xfer;
  gboolean receiver_deletes_after_transfer;
  gboolean also_delete_excluded_files_on_receiver;
  gboolean delete_even_if_there_are_io_errors;
  gboolean force_deletion_of_dirs_even_if_not_empty;
  gboolean update_destination_files_inplace;
  gboolean only_update_files_that_already_exist;
  gboolean put_all_updated_files_into_place_at_end;
  gint dont_transfer_any_file_larger_than_size;
  gint force_a_fixed_checksum_blocksize;
  gint compare_mod_times_with_reduced_accuracy;
  gboolean dont_skip_files_that_match_size_and_time;
  gboolean skip_files_that_match_in_size;
  gboolean skip_based_on_checksum;
  gboolean skip_files_that_are_newer_on_the_receiver;
  gboolean ignore_files_that_already_exist_on_receiver;
  gboolean autoignore_files_in_the_same_way_cvs_does;
  gboolean dont_cross_filesystem_boundaries;
  gchar *put_a_partially_transferred_files_into_dir;
  gint set_io_timeout_in_seconds;
  gint limit_io_bandwith_kbytes_per_second;
  gboolean copy_files_whole;
  gboolean always_use_incremental_rsync_algorithm;
  gboolean dont_map_uid_gid_values_by_user_group_name;
  gboolean find_similiar_file_for_basis_if_no_dest_file;
  gboolean compress_file_data_during_the_transfer;
  gboolean keep_partially_transfered_files;
  gboolean copy_symlinks_as_symlinks;
  gboolean transform_symlink_into_referent_file_dir; 
  gboolean only_unsafe_symlinks_are_transformed;
  gboolean ignore_symlinks_that_point_outside_the_tree;
  gboolean preserve_hardlinks;
  gboolean treat_symlinked_dir_on_receiver_as_dir;
  gchar *hostname;
  gchar *port;
  gchar *username;
  gchar *password;
  gchar *remote_shell;
  gchar *remote_rsync;
  gboolean sync_to_host;
  gboolean specify_the_remote_shell_to_use;
  gboolean specify_the_rsync_to_run_on_remote_machine;
  gchar *create_temporary_files_in_dir;
  gchar *custom_args;
  gboolean archive_mode;
  gboolean handle_sparse_files_effeciently;
  gboolean prefer_ipv4;
  gboolean prefer_ipv6;
} ZConf;

typedef struct _ZGui {
  GtkWidget *command;
  GtkWidget *commandoutput;
  GtkWidget *entry_sourcepath;
  GtkWidget *entry_destinationpath;
  GtkWidget *checkbutton_increase_verbosity;
  GtkWidget *checkbutton_suppres_non_error_messages;
  GtkWidget *checkbutton_recurse_into_directories;
  GtkWidget *checkbutton_use_relative_path_names;
  GtkWidget *checkbutton_turn_off_relative;
  GtkWidget *checkbutton_dont_send_implied_dirs_with_relative;
  GtkWidget *checkbutton_transfer_directories_without_recursing;
  GtkWidget *checkbutton_preserve_permissions;
  GtkWidget *checkbutton_preserve_owner;
  GtkWidget *checkbutton_preserve_group;
  GtkWidget *checkbutton_preserve_devices;
  GtkWidget *checkbutton_preserve_times;
  GtkWidget *checkbutton_omit_directories_when_preserving_times;
  GtkWidget *entry_put_a_partially_transferred_files_into_dir;
  GtkWidget *spinbutton_set_io_timeout_in_seconds;
  GtkWidget *spinbutton_limit_io_bandwith_kbytes_per_second;
  GtkWidget *checkbutton_copy_files_whole;
  GtkWidget *checkbutton_always_use_incremental_rsync_algorithm;
  GtkWidget *checkbutton_put_all_updated_files_into_place_at_end;
  GtkWidget *checkbutton_dont_map_uid_gid_values_by_user_group_name;
  GtkWidget *checkbutton_find_similiar_file_for_basis_if_no_dest_file;
  GtkWidget *checkbutton_compress_file_data_during_the_transfer;
  GtkWidget *checkbutton_keep_partially_transfered_files;
  GtkWidget *checkbutton_copy_symlinks_as_symlinks;
  GtkWidget *checkbutton_transform_symlink_into_referent_file_dir; 
  GtkWidget *checkbutton_only_unsafe_symlinks_are_transformed;
  GtkWidget *checkbutton_ignore_symlinks_that_point_outside_the_tree;
  GtkWidget *checkbutton_preserve_hardlinks;
  GtkWidget *checkbutton_treat_symlinked_dir_on_receiver_as_dir;
  GtkWidget *entry_hostname;
  GtkWidget *entry_port;
  GtkWidget *entry_username;
  GtkWidget *entry_password;
  GtkWidget *entry_remote_shell;
  GtkWidget *entry_remote_rsync;
  GtkWidget *checkbutton_sync_to_host;
  GtkWidget *checkbutton_specify_the_remote_shell_to_use;
  GtkWidget *checkbutton_specify_the_rsync_to_run_on_remote_machine;
  GtkWidget *spinbutton_dont_delete_more_then_num_files;
  GtkWidget *checkbutton_sent_files_are_removed_from_sender;
  GtkWidget *checkbutton_delete_files_that_dont_exist_on_sender;
  GtkWidget *checkbutton_also_delete_excluded_files_on_receiver;
  GtkWidget *checkbutton_delete_even_if_there_are_io_errors;
  GtkWidget *checkbutton_force_deletion_of_dirs_even_if_not_empty;
  GtkWidget *radiobutton_receiver_deletes_before_transfer;
  GtkWidget *radiobutton_receiver_deletes_during_xfer;
  GtkWidget *radiobutton_receiver_deletes_after_transfer;
  GtkWidget *checkbutton_update_destination_files_inplace;
  GtkWidget *checkbutton_only_update_files_that_already_exist;
  GtkWidget *spinbutton_dont_transfer_any_file_larger_than_size;
  GtkWidget *spinbutton_compare_mod_times_with_reduced_accuracy;
  GtkWidget *spinbutton_force_a_fixed_checksum_blocksize;
  GtkWidget *checkbutton_dont_skip_files_that_match_size_and_time;
  GtkWidget *checkbutton_skip_files_that_match_in_size;
  GtkWidget *checkbutton_skip_based_on_checksum;
  GtkWidget *checkbutton_skip_files_that_are_newer_on_the_receiver;
  GtkWidget *checkbutton_ignore_files_that_already_exist_on_receiver;
  GtkWidget *checkbutton_autoignore_files_in_the_same_way_cvs_does;
  GtkWidget *checkbutton_dont_cross_filesystem_boundaries;
  GtkWidget *entry_create_temporary_files_in_dir;
  GtkWidget *entry_custom_args;
  GtkWidget *checkbutton_archive_mode;
  GtkWidget *checkbutton_handle_sparse_files_effeciently;
  GtkWidget *radiobutton_prefer_ipv4;
  GtkWidget *radiobutton_prefer_ipv6;
} ZGui;

gboolean load_settings(const char *filename, ZConf *run_settings);

gboolean write_settings(const char *filename, ZConf *run_settings);
