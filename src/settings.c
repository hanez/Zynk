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
 
#if HAVE_CONFIG_H
#  include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <string.h>

#include <gnome.h>

#include <libxml/parser.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include "settings.h"

#define MY_ENCODING "ISO-8859-1"

gboolean
xml_bool (const xmlChar *key) 
{
	if (!xmlStrcmp(key, (const xmlChar *)"TRUE") || !xmlStrcmp(key, (const xmlChar *)"True") || !xmlStrcmp(key, (const xmlChar *)"true"))
		return TRUE;
	
	return FALSE;
}

/**
 * ConvertInput:
 * @in: string in a given encoding
 * @encoding: the encoding used
 *
 * Converts @in into UTF-8 for processing with libxml2 APIs
 *
 * Returns the converted UTF-8 string, or NULL in case of error.
 */
xmlChar *
ConvertInput(const char *in, const char *encoding)
{
    xmlChar *out;
    int ret;
    int size;
    int out_size;
    int temp;
    xmlCharEncodingHandlerPtr handler;

    if (in == 0)
        return 0;

    handler = xmlFindCharEncodingHandler(encoding);

    if (!handler) {
        printf("ConvertInput: no encoding handler found for '%s'\n",
               encoding ? encoding : "");
        return 0;
    }

    size = (int) strlen(in) + 1;
    out_size = size * 2 - 1;
    out = (unsigned char *) xmlMalloc((size_t) out_size);

    if (out != 0) {
        temp = size - 1;
        ret = handler->input(out, &out_size, (const xmlChar *) in, &temp);
        if ((ret < 0) || (temp - size + 1)) {
            if (ret < 0) {
                printf("ConvertInput: conversion wasn't successful.\n");
            } else {
                printf
                    ("ConvertInput: conversion wasn't successful. converted: %i octets.\n",
                     temp);
            }

            xmlFree(out);
            out = 0;
        } else {
            out = (unsigned char *) xmlRealloc(out, out_size + 1);
            out[out_size] = 0;  /*null terminating out */
        }
    } else {
        printf("ConvertInput: no mem\n");
    }

    return out;
}

int
parse_settings (xmlDocPtr doc, xmlNodePtr cur, ZConf *run_settings)
{	
	xmlChar *key;
	cur = cur->xmlChildrenNode;
	while (cur != NULL) {

	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"sourcepath"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);	
			run_settings->sourcepath = key;
			//xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"destinationpath"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);	
			run_settings->destinationpath = key;
			//xmlFree(key);
 	    }
 	    
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"increase_verbosity"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->increase_verbosity = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"suppres_non_error_messages"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->suppres_non_error_messages = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"recurse_into_directories"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->recurse_into_directories = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"use_relative_path_names"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->use_relative_path_names = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"turn_off_relative"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->turn_off_relative = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"dont_send_implied_dirs_with_relative"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->dont_send_implied_dirs_with_relative = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"transfer_directories_without_recursing"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->transfer_directories_without_recursing = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"preserve_permissions"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->preserve_permissions = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"preserve_owner"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->preserve_owner = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"preserve_group"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->preserve_group = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"preserve_devices"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->preserve_devices = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"preserve_times"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->preserve_times = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"omit_directories_when_preserving_times"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->omit_directories_when_preserving_times = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
        if ((!xmlStrcmp(cur->name, (const xmlChar *)"dont_delete_more_then_num_files"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);	
			run_settings->dont_delete_more_then_num_files = key;
			//xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"update_destination_files_inplace"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->update_destination_files_inplace = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"only_update_files_that_already_exist"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->only_update_files_that_already_exist = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"sent_files_are_removed_from_sender"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->sent_files_are_removed_from_sender = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }

 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"delete_files_that_dont_exist_on_sender"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->delete_files_that_dont_exist_on_sender = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }

   	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"receiver_deletes_before_transfer"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->receiver_deletes_before_transfer = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"receiver_deletes_during_xfer"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->receiver_deletes_during_xfer = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"receiver_deletes_after_transfer"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->receiver_deletes_after_transfer = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"also_delete_excluded_files_on_receiver"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->also_delete_excluded_files_on_receiver = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"delete_even_if_there_are_io_errors"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->delete_even_if_there_are_io_errors = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"force_deletion_of_dirs_even_if_not_empty"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->force_deletion_of_dirs_even_if_not_empty = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }

        if ((!xmlStrcmp(cur->name, (const xmlChar *)"dont_transfer_any_file_larger_than_size"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);	
			run_settings->dont_transfer_any_file_larger_than_size = key;
			//xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"dont_skip_files_that_match_size_and_time"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->dont_skip_files_that_match_size_and_time = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"skip_files_that_match_in_size"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->skip_files_that_match_in_size = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"skip_based_on_checksum"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->skip_based_on_checksum = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"skip_files_that_are_newer_on_the_receiver"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->skip_files_that_are_newer_on_the_receiver = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"ignore_files_that_already_exist_on_receiver"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->ignore_files_that_already_exist_on_receiver = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"autoignore_files_in_the_same_way_cvs_does"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->autoignore_files_in_the_same_way_cvs_does = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"dont_cross_filesystem_boundaries"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->dont_cross_filesystem_boundaries = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"set_io_timeout_in_seconds"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);	
			run_settings->set_io_timeout_in_seconds = key;
			//xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"put_a_partially_transferred_files_into_dir"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);	
			run_settings->put_a_partially_transferred_files_into_dir = key;
			//xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"limit_io_bandwith_kbytes_per_second"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);	
			run_settings->limit_io_bandwith_kbytes_per_second = key;
			//xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"copy_files_whole"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->copy_files_whole = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"always_use_incremental_rsync_algorithm"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->always_use_incremental_rsync_algorithm = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"put_all_updated_files_into_place_at_end"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->put_all_updated_files_into_place_at_end = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"dont_map_uid_gid_values_by_user_group_name"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->dont_map_uid_gid_values_by_user_group_name = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"find_similiar_file_for_basis_if_no_dest_file"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->find_similiar_file_for_basis_if_no_dest_file = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"compress_file_data_during_the_transfer"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->compress_file_data_during_the_transfer = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"keep_partially_transfered_files"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->keep_partially_transfered_files = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"copy_symlinks_as_symlinks"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->copy_symlinks_as_symlinks = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"transform_symlink_into_referent_file_dir"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->transform_symlink_into_referent_file_dir = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"only_unsafe_symlinks_are_transformed"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->only_unsafe_symlinks_are_transformed = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"ignore_symlinks_that_point_outside_the_tree"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->ignore_symlinks_that_point_outside_the_tree = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"preserve_hardlinks"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->preserve_hardlinks = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"treat_symlinked_dir_on_receiver_as_dir"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->treat_symlinked_dir_on_receiver_as_dir = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"hostname"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);	
			run_settings->hostname = key;
			//xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"port"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);	
			run_settings->port = key;
			//xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"username"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);	
			run_settings->username = key;
			//xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"password"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);	
			run_settings->password = key;
			//xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"remote_shell"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);	
			run_settings->remote_shell = key;
			//xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"remote_rsync"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);	
			run_settings->remote_rsync = key;
			//xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"sync_to_host"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->sync_to_host = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"specify_the_remote_shell_to_use"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->specify_the_remote_shell_to_use = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"specify_the_rsync_to_run_on_remote_machine"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->specify_the_rsync_to_run_on_remote_machine = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"compare_mod_times_with_reduced_accuracy"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);	
			run_settings->compare_mod_times_with_reduced_accuracy = key;
			//xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"create_temporary_files_in_dir"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);	
			run_settings->create_temporary_files_in_dir = key;
			//xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"custom_args"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);	
			run_settings->custom_args = key;
			//xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"archive_mode"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->archive_mode = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	     	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"force_a_fixed_checksum_blocksize"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);	
			run_settings->force_a_fixed_checksum_blocksize = key;
			//xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"handle_sparse_files_effeciently"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->handle_sparse_files_effeciently = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"prefer_ipv4"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->prefer_ipv4 = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
 	    
 	    if ((!xmlStrcmp(cur->name, (const xmlChar *)"prefer_ipv6"))) {
		    key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
			run_settings->prefer_ipv6 = xml_bool((const xmlChar *)key);
		    //xmlFree(key);
 	    }
	cur = cur->next;
	}
    return 0;
}

gboolean
load_settings (const char *filename, ZConf *run_settings) 
{	
	xmlDocPtr doc;
	xmlNodePtr cur;
	
	doc = xmlParseFile(filename);
	
	if (doc == NULL ) {
		fprintf(stderr,"Document not parsed successfully. \n");
		return FALSE;
	} else {
	
		cur = xmlDocGetRootElement(doc);
		if (cur == NULL) {
			fprintf(stderr,"Empty document\n");
			xmlFreeDoc(doc);
			return FALSE;
		}
		if (xmlStrcmp(cur->name, (const xmlChar *) "zynk")) {
			fprintf(stderr,"Document of the wrong type, root node != zynk");
			xmlFreeDoc(doc);
			return FALSE;
		}
		cur = cur->xmlChildrenNode;
		while (cur != NULL) {
			if ((!xmlStrcmp(cur->name, (const xmlChar *)"values"))){
				parse_settings (doc, cur, run_settings);
			}
		cur = cur->next;
		}
		xmlFreeDoc(doc);
		return TRUE;
	}
	return FALSE;
}

void 
testXmlwriterFilename(const char *uri)
{
    int rc;
    xmlTextWriterPtr writer;
    xmlChar *tmp;

    /* Create a new XmlWriter for uri, with no compression. */
    writer = xmlNewTextWriterFilename(uri, 0);
    if (writer == NULL) {
        printf("testXmlwriterFilename: Error creating the xml writer\n");
        return;
    }

    /* Start the document with the xml default for the version,
     * encoding ISO 8859-1 and the default for the standalone
     * declaration. */
    rc = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterStartDocument\n");
        return;
    }

    /* Start an element named "EXAMPLE". Since thist is the first
     * element, this will be the root element of the document. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "EXAMPLE");
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
        return;
    }

    /* Write a comment as child of EXAMPLE.
     * Please observe, that the input to the xmlTextWriter functions
     * HAS to be in UTF-8, even if the output XML is encoded
     * in iso-8859-1 */
    tmp = ConvertInput("This is a comment with special chars: <äöü>", MY_ENCODING);
    rc = xmlTextWriterWriteComment(writer, tmp);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterWriteComment\n");
        return;
    }
    if (tmp != NULL) xmlFree(tmp);

    /* Start an element named "ORDER" as child of EXAMPLE. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "ORDER");
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
        return;
    }

    /* Add an attribute with name "version" and value "1.0" to ORDER. */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "version",
                                     BAD_CAST "1.0");
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterWriteAttribute\n");
        return;
    }

    /* Add an attribute with name "xml:lang" and value "de" to ORDER. */
    rc = xmlTextWriterWriteAttribute(writer, BAD_CAST "xml:lang",
                                     BAD_CAST "de");
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterWriteAttribute\n");
        return;
    }

    /* Write a comment as child of ORDER */
    tmp = ConvertInput("<äöü>", MY_ENCODING);
    rc = xmlTextWriterWriteFormatComment(writer,
		     "This is another comment with special chars: %s",
		     tmp);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterWriteFormatComment\n");
        return;
    }
    if (tmp != NULL) xmlFree(tmp);

    /* Start an element named "HEADER" as child of ORDER. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "HEADER");
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
        return;
    }

    /* Write an element named "X_ORDER_ID" as child of HEADER. */
    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "X_ORDER_ID",
                                         "%010d", 53535L);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
        return;
    }

    /* Write an element named "CUSTOMER_ID" as child of HEADER. */
    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "CUSTOMER_ID",
                                         "%d", 1010);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
        return;
    }

    /* Write an element named "NAME_1" as child of HEADER. */
    tmp = ConvertInput("Müller", MY_ENCODING);
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "NAME_1", tmp);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterWriteElement\n");
        return;
    }
    if (tmp != NULL) xmlFree(tmp);

    /* Write an element named "NAME_2" as child of HEADER. */
    tmp = ConvertInput("Jörg", MY_ENCODING);
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "NAME_2", tmp);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterWriteElement\n");
        return;
    }
    if (tmp != NULL) xmlFree(tmp);

    /* Close the element named HEADER. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterEndElement\n");
        return;
    }

    /* Start an element named "ENTRIES" as child of ORDER. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "ENTRIES");
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
        return;
    }

    /* Start an element named "ENTRY" as child of ENTRIES. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "ENTRY");
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
        return;
    }

    /* Write an element named "ARTICLE" as child of ENTRY. */
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "ARTICLE",
                                   BAD_CAST "<Test>");
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterWriteElement\n");
        return;
    }

    /* Write an element named "ENTRY_NO" as child of ENTRY. */
    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "ENTRY_NO", "%d",
                                         10);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
        return;
    }

    /* Close the element named ENTRY. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterEndElement\n");
        return;
    }

    /* Start an element named "ENTRY" as child of ENTRIES. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "ENTRY");
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
        return;
    }

    /* Write an element named "ARTICLE" as child of ENTRY. */
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "ARTICLE",
                                   BAD_CAST "<Test 2>");
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterWriteElement\n");
        return;
    }

    /* Write an element named "ENTRY_NO" as child of ENTRY. */
    rc = xmlTextWriterWriteFormatElement(writer, BAD_CAST "ENTRY_NO", "%d",
                                         20);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterWriteFormatElement\n");
        return;
    }

    /* Close the element named ENTRY. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterEndElement\n");
        return;
    }

    /* Close the element named ENTRIES. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterEndElement\n");
        return;
    }

    /* Start an element named "FOOTER" as child of ORDER. */
    rc = xmlTextWriterStartElement(writer, BAD_CAST "FOOTER");
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterStartElement\n");
        return;
    }

    /* Write an element named "TEXT" as child of FOOTER. */
    rc = xmlTextWriterWriteElement(writer, BAD_CAST "TEXT",
                                   BAD_CAST "This is a text.");
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterWriteElement\n");
        return;
    }

    /* Close the element named FOOTER. */
    rc = xmlTextWriterEndElement(writer);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterEndElement\n");
        return;
    }

    /* Here we could close the elements ORDER and EXAMPLE using the
     * function xmlTextWriterEndElement, but since we do not want to
     * write any other elements, we simply call xmlTextWriterEndDocument,
     * which will do all the work. */
    rc = xmlTextWriterEndDocument(writer);
    if (rc < 0) {
        printf
            ("testXmlwriterFilename: Error at xmlTextWriterEndDocument\n");
        return;
    }

    xmlFreeTextWriter(writer);
}

gboolean 
write_settings (const char *filename, ZConf *run_settings)
{
	/*
     * this initialize the library and check potential ABI mismatches
     * between the version it was compiled for and the actual shared
     * library used.
     */
    LIBXML_TEST_VERSION

    /* first, the file version */
    testXmlwriterFilename("/home/hanez/Desktop/zynk.zynk");

    /*
     * Cleanup function for the XML library.
     */
    xmlCleanupParser();
    /*
     * this is to debug memory for regression tests
     */
    xmlMemoryDump();
	return 0;
}
