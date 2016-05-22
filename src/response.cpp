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

#include <http_parser.h>

namespace http {

static void configure_settings(http_parser_settings&) noexcept;

static void execute_parser(Response*, http_parser*, http_parser_settings*, const std::string&) noexcept;

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
  http_parser          _parser;
  http_parser_settings _settings;

  configure_settings(_settings);
  execute_parser(this, &_parser, &_settings, response_);
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
  std::ostringstream response;
  //-----------------------------------
  response << version_ << " " << code_ << " "
           << code_description(code_)  << "\r\n"
           << Message::to_string();
  //-----------------------------------
  return response.str();
}

///////////////////////////////////////////////////////////////////////////////
Response::operator std::string () const {
  return to_string();
}

///////////////////////////////////////////////////////////////////////////////
static void configure_settings(http_parser_settings& settings_) noexcept {
  http_parser_settings_init(&settings_);

  settings_.on_header_field = [](http_parser* parser, const char* at, size_t length) {
    auto res = reinterpret_cast<Response*>(parser->data);
    res->field().data = at;
    res->field().len  = length;
    return 0;
  };

  settings_.on_header_value = [](http_parser* parser, const char* at, size_t length) {
    auto res = reinterpret_cast<Response*>(parser->data);
    res->add_header(res->field(), {at, length});
    return 0;
  };

  settings_.on_body = [](http_parser* parser, const char* at, size_t length) {
    auto res = reinterpret_cast<Response*>(parser->data);
    res->add_chunk({at, length});
    return 0;
  };

  settings_.on_headers_complete = [](http_parser* parser) {
    auto res = reinterpret_cast<Response*>(parser->data);
    res->set_version(Version{parser->http_major, parser->http_minor});
    res->set_status_code(static_cast<status_t>(parser->status_code));
    return 0;
  };
}

///////////////////////////////////////////////////////////////////////////////
static void execute_parser(Response* res_, http_parser* parser_, http_parser_settings* settings_,
                           const std::string& data_) noexcept {
  http_parser_init(parser_, HTTP_RESPONSE);
  parser_->data = res_;
  http_parser_execute(parser_, settings_, data_.data(), data_.size());
}

///////////////////////////////////////////////////////////////////////////////
Response& operator << (Response& res, const HeaderSet& headers) {
  for (const auto& field : headers) {
    res.add_header(field.first, field.second);
  }
  return res;
}

} //< namespace http
