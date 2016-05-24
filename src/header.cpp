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

#include <header.hpp>

namespace http {

///////////////////////////////////////////////////////////////////////////////
Header::Header(const Limit limit) noexcept {
  if (limit <= 0) return;
  limit_ = limit;
}

///////////////////////////////////////////////////////////////////////////////
bool Header::add_field(const span& field, const span& value) {
  if (field.is_empty()) return false;
  //-----------------------------------
  if (size() < limit_) {
    map_.emplace_back(field, value);
    return true;
  }
  //-----------------------------------
  return false;
}

///////////////////////////////////////////////////////////////////////////////
bool Header::set_field(const span& field, const span& value) {
  if (field.is_empty() || value.is_empty()) return false;
  //-----------------------------------
  auto target = find(field);
  //-----------------------------------
  if (target not_eq map_.end()) {
    const_cast<span&>(target->second).data = value.data;
    const_cast<span&>(target->second).len  = value.len;
    return true;
  }
  //-----------------------------------
  else return add_field(field, value);
}

///////////////////////////////////////////////////////////////////////////////
bool Header::has_field(const span& field) const noexcept {
  return find(field) not_eq map_.end();
}

///////////////////////////////////////////////////////////////////////////////
const span& Header::get_value(const span& field) const noexcept {
  return find(field)->second;
}

///////////////////////////////////////////////////////////////////////////////
bool Header::is_empty() const noexcept {
  return map_.empty();
}

///////////////////////////////////////////////////////////////////////////////
Limit Header::size() const noexcept {
  return map_.size();
}

///////////////////////////////////////////////////////////////////////////////
void Header::erase(const span& field) noexcept {
  auto target = find(field);
  //-----------------------------------
  if (target not_eq map_.end()) map_.erase(target);
}

///////////////////////////////////////////////////////////////////////////////
void Header::clear() noexcept {
  map_.clear();
}

///////////////////////////////////////////////////////////////////////////////
static std::string string_to_lower_case(std::string string) {
  std::transform(string.begin(), string.end(),
                 string.begin(), ::tolower);
  //-----------------------------------
  return string;
}

///////////////////////////////////////////////////////////////////////////////
Header::Const_Iterator Header::find(const span& field) const noexcept {
  if (field.is_empty()) return map_.end();
  //-----------------------------------
  return
  std::find_if(map_.begin(), map_.end(), [&field](const auto& f){
    return string_to_lower_case(f.first.to_string())
           ==
           string_to_lower_case(field.to_string());
  });
}

///////////////////////////////////////////////////////////////////////////////
std::ostream& operator << (std::ostream& output_device, const Header& header) {
  for (const auto& field : header.map_) {
    output_device << field.first  << ": "
                  << field.second << "\r\n";
  }
  //-----------------------------------
  return output_device << "\r\n";
}

} //< namespace http
