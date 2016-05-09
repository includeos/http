// This file is a part of the IncludeOS unikernel - www.includeos.org
//
// Copyright 2015-2016 Oslo and Akershus University College of Applied Sciences
// and Alfred Bratterud
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef HTTP_HEADER_HPP
#define HTTP_HEADER_HPP

#include <list>
#include <cctype>
#include <cstring>
#include <utility>
#include <ostream>
#include <algorithm>
#include <type_traits>

#include "span.hpp"
#include "common.hpp"
#include "header_fields.hpp" //< Standard header field names

namespace http {

//-----------------------------------------------
// This class is used to store header information
// associated with an http message
//
// By default it is limited to 100 fields
// but the amount can be specified by using the
// appropriate constructor and provided method.
//-----------------------------------------------
class Header {
private:
  //-----------------------------------------------
  // Internal class type aliases
  //-----------------------------------------------
  using Map_Type  = std::pair<span, span>;
  using Field_Map = std::list<Map_Type>;
  using Const_Iterator  = Field_Map::const_iterator;
  //-----------------------------------------------
public:
  //-----------------------------------------------
  // Default constructor that limits the amount
  // of fields that can be added to 100
  //-----------------------------------------------
  explicit Header() = default;

  //-----------------------------------------------
  // Constructor to specify the limit of how many
  // fields that can be added
  //
  // @param limit - Capacity of how many fields can
  //                be added
  //-----------------------------------------------
  explicit Header(const Limit limit) noexcept;

  //-----------------------------------------------
  // Default destructor
  //-----------------------------------------------
  ~Header() noexcept = default;

  //-----------------------------------------------
  // Default copy constructor
  //-----------------------------------------------
  Header(const Header&) = default;

  //-----------------------------------------------
  // Default move constructor
  //-----------------------------------------------
  Header(Header&&) noexcept = default;

  //-----------------------------------------------
  // Default assignment operator
  //-----------------------------------------------
  Header& operator = (const Header&) = default;

  //-----------------------------------------------
  // Default move assignemt operator
  //-----------------------------------------------
  Header& operator = (Header&&) = default;

  //-----------------------------------------------
  // Add a new field to the current set
  //
  // @param field - The field name
  // @param value - The field value
  //
  // @return - true if the field was added, false
  //           otherwise
  //-----------------------------------------------
  bool add_field(const span field, const span value);

  //-----------------------------------------------
  // Change the value of the specified field
  //
  // If the field is absent from the set it will
  // be added with the associated value once its
  // within capacity
  //
  // @param field - The field name
  // @param value - The field value
  //
  // @return - true if successful, false otherwise
  //-----------------------------------------------
  bool set_field(const span field, const span value);

  //-----------------------------------------------
  // Get the value associated with a field
  //
  // Should call <has_field> before calling this
  //
  // @param field - The field name
  //
  // @return - The value associated with the
  //           specified field name
  //-----------------------------------------------
  const span& get_value(const char* field) const noexcept;

  //-----------------------------------------------
  // Check to see if the specified field is a
  // member of the set
  //
  // @param field - The field name
  //
  // @return - true if the field is a member,
  //           false otherwise
  //-----------------------------------------------
  bool has_field(const char* field) const noexcept;

  //-----------------------------------------------
  // Check to see if the set is empty
  //
  // @return - true if there are no fields within
  //           the set, false otherwise
  //-----------------------------------------------
  bool is_empty() const noexcept;

  //-----------------------------------------------
  // Check to see how many fields are currently
  // in the set
  //
  // @return - The amount of fields currently in
  //           the set
  //-----------------------------------------------
  Limit size() const noexcept;

  //-----------------------------------------------
  // Remove all fields from the set with the
  // specified name
  //
  // @param field - The field name to remove
  //-----------------------------------------------
  void erase(const char* field) noexcept;

  //-----------------------------------------------
  // Remove all fields from the set leaving it
  // empty
  //-----------------------------------------------
  void clear() noexcept;
private:
  //-----------------------------------------------
  // Class data members
  //-----------------------------------------------
  Field_Map map_;
  Limit     limit_ {100};

  //-----------------------------------------------
  // Find the location of a field within the set
  //
  // @param field - The field name to locate
  //
  // @return - Iterator to the location of the field,
  //           else location to the end of the
  //           sequence
  //-----------------------------------------------
  Const_Iterator find(const span field) const noexcept;

  //-----------------------------------------------
  // Operator to stream the contents of the set
  // into the specified output stream device
  //
  // The format is as follows:
  // field : value "\r\n"
  //-----------------------------------------------
  friend std::ostream& operator << (std::ostream&, const Header&);
}; //< class Header

/**--v----------- Implementation Details -----------v--**/

inline Header::Header(const Limit limit) noexcept {
  if (limit <= 0) return;
  limit_ = limit;
}

inline bool Header::add_field(const span field, const span value) {
  if (empty(field)) return false;
  //-----------------------------------
  if (size() < limit_) {
    map_.emplace_back(field, value);
    return true;
  }
  //-----------------------------------
  return false;
}

inline bool Header::set_field(const span field, const span value) {
  if (empty(field) || empty(value)) return false;
  //-----------------------------------
  auto target = find(field);
  //-----------------------------------
  if (target not_eq map_.end()) {
    const_cast<span&>(target->second).data = value.data;
    const_cast<span&>(target->second).len  = value.len;
    return true;
  }
  else return add_field(field, value);
}

inline const span& Header::get_value(const char* field) const noexcept {
  auto field_length = strlen(field);
  //-----------------------------------
  if (field_length == 0) return {"", 0};
  //-----------------------------------
  return find({field, field_length})->second;
}

inline bool Header::has_field(const char* field) const noexcept {
  auto field_length = strlen(field);
  //-----------------------------------
  if (field_length == 0) return false;
  //-----------------------------------
  return find({field, field_length}) not_eq map_.end();
}

inline bool Header::is_empty() const noexcept {
  return map_.empty();
}

inline Limit Header::size() const noexcept {
  return map_.size();
}

inline void Header::erase(const char* field) noexcept {
   auto field_length = strlen(field);
  //-----------------------------------
  if (field_length == 0) return;
  //-----------------------------------
  auto target = find({field, field_length});
  //-----------------------------------
  if (target not_eq map_.end()) map_.erase(target);
}

inline void Header::clear() noexcept {
  map_.clear();
}

inline static std::string string_to_lower_case(std::string string) {
  std::transform(string.begin(), string.end(),
                 string.begin(), ::tolower);
  //-----------------------------------
  return string;
}

inline Header::Const_Iterator Header::find(const span field) const noexcept {
  if (empty(field)) return map_.end();
  //-----------------------------------
  return
  std::find_if(map_.begin(), map_.end(), [&field](const Header::Map_Type& f){
    return string_to_lower_case({f.first.data, f.first.len})
           ==
           string_to_lower_case({field.data, field.len});
  });
}

inline std::ostream& operator << (std::ostream& output_device, const Header& header) {
  for (const auto& field : header.map_) {
    output_device << std::string{field.first.data, field.first.len}   << ": "
                  << std::string{field.second.data, field.second.len} << "\r\n";
  }
  //-----------------------------------
  return output_device << "\r\n";
}

/**--^----------- Implementation Details -----------^--**/

} //< namespace http

#endif //< HTTP_HEADER_HPP
