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

#include <request.hpp>
#include <response.hpp>

int main() {

  using namespace std;

  //--------------------------------------------------------------
  // Request
  //--------------------------------------------------------------
  auto ingress = "GET /q?install=yes&machine=x86 HTTP/1.1\r\n"
                 "Host: 128.39.120.91:4300\r\n"
                 "Accept: */*\r\n\r\n"s;

  auto req = http::make_request(move(ingress));

  std::cout << req->header_value("Host") << '\n';

  //--------------------------------------------------------------
  // Response
  //--------------------------------------------------------------
  auto egress = "HTTP/1.1 200 OK\r\n"
                "Server: IncludeOS/Acorn\r\n\r\n"s;

  auto res = http::make_response(move(egress));

  std::cout << res->version() << " " << res->status_code() << '\n'
            << res->header_value("Server") << '\n';
}
