/**
 * Copyright (c) 2016-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the BSD-style license found in the
 * LICENSE file in the root directory of this source tree. An additional grant
 * of patent rights can be found in the PATENTS file in the same directory.
 */

#pragma once

#include <map>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "androidfw/ResourceTypes.h"

const char* const ONCLICK_ATTRIBUTE = "android:onClick";

std::string read_entire_file(const std::string& filename);
void write_entire_file(const std::string& filename, const std::string& contents);
void* map_file(
    const char* path,
    int& file_descriptor,
    size_t& length,
    const bool mode_write = false);
size_t write_serialized_data(
    const android::Vector<char>& cVec,
    int file_descriptor,
    void* file_pointer,
    const size_t& length);
void unmap_and_close(int file_descriptor, void* file_pointer, size_t length);

std::string get_string_attribute_value(const android::ResXMLTree& parser,
                                       const android::String16& attribute_name);
bool has_raw_attribute_value(
    const android::ResXMLTree& parser,
    const android::String16& attribute_name,
    android::Res_value& out_value);
std::unordered_set<std::string> get_manifest_classes(
    const std::string& filename);
std::unordered_set<std::string> get_native_classes(
    const std::string& apk_directory);
std::unordered_set<std::string> get_layout_classes(
    const std::string& apk_directory);
std::unordered_set<std::string> get_xml_files(
    const std::string& directory);
std::unordered_set<uint32_t> get_xml_reference_attributes(
    const std::string& filename);
int inline_xml_reference_attributes(
    const std::string& filename,
    const std::map<uint32_t, android::Res_value>& id_to_inline_value);
void remap_xml_reference_attributes(
    const std::string& filename,
    const std::map<uint32_t, uint32_t>& kept_to_remapped_ids);

// Iterates through all layouts in the given directory. Adds all class names to
// the output set, and allows for any specified attribute values to be returned
// as well. Attribute names should specify their namespace, if any (so
// android:onClick instead of just onClick)
void collect_layout_classes_and_attributes(
    const std::string& apk_directory,
    const std::unordered_set<std::string>& attributes_to_read,
    std::unordered_set<std::string>& out_classes,
    std::unordered_multimap<std::string, std::string>& out_attributes);

// Same as above, for single file.
void collect_layout_classes_and_attributes_for_file(
    const std::string& file_path,
    const std::unordered_set<std::string>& attributes_to_read,
    std::unordered_set<std::string>& out_classes,
    std::unordered_multimap<std::string, std::string>& out_attributes);

// Convenience method for copying values in a multimap to a set, for a
// particular key.
std::set<std::string> multimap_values_to_set(
  const std::unordered_multimap<std::string, std::string>& map,
  const std::string& key);

// Given the bytes of a binary XML file, replace the entries (if any) in the
// ResStringPool. Writes result to the given Vector output param.
// Returns android::NO_ERROR (0) on success, or one of the corresponding
// android:: error codes for failure conditions/bad input data.
int replace_in_xml_string_pool(
    const void* data,
    const size_t len,
    const std::map<std::string, std::string>& shortened_names,
    android::Vector<char>* out_data,
    size_t* out_num_renamed);

// Replaces all strings in the ResStringPool for the given file with their
// replacements. Writes all changes to disk, clobbering the given file.
// Same return codes as replace_in_xml_string_pool.
int rename_classes_in_layout(
    const std::string& file_path,
    const std::map<std::string, std::string>& shortened_names,
    size_t* out_num_renamed,
    ssize_t* out_size_delta);


/**
 * Follows the reference links for a resource for all configurations.
 * Returns all the nodes visited, as well as all the string values seen.
 */
void walk_references_for_resource(
   uint32_t resID,
   std::unordered_set<uint32_t>& nodes_visited,
   std::unordered_set<std::string>& leaf_string_values,
   android::ResTable* table);

std::unordered_set<uint32_t> get_js_resources_by_parsing(
   const std::string& directory,
   std::map<std::string, std::vector<uint32_t>> name_to_ids);

std::unordered_set<uint32_t> get_resources_by_name_prefix(
   std::vector<std::string> prefixes,
   std::map<std::string, std::vector<uint32_t>> name_to_ids);
