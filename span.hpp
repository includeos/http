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

#ifndef HTTP_SPAN_HPP
#define HTTP_SPAN_HPP

struct span {
  const char* data;
  size_t      len;
};

inline bool empty(const span& span) noexcept {
  return span.len == 0;
}

inline bool operator==(const span& lhs, const span& rhs) noexcept {
  if (lhs.len not_eq rhs.len) return false;
  return strncmp(lhs.data, rhs.data, lhs.len) == 0;
}

#endif //< HTTP_SPAN_HPP