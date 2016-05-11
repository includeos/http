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

#include <iostream>

#include <message.hpp>

#include <http_parser.h>

http::Message message;

span field;
span value;

int on_field(http_parser* parser, const char* at, size_t length) {
  (void)parser;
  field.data = at;
  field.len = length;
  return 0;
}

int on_value(http_parser* parser, const char* at, size_t length) {
  (void)parser;
  value.data = at;
  value.len = length;

  message.add_header(field, value);
  return 0;
}

int main() {

  http_parser parser;
  http_parser_init(&parser, HTTP_REQUEST);

  http_parser_settings settings;
  http_parser_settings_init(&settings);
  settings.on_header_field = on_field;
  settings.on_header_value = on_value;

  const char* request = "GET /q?install=yes&machine=x86 HTTP/1.1\r\n"
                        "Host: 128.39.120.91:4300\r\n"
                        "Accept: text/html\r\n\r\n";

  http_parser_execute(&parser, &settings, request, strlen(request));

  message.add_body("Testing 123...");

  std::cout << message;
}
