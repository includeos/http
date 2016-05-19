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

#ifndef HTTP_REQUEST_HPP
#define HTTP_REQUEST_HPP

#include "message.hpp"
#include "methods.hpp"
#include "version.hpp"

namespace http {

//----------------------------------------
// This class is used to represent
// an http request message
//----------------------------------------
class Request : public Message {
public:
  //----------------------------------------
  // Default constructor
  //
  // Constructs a request message as follows:
  //
  // <GET / HTTP/1.1>
  //----------------------------------------
  explicit Request() = default;

  //----------------------------------------
  // Constructor to construct a request
  // message from the incoming character
  // stream of data which is a C-String
  //
  // @param request - The character stream of data
  // @param length  - The length of the character stream
  //
  // @param limit - Capacity of how many fields can
  //                be added
  //----------------------------------------
  explicit Request(const char* request, const size_t length, const Limit limit = 100);

  //----------------------------------------
  // Constructor to construct a request
  // message from the incoming character
  // stream of data which is a <std::string>
  // object
  //
  // @param request - The character stream of data
  //
  // @param limit - Capacity of how many fields can
  //                be added
  //----------------------------------------
  explicit Request(std::string request, const Limit limit = 100);

  //----------------------------------------
  // Default copy constructor
  //----------------------------------------
  Request(Request&) = default;

  //----------------------------------------
  // Default move constructor
  //----------------------------------------
  Request(Request&&) = default;

  //----------------------------------------
  // Default destructor
  //----------------------------------------
  ~Request() noexcept = default;

  //----------------------------------------
  // Default copy assignment operator
  //----------------------------------------
  Request& operator = (Request&) = default;

  //----------------------------------------
  // Default move assignment operator
  //----------------------------------------
  Request& operator = (Request&&) = default;

  //----------------------------------------
  // Get the method of the request message
  //
  // @return - The method of the request
  //----------------------------------------
  Method method() const noexcept;

  //----------------------------------------
  // Set the method of the request message
  //
  // @param method - The method to set
  //
  // @return - The object that invoked this
  //           method
  //----------------------------------------
  Request& set_method(const Method method);

  //----------------------------------------
  // Get the URI of the request message
  //
  // @return - The URI of the request
  //----------------------------------------
  const URI& uri() const noexcept;

  //----------------------------------------
  // Set the URI of the request message
  //
  // @param uri - The URI to set
  //
  // @return - The object that invoked this
  //           method
  //----------------------------------------
  Request& set_uri(const URI& uri);

  //----------------------------------------
  // Get the version of the request message
  //
  // @return - The version of the request
  //----------------------------------------
  const Version& version() const noexcept;

  //----------------------------------------
  // Set the version of the request message
  //
  // @param version - The version to set
  //
  // @return - The object that invoked this
  //           method
  //----------------------------------------
  Request& set_version(const Version& version) noexcept;

  //----------------------------------------
  // Get the value associated with the name
  // field from a query string
  //
  // @tparam (std::string) name - The name to find the associated
  //                              value
  //
  // @return - The associated value if name was found,
  //           an empty string otherwise
  //----------------------------------------
  template <typename Name>
  std::string query_value(Name&& name) const noexcept;

  //----------------------------------------
  // Get the value associated with the name
  // field from the message body in a post request
  //
  // @tparam (std::string) name - The name to find the associated
  //                              value
  //
  // @return - The associated value if name was found,
  //           an empty string otherwise
  //----------------------------------------
  template <typename Name>
  std::string post_value(Name&& name) const noexcept;

  //----------------------------------------
  // Reset the request message as if it was now
  // default constructed
  //
  // @return - The object that invoked this method
  //----------------------------------------
  virtual Request& reset() noexcept override;

  //----------------------------------------
  // Get a string representation of this
  // class
  //
  // @return - A string representation
  //----------------------------------------
  virtual std::string to_string() const override;

  //----------------------------------------
  // Operator to transform this class
  // into string form
  //----------------------------------------
  operator std::string () const;
  //----------------------------------------
private:
  //----------------------------------------
  // Class data members
  //----------------------------------------
  const std::string request_;
  span              field_;

  //----------------------------------------
  // Request-line parts
  //----------------------------------------
  Method  method_{GET};
  URI     uri_{"/"};
  Version version_{1U, 1U};

  //----------------------------------------
  // Private request parser
  //----------------------------------------
  http_parser          parser_;
  http_parser_settings settings_;

  //----------------------------------------
  // Configure the parser settings
  //
  // @return - The object that invoked this
  //           method
  //----------------------------------------
  Request& configure_settings() noexcept;

  //----------------------------------------
  // Execute the parser
  //----------------------------------------
  void execute_parser() noexcept;

  //----------------------------------------
  // Find the value associated with a name
  // in the following format:
  //
  // name=value
  //
  // @tparam (std::string) data - The data to search through
  // @tparam (std::string) name - The name to find the associated
  //                              value
  //
  // @return - The associated value if name was found,
  //           an empty string otherwise
  //----------------------------------------
  template <typename Data, typename Name>
  std::string get_value(Data&& data, Name&& name) const noexcept;
}; //< class Request

/**--v----------- Implementation Details -----------v--**/

///////////////////////////////////////////////////////////////////////////////
template <typename Name>
inline std::string Request::query_value(Name&& name) const noexcept {
  return get_value(uri(), std::forward<Name>(name));
}

///////////////////////////////////////////////////////////////////////////////
template <typename Name>
inline std::string Request::post_value(Name&& name) const noexcept {
  if (method() not_eq POST) return std::string{};
  return get_value(get_body().to_string(), std::forward<Name>(name));
}

///////////////////////////////////////////////////////////////////////////////
template <typename Data, typename Name>
inline std::string Request::get_value(Data&& data, Name&& name) const noexcept {
  if (data.empty() || name.empty()) return std::string{};
  //---------------------------------
  auto target = data.find(std::forward<Name>(name));
  //---------------------------------
  if (target == std::string::npos) return std::string{};
  //---------------------------------
  auto focal_point = data.substr(target);
  //---------------------------------
  focal_point = focal_point.substr(0, focal_point.find_first_of('&'));
  //---------------------------------
  auto lock_and_load = focal_point.find('=');
  //---------------------------------
  if (lock_and_load == std::string::npos) return std::string{};
  //---------------------------------
  return focal_point.substr(lock_and_load + 1);
}

///////////////////////////////////////////////////////////////////////////////
inline Request_ptr make_request(buffer_t buf, const size_t len) {
  return std::make_shared<Request>(reinterpret_cast<char*>(buf.get()), len);
}

///////////////////////////////////////////////////////////////////////////////
inline Request_ptr make_request(std::string request) {
  return std::make_shared<Request>(std::move(request));
}

///////////////////////////////////////////////////////////////////////////////
inline std::ostream& operator << (std::ostream& output_device, const Request& req) {
  return output_device << req.to_string();
}

/**--^----------- Implementation Details -----------^--**/

} //< namespace http

#endif //< HTTP_REQUEST_HPP
