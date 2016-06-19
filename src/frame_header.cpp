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

#include <frame_header.hpp>

namespace http2 {

///////////////////////////////////////////////////////////////////////////////
Frame_header::Frame_header(const Length length, const Type type,
                           const Flags  flags,  const SID id) noexcept
  : length_{}
  , type_{}
  , flags_{flags}
  , stream_id_{id}
{
  if (length > SETTINGS_MAX_FRAME_SIZE) {
    type_ = Type::INVALID;
    return;
  }

  length_ = length;
  type_ = type;
}

///////////////////////////////////////////////////////////////////////////////
uint32_t Frame_header::length() const noexcept {
  return length_;
}

///////////////////////////////////////////////////////////////////////////////
Type Frame_header::type() const noexcept {
  return type_;
}

///////////////////////////////////////////////////////////////////////////////
Frame_header::Flags Frame_header::flags() const noexcept {
  return flags_;
}

///////////////////////////////////////////////////////////////////////////////
uint32_t Frame_header::id() const noexcept {
  return stream_id_;
}

///////////////////////////////////////////////////////////////////////////////
bool Frame_header::valid() const noexcept {
  return not (type_ == Type::INVALID);
}

} //< namespace http2
