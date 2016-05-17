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

inline Request::Request(const char* request, const size_t length, const Limit limit)
  : Request{{request, length}, limit}
{}

inline Request::Request(std::string request, const Limit limit)
  : Message{limit}
  , request_{std::move(request)}
  , field_{nullptr, 0}
{
  configure_settings()
  .execute_parser();
}

inline Method Request::method() const noexcept {
  return method_;
}

inline Request& Request::set_method(const Method method) {
  method_ = method;
  return *this;
}

inline const URI& Request::uri() const noexcept {
  return uri_;
}

inline Request& Request::set_uri(const URI& uri) {
  uri_ = uri;
  return *this;
}

inline const Version& Request::version() const noexcept {
  return version_;
}

inline Request& Request::set_version(const Version& version) noexcept {
  version_ = version;
  return *this;
}

template <typename Name>
inline std::string Request::query_value(Name&& name) const noexcept {
  return get_value(uri(), std::forward<Name>(name));
}

template <typename Name>
inline std::string Request::post_value(Name&& name) const noexcept {
  if (method() not_eq POST) return std::string{};
  return get_value(get_body(), std::forward<Name>(name));
}

inline Request& Request::reset() noexcept {
  Message::reset();
  return set_method(GET)
        .set_uri("/")
        .set_version(Version{1U, 1U});
}

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

inline std::string Request::to_string() const {
  return *this;
}

inline Request::operator std::string () const {
  return request_;
}

inline Request& Request::configure_settings() noexcept {
  http_parser_settings_init(&settings_);

  settings_.on_message_begin = [](http_parser* parser) {
    auto req = reinterpret_cast<Request*>(parser->data);
    req->method_ = http::method::code(http_method_str(static_cast<http_method>(parser->method)));
    return 0;
  };

  settings_.on_url = [](http_parser* parser, const char* at, size_t length) {
    auto req = reinterpret_cast<Request*>(parser->data);
    req->uri_ = std::string{at, length};
    return 0;
  };

  settings_.on_header_field = [](http_parser* parser, const char* at, size_t length) {
    auto req = reinterpret_cast<Request*>(parser->data);
    req->field_.data = at;
    req->field_.len  = length;
    return 0;
  };

  settings_.on_header_value = [](http_parser* parser, const char* at, size_t length) {
    auto req = reinterpret_cast<Request*>(parser->data);
    req->add_header(req->field_, {at, length});
    return 0;
  };

  settings_.on_body = [](http_parser* parser, const char* at, size_t length) {
    auto req = reinterpret_cast<Request*>(parser->data);
    req->add_body({at, length});
    return 0;
  };

  settings_.on_headers_complete = [](http_parser* parser) {
    auto req = reinterpret_cast<Request*>(parser->data);
    req->version_ = Version{parser->http_major, parser->http_minor};
    return 0;
  };

  return *this;
}

inline void Request::execute_parser() noexcept {
  http_parser_init(&parser_, HTTP_REQUEST);
  parser_.data = this;
  http_parser_execute(&parser_, &settings_, request_.data(), request_.size());
}

inline std::ostream& operator << (std::ostream& output_device, const Request& req) {
  return output_device << req.to_string();
}

inline Request_ptr make_request(buffer_t buf, const size_t len) {
  return std::make_shared<Request>(reinterpret_cast<char*>(buf.get()), len);
}

inline Request_ptr make_request(std::string request) {
  return std::make_shared<Request>(std::move(request));
}

/**--^----------- Implementation Details -----------^--**/

} //< namespace http

#endif //< HTTP_REQUEST_HPP
