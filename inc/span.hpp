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

#ifndef HTTP_SPAN_HPP
#define HTTP_SPAN_HPP

#include <string>
#include <ostream>

namespace http {

//-----------------------------------------------
// This is a lightweight data-structure to be
// used within the http package
//
// Its main purpose is to refer to tokens within
// a buffer to eliminate unnecessary copying
//-----------------------------------------------
struct span {
  const char* data;
  size_t      len;

  //-----------------------------------------------
  // Constructor to create an empty span
  //-----------------------------------------------
  span() noexcept;

  //-----------------------------------------------
  // Constructor to create a span from a C-String
  //
  // @param data - The data to point to (null terminated cstring)
  //-----------------------------------------------
  span(const char* data) noexcept;

  //-----------------------------------------------
  // Constructor to create a span from data pointer and
  // length
  //
  // @param data - The data to point to
  // @paran len  - The length (number of bytes) of the data
  //-----------------------------------------------
  span(const char* data, const size_t len) noexcept;

  //-----------------------------------------------
  // Operator to check if two spans are equal to
  // each other, which mean that they represent
  // similar tokens
  //-----------------------------------------------
  bool operator==(const span& other) const noexcept;

  //-----------------------------------------------
  // Method to check if a span has no information,
  // which mean it doesn't represent a token
  //-----------------------------------------------
  bool is_empty() const noexcept;

  //-----------------------------------------------
  // Method to change the state of this
  // data-structure to empty
  //-----------------------------------------------
  void clear() noexcept;

  //-----------------------------------
  // Get a string representation of this
  // data-structure
  //
  // @return - A string representation
  //-----------------------------------
  std::string to_string() const;

  //-----------------------------------------------
  // Operator to transform this data-structure
  // into string form
  //-----------------------------------------------
  operator std::string () const;
}; //< struct span

/**--v----------- Implementation Details -----------v--**/

//-----------------------------------------------
// Operator to stream the contents of a span to
// an output device
//-----------------------------------------------
inline std::ostream& operator << (std::ostream& output_device, const span& span) {
  return output_device << span.to_string();
}

/**--^----------- Implementation Details -----------^--**/

} //< namespace http

#endif //< HTTP_SPAN_HPP
