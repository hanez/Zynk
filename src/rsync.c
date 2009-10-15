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

#include "settings.h"

GString *build_shellcommand (ZConf *run_settings) {

    GString *shellcommand;
    
    shellcommand = g_string_new ("rsync ");
	
	if (run_settings->recurse_into_directories != 0)
		shellcommand = g_string_append (shellcommand, "--recursive ");
	
	if (run_settings->suppres_non_error_messages != 0)
		shellcommand = g_string_append (shellcommand, "--quiet ");
	
	if(run_settings->increase_verbosity != 0)
		shellcommand = g_string_append (shellcommand, "--verbose ");
		
	if(run_settings->use_relative_path_names != 0)
		shellcommand = g_string_append (shellcommand, "--relative ");
	
	if(run_settings->turn_off_relative != 0)
		shellcommand = g_string_append (shellcommand, "--no-relative ");
	
	if(run_settings->dont_send_implied_dirs_with_relative != 0)
		shellcommand = g_string_append (shellcommand, "--no-implied-dirs ");
		
	if(run_settings->transfer_directories_without_recursing != 0)
		shellcommand = g_string_append (shellcommand, "--dirs ");
	
	if(run_settings->preserve_permissions != 0)
		shellcommand = g_string_append (shellcommand, "--perms ");
		
	if(run_settings->preserve_owner != 0)
		shellcommand = g_string_append (shellcommand, "--owner ");
		
	if(run_settings->preserve_group != 0)
		shellcommand = g_string_append (shellcommand, "--group ");
	
	if(run_settings->preserve_devices != 0)
		shellcommand = g_string_append (shellcommand, "--devices ");
		
	if(run_settings->preserve_times != 0)
		shellcommand = g_string_append (shellcommand, "--times ");
		
	if(run_settings->omit_directories_when_preserving_times != 0)
		shellcommand = g_string_append (shellcommand, "--omit-dir-times ");
	
	if(run_settings->update_destination_files_inplace != 0)
		shellcommand = g_string_append (shellcommand, "--inplace ");
		
	if(run_settings->only_update_files_that_already_exist != 0)
		shellcommand = g_string_append (shellcommand, "--existing ");

	/* TODO: get the value as integer and compare the size with ">" not the strlen*/	
    if(run_settings->dont_delete_more_then_num_files > 0) {
        gchar *dont_delete_more_then_num_files;  
    	
    	dont_delete_more_then_num_files = g_strdup_printf ("%d", run_settings->dont_delete_more_then_num_files); 
        
        shellcommand = g_string_append (shellcommand, "--max-delete=");
        shellcommand = g_string_append (shellcommand, dont_delete_more_then_num_files);
        shellcommand = g_string_append (shellcommand, " ");
        
        g_free (dont_delete_more_then_num_files);
	    dont_delete_more_then_num_files = NULL;
    }

	if(run_settings->sent_files_are_removed_from_sender != 0)
		shellcommand = g_string_append (shellcommand, "--remove-sent-files ");
		
	if(run_settings->delete_files_that_dont_exist_on_sender != 0)
		shellcommand = g_string_append (shellcommand, "--delete ");
		
	if(run_settings->also_delete_excluded_files_on_receiver != 0)
		shellcommand = g_string_append (shellcommand, "--delete-excluded ");
    
    /* TODO: these are radiobuttons
    gboolean receiver_deletes_before_transfer;
	gboolean receiver_deletes_during_xfer;
	gboolean receiver_deletes_after_transfer;
	*/
	
	if(run_settings->delete_even_if_there_are_io_errors != 0)
		shellcommand = g_string_append (shellcommand, "--ignore-errors ");
		
	if(run_settings->force_deletion_of_dirs_even_if_not_empty != 0)
		shellcommand = g_string_append (shellcommand, "--force ");
	
	if(run_settings->dont_skip_files_that_match_size_and_time != 0)
		shellcommand = g_string_append (shellcommand, "--ignore-times ");
		
	if(run_settings->skip_files_that_match_in_size != 0)
		shellcommand = g_string_append (shellcommand, "--size-only ");

    /* TODO: get the value as integer and compare the size with ">" not the strlen*/	
	if(run_settings->dont_transfer_any_file_larger_than_size > 0) {
    	gchar *dont_transfer_any_file_larger_than_size;  
    	
    	dont_transfer_any_file_larger_than_size = g_strdup_printf ("%d", run_settings->dont_transfer_any_file_larger_than_size); 
    	
        shellcommand = g_string_append (shellcommand, "--max-size=");
		shellcommand = g_string_append (shellcommand, dont_transfer_any_file_larger_than_size);
		shellcommand = g_string_append (shellcommand, " ");
		
		g_free (dont_transfer_any_file_larger_than_size);
	    dont_transfer_any_file_larger_than_size = NULL;
	}

	if(run_settings->compare_mod_times_with_reduced_accuracy > 0) {
    	gchar *compare_mod_times_with_reduced_accuracy;  
    	
    	compare_mod_times_with_reduced_accuracy = g_strdup_printf ("%d", run_settings->compare_mod_times_with_reduced_accuracy); 
    	
        shellcommand = g_string_append (shellcommand, "--modify-window=");
		shellcommand = g_string_append (shellcommand, compare_mod_times_with_reduced_accuracy);
		shellcommand = g_string_append (shellcommand, " ");
		
		g_free (compare_mod_times_with_reduced_accuracy); 
	    compare_mod_times_with_reduced_accuracy = NULL;
	}

	if(run_settings->skip_based_on_checksum != 0)
		shellcommand = g_string_append (shellcommand, "--checksum ");
		
	if(run_settings->skip_files_that_are_newer_on_the_receiver != 0)
		shellcommand = g_string_append (shellcommand, "--update ");
		
	if(run_settings->ignore_files_that_already_exist_on_receiver != 0)
		shellcommand = g_string_append (shellcommand, "--ignore-existing ");
		
	if(run_settings->autoignore_files_in_the_same_way_cvs_does != 0)
		shellcommand = g_string_append (shellcommand, "--cvs-exclude ");
		
	if(run_settings->dont_cross_filesystem_boundaries != 0)
		shellcommand = g_string_append (shellcommand, "--one-file-system ");
		
	if(run_settings->copy_files_whole != 0)
		shellcommand = g_string_append (shellcommand, "--whole-file ");
		
	if(run_settings->always_use_incremental_rsync_algorithm != 0)
		shellcommand = g_string_append (shellcommand, "--no-whole-file ");

	if(run_settings->put_all_updated_files_into_place_at_end != 0)
		shellcommand = g_string_append (shellcommand, "--delay-updates ");
		
	if(run_settings->dont_map_uid_gid_values_by_user_group_name != 0)
		shellcommand = g_string_append (shellcommand, "--numeric-ids ");

	/* TODO: get the value as integer and compare the size with ">" not the strlen*/
	if(run_settings->set_io_timeout_in_seconds > 0) {
	    gchar *set_io_timeout_in_seconds;  
    	
    	set_io_timeout_in_seconds = g_strdup_printf ("%d", run_settings->set_io_timeout_in_seconds); 
    	
   	    shellcommand = g_string_append (shellcommand, "--timeout=");
		shellcommand = g_string_append (shellcommand, set_io_timeout_in_seconds);
		shellcommand = g_string_append (shellcommand, " ");
		
		g_free (set_io_timeout_in_seconds); 
	    set_io_timeout_in_seconds = NULL;
	}

	if(run_settings->find_similiar_file_for_basis_if_no_dest_file != 0)
		shellcommand = g_string_append (shellcommand, "--fuzzy ");
		
	if(run_settings->compress_file_data_during_the_transfer != 0)
		shellcommand = g_string_append (shellcommand, "--compress ");

	if(run_settings->keep_partially_transfered_files != 0)
		shellcommand = g_string_append (shellcommand, "--partial ");
    
    /* TODO: get the value as integer and compare the size with ">" not the strlen*/
    if(strlen(run_settings->put_a_partially_transferred_files_into_dir) > 0) {
        shellcommand = g_string_append (shellcommand, "--partial-dir=");
    	shellcommand = g_string_append (shellcommand, run_settings->put_a_partially_transferred_files_into_dir);
    	shellcommand = g_string_append (shellcommand, " ");
    }
    
	if(run_settings->copy_symlinks_as_symlinks != 0)
		shellcommand = g_string_append (shellcommand, "--links ");

	if(run_settings->transform_symlink_into_referent_file_dir != 0)
		shellcommand = g_string_append (shellcommand, "--copy-links ");
    
	if(run_settings->only_unsafe_symlinks_are_transformed != 0)
		shellcommand = g_string_append (shellcommand, "--copy-unsafe-links ");
		
	if(run_settings->ignore_symlinks_that_point_outside_the_tree != 0)
		shellcommand = g_string_append (shellcommand, "--safe-links ");
		
	if(run_settings->preserve_hardlinks != 0)
		shellcommand = g_string_append (shellcommand, "--hard-links ");
		
	if(run_settings->treat_symlinked_dir_on_receiver_as_dir != 0)
		shellcommand = g_string_append (shellcommand, "--keep-dirlinks ");

    if(run_settings->force_a_fixed_checksum_blocksize > 0) {
        gchar *force_a_fixed_checksum_blocksize;  
    	
    	force_a_fixed_checksum_blocksize = g_strdup_printf ("%d", run_settings->force_a_fixed_checksum_blocksize); 
        
        shellcommand = g_string_append (shellcommand, "--block-size=");
		shellcommand = g_string_append (shellcommand, force_a_fixed_checksum_blocksize);
		shellcommand = g_string_append (shellcommand, " ");
		
		g_free (force_a_fixed_checksum_blocksize); 
	    force_a_fixed_checksum_blocksize = NULL;
	}

    if(run_settings->limit_io_bandwith_kbytes_per_second > 0) {
   	    gchar *limit_io_bandwith_kbytes_per_second;  
    	
    	limit_io_bandwith_kbytes_per_second = g_strdup_printf ("%d", run_settings->limit_io_bandwith_kbytes_per_second); 
    	
        shellcommand = g_string_append (shellcommand, "--bwlimit=");
		shellcommand = g_string_append (shellcommand, limit_io_bandwith_kbytes_per_second);
		shellcommand = g_string_append (shellcommand, " ");
		
		g_free (limit_io_bandwith_kbytes_per_second); 
	    limit_io_bandwith_kbytes_per_second = NULL;
    }

    shellcommand = g_string_append (shellcommand, run_settings->custom_args);
    shellcommand = g_string_append (shellcommand, " ");

	if(run_settings->sync_to_host != 0 && run_settings->specify_the_remote_shell_to_use != 0) {
	    shellcommand = g_string_append (shellcommand, "--rsh=\"");
	    shellcommand = g_string_append (shellcommand, run_settings->remote_shell);
		shellcommand = g_string_append (shellcommand, "\" ");
	}
	
	if(run_settings->sync_to_host != 0 && run_settings->specify_the_rsync_to_run_on_remote_machine != 0) {
	    shellcommand = g_string_append (shellcommand, "--rsync-path=\"");
	    shellcommand = g_string_append (shellcommand, run_settings->remote_rsync);
		shellcommand = g_string_append (shellcommand, "\" ");
	}

	shellcommand = g_string_append (shellcommand, run_settings->sourcepath);
	shellcommand = g_string_append (shellcommand, " ");
	
	if(run_settings->sync_to_host != 0) {

		/* check if username is empty and not add the @ below */
		shellcommand = g_string_append (shellcommand, run_settings->username);
		
		/* TODO: password missing! */

		/* TODO: remove "@" when no username is set */
		shellcommand = g_string_append (shellcommand, "@");
		
		shellcommand = g_string_append (shellcommand, run_settings->hostname);
		shellcommand = g_string_append (shellcommand, ":");

	}
	
	shellcommand = g_string_append (shellcommand, run_settings->destinationpath);
	shellcommand = g_string_append (shellcommand, " ");
	
	return shellcommand;
}

gchar *rsync (GString *shellcommand) {

  FILE *read_fp;
  char buffer[BUFSIZ +1];
  int chars_read;
  memset (buffer, '\0', sizeof(buffer));

	//shellcommand = g_string_new ("ls -l /usr");

	shellcommand = g_string_append (shellcommand, " 2>&1");
	
	g_print(shellcommand->str);

	read_fp = popen(shellcommand->str, "r");

	g_string_free(shellcommand, TRUE);
	
    if (read_fp != NULL) {
        chars_read = fread (buffer, sizeof(char), BUFSIZ, read_fp);
        pclose (read_fp);
    }
	return &buffer;
}
