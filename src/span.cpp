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

#include <cstring>

#include <span.hpp>

namespace http {

///////////////////////////////////////////////////////////////////////////////
span::span() noexcept
  : span{nullptr, 0}
{}

///////////////////////////////////////////////////////////////////////////////
span::span(const char* data) noexcept
  : span{data, strlen(data)}
{}

///////////////////////////////////////////////////////////////////////////////
span::span(const char* data, const size_t len) noexcept
  : data{data}
  , len{len}
{}

///////////////////////////////////////////////////////////////////////////////
bool span::operator == (const span& other) const noexcept {
  if (len not_eq other.len) return false;
  return strncmp(data, other.data, other.len) == 0;
}

///////////////////////////////////////////////////////////////////////////////
bool span::is_empty() const noexcept {
  return data == nullptr or len == 0;
}

///////////////////////////////////////////////////////////////////////////////
void span::clear() noexcept{
  data = nullptr;
  len = 0;
}

///////////////////////////////////////////////////////////////////////////////
std::string span::to_string() const {
  return std::string{data, len};
}

///////////////////////////////////////////////////////////////////////////////
span::operator std::string () const {
  return  to_string();
}

} //< namespace http
