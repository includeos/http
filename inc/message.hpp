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

#ifndef HTTP_MESSAGE_HPP
#define HTTP_MESSAGE_HPP

#include <sstream>

#include "time.hpp"
#include "header.hpp"
#include "header_fields.hpp"

namespace http {

class Message {
private:
  //----------------------------------------
  // Internal class type aliases
  //----------------------------------------
  using HSize        = Limit;
  using Message_Body = std::string;
  using MBody_Length = std::string;
  //----------------------------------------
public:
  //----------------------------------------
  // Default constructor
  //----------------------------------------
  explicit Message() = default;

  //----------------------------------------
  // Constructor to specify the limit of how many
  // fields that can be added to the message
  //
  // @param limit - Capacity of how many fields can
  //                be added to the message
  //----------------------------------------
  explicit Message(const Limit limit) noexcept;

  //----------------------------------------
  // Default copy constructor
  //----------------------------------------
  Message(const Message&) = default;

  //----------------------------------------
  // Default move constructor
  //----------------------------------------
  explicit Message(Message&&) noexcept = default;

  //----------------------------------------
  // Default destructor
  //----------------------------------------
  virtual ~Message() noexcept = default;

  //----------------------------------------
  // Default copy assignment operator
  //----------------------------------------
  Message& operator = (const Message&) = default;

  //----------------------------------------
  // Default move assignment operator
  //----------------------------------------
  Message& operator = (Message&&) = default;

  //----------------------------------------
  // Add a new field to the current set of
  // headers
  //
  // @param field - The field name
  // @param value - The field value
  //
  // @return - The object that invoked this method
  //----------------------------------------
  Message& add_header(const span& field, const span& value);

  //----------------------------------------
  // Change the value of the specified field
  //
  // If the field is absent from the message it
  // will be added with the associated value
  //
  // @param field - The field name
  // @param value - The field value
  //
  // @return - The object that invoked this method
  //----------------------------------------
  Message& set_header(const span& field, const span& value);

  //----------------------------------------
  // Check if the specified field is within
  // this message
  //
  // @param field - The field name to search for
  //
  // @return - true is present, false otherwise
  //----------------------------------------
  bool has_header(const span& field) const noexcept;

  //----------------------------------------
  // Get the value associated with the
  // specified field name
  //
  // Should call <has_header> before calling this
  //
  // @param field - The field name to get associated
  //                value
  //
  // @return - The value associated with the
  //           specified field name
  //----------------------------------------
  const span& header_value(const span& field) const noexcept;

  //----------------------------------------
  // Check if there are no fields in this
  // message
  //
  // @return - true if the message has no
  //           fields, false otherwise
  //----------------------------------------
  bool is_header_empty() const noexcept;

  //----------------------------------------
  // Get the number of fields in this
  // message
  //
  // @return - The number of fields in this
  //           message
  //----------------------------------------
  HSize header_size() const noexcept;

  //----------------------------------------
  // Remove the specified field from this
  // message
  //
  // @param field - The header to remove from
  //                this message
  //
  // @return - The object that invoked this method
  //----------------------------------------
  Message& erase_header(const span& field) noexcept;

  //----------------------------------------
  // Remove all header fields from this
  // message
  //
  // @return - The object that invoked this method
  //----------------------------------------
  Message& clear_headers() noexcept;

  //----------------------------------------
  // Add an entity to the message
  //
  // @param message_body - The entity to be
  //                       sent with the message
  //
  // @return - The object that invoked this method
  //----------------------------------------
  Message& add_body(const Message_Body& message_body);

  //----------------------------------------
  // Append a chunk to the entity of the message
  //
  // @param chunk - A chunk to append to the entity
  //
  // @return - The object that invoked this method
  //----------------------------------------
  Message& add_chunk(const std::string& chunk);

  //----------------------------------------
  // Check if this message has an entity
  //
  // @return true if entity is present, false
  //         otherwise
  //----------------------------------------
  bool has_body() const noexcept;

  //----------------------------------------
  // Get the entity in this the message if
  // present
  //
  // @return - The entity in this message
  //----------------------------------------
  const Message_Body& get_body() const noexcept;

  //----------------------------------------
  // Remove the entity from the message
  //
  // @return - The object that invoked this method
  //----------------------------------------
  Message& clear_body() noexcept;

  //----------------------------------------
  // Reset the message as if it was now
  // default constructed
  //
  // @return - The object that invoked this method
  //----------------------------------------
  virtual Message& reset() noexcept;
  
  //-----------------------------------
  // Get a string representation of this
  // class
  //
  // @return - A string representation
  //-----------------------------------
  virtual std::string to_string() const;

  //-----------------------------------
  // Operator to transform this class
  // into string form
  //-----------------------------------
  operator std::string () const;
  //-----------------------------------

private:
  //------------------------------
  // Class data members
  //------------------------------
  Header       header_fields_;
  Message_Body message_body_;
  MBody_Length mbody_length_;
}; //< class Message

} //< namespace http

#endif //< HTTP_MESSAGE_HPP
