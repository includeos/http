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
  auto ingress = "POST /greeting.txt HTTP/1.1\r\n"
                 "Host: includeos.server.acorn:5050\r\n"
                 "Accept-Encoding: gzip, deflate, sdch\r\n"
                 "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.6,en;q=0.4\r\n"
                 "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_10_1) "
                 "AppleWebKit/537.36 (KHTML, like Gecko) "
                 "Chrome/39.0.2171.65 Safari/537.36\r\n"
                 "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,"
                 "image/webp,*/*;q=0.8\r\n"
                 "Referer: https://github.com/hioa-cs/IncludeOS\r\n"
                 "Connection: keep-alive\r\n"
                 "Transfer-Encoding: chunked\r\n"
                 "Cache-Control: max-age=0\r\n\r\nb\r\nHello World\r\n5\r\n from\r\n"
                 "a\r\n IncludeOS\r\n0\r\n\r\n"s;

  auto req = http::make_request(move(ingress));

  std::cout << req->get_body() << '\n';

  //--------------------------------------------------------------
  // Response
  //--------------------------------------------------------------
  auto egress = "HTTP/1.1 200 OK\r\n"
                "Server: IncludeOS/Acorn\r\n\r\n"s;

  auto res = http::make_response(move(egress));

  std::cout << res->version() << " " << res->status_code() << '\n'
            << res->header_value("Server") << '\n';
}
