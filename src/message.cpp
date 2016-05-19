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

#include <message.hpp>
#include <header_fields.hpp> //< Standard header field names

namespace http {

///////////////////////////////////////////////////////////////////////////////
Message::Message(const Limit limit) noexcept
  : header_fields_{limit}
  , message_body_{}
{}

///////////////////////////////////////////////////////////////////////////////
Message& Message::add_header(const char* field, const char* value) {
  header_fields_.add_field({field, strlen(field)}, {value, strlen(value)});
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
Message& Message::set_header(const char* field, const char* value) {
  header_fields_.set_field({field, strlen(field)}, {value, strlen(value)});
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
Message& Message::add_header(const span& field, const span& value) {
  header_fields_.add_field(field, value);
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
Message& Message::set_header(const span& field, const span& value) {
  header_fields_.set_field(field, value);
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
const span Message::header_value(const char* field) const noexcept {
  return header_fields_.get_value(field);
}

///////////////////////////////////////////////////////////////////////////////
bool Message::has_header(const char* field) const noexcept {
  return header_fields_.has_field(field);
}

///////////////////////////////////////////////////////////////////////////////
bool Message::is_header_empty() const noexcept {
  return header_fields_.is_empty();
}

///////////////////////////////////////////////////////////////////////////////
Message::HSize Message::header_size() const noexcept {
  return header_fields_.size();
}

///////////////////////////////////////////////////////////////////////////////
Message& Message::erase_header(const char* field) noexcept {
  header_fields_.erase(field);
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
Message& Message::clear_headers() noexcept {
  header_fields_.clear();
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
Message& Message::add_body(const Message_Body& message_body) {
  if (message_body.empty()) return *this;
  //-----------------------------------
  message_body_ = message_body;
  mbody_length_ = std::to_string(message_body_.size());
  //-----------------------------------
  return add_header(header_fields::Entity::Content_Length,
                    mbody_length_.c_str());
}

///////////////////////////////////////////////////////////////////////////////
Message& Message::add_chunk(const std::string& chunk) {
  if (chunk.empty()) return *this;
  //-----------------------------------
  message_body_.append(chunk);
  mbody_length_ = std::to_string(message_body_.size());
  //-----------------------------------
  return set_header(header_fields::Entity::Content_Length,
                    mbody_length_.c_str());
}

///////////////////////////////////////////////////////////////////////////////
bool Message::has_body() const noexcept {
  return not message_body_.empty();
}

///////////////////////////////////////////////////////////////////////////////
const Message::Message_Body& Message::get_body() const noexcept {
  return message_body_;
}

///////////////////////////////////////////////////////////////////////////////
Message& Message::clear_body() noexcept {
  message_body_.clear();
  return erase_header(header_fields::Entity::Content_Length);
}

///////////////////////////////////////////////////////////////////////////////
Message& Message::reset() noexcept {
  return clear_headers().clear_body();
}

///////////////////////////////////////////////////////////////////////////////
std::string Message::to_string() const {
  std::ostringstream message;
  //-----------------------------------
  message << header_fields_
          << message_body_;
  //-----------------------------------
  return message.str();
}

///////////////////////////////////////////////////////////////////////////////
Message::operator std::string () const {
  return to_string();
}

///////////////////////////////////////////////////////////////////////////////
std::ostream& operator << (std::ostream& output_device, const Message& message) {
  return output_device << message.to_string();
}

} //< namespace http
