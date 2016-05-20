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

#include <response.hpp>

namespace http {

///////////////////////////////////////////////////////////////////////////////
Response::Response(const Code code, const Version version) noexcept
  : code_{code}
  , version_{version}
{}

///////////////////////////////////////////////////////////////////////////////
Response::Response(std::string response, const Limit limit)
  : Message{limit}
  , response_{std::move(response)}
  , field_{nullptr, 0}
{
  configure_settings()
  .execute_parser();
}

///////////////////////////////////////////////////////////////////////////////
Code Response::status_code() const noexcept {
  return code_;
}

///////////////////////////////////////////////////////////////////////////////
Response& Response::set_status_code(const Code code) noexcept {
  code_ = code;
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
const Version Response::version() const noexcept {
  return version_;
}

///////////////////////////////////////////////////////////////////////////////
Response& Response::set_version(const Version version) noexcept {
  version_ = version;
  return *this;
}

///////////////////////////////////////////////////////////////////////////////
Response& Response::reset() noexcept {
  Message::reset();
  return set_status_code(OK);
}

///////////////////////////////////////////////////////////////////////////////
std::string Response::to_string() const {
  return response_;
}

///////////////////////////////////////////////////////////////////////////////
Response::operator std::string () const {
  return to_string();
}

///////////////////////////////////////////////////////////////////////////////
Response& Response::configure_settings() noexcept {
  http_parser_settings_init(&settings_);

  settings_.on_header_field = [](http_parser* parser, const char* at, size_t length) {
    auto res = reinterpret_cast<Response*>(parser->data);
    res->field_.data = at;
    res->field_.len  = length;
    return 0;
  };

  settings_.on_header_value = [](http_parser* parser, const char* at, size_t length) {
    auto res = reinterpret_cast<Response*>(parser->data);
    res->add_header(res->field_, {at, length});
    return 0;
  };

  settings_.on_body = [](http_parser* parser, const char* at, size_t length) {
    auto res = reinterpret_cast<Response*>(parser->data);
    res->add_chunk({at, length});
    return 0;
  };

  settings_.on_headers_complete = [](http_parser* parser) {
    auto res = reinterpret_cast<Response*>(parser->data);
    res->version_ = Version{parser->http_major, parser->http_minor};
    res->code_    = static_cast<status_t>(parser->status_code);
    return 0;
  };

  return *this;
}

///////////////////////////////////////////////////////////////////////////////
void Response::execute_parser() noexcept {
  http_parser_init(&parser_, HTTP_RESPONSE);
  parser_.data = this;
  http_parser_execute(&parser_, &settings_, response_.data(), response_.size());
}

} //< namespace http
