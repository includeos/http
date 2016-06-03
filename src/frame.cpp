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

#include <frame.hpp>

namespace http2 {

///////////////////////////////////////////////////////////////////////////////
Frame::Frame(const Type type, const Flags flags, const SID id, const Payload& payload)
  : length_{}
  , type_{type}
  , flags_{flags}
  , stream_id_{id}
  , payload_{}
{
  auto payload_size = payload.size();
  if (payload_size > SETTINGS_MAX_FRAME_SIZE) {
    throw "FRAME_SIZE_ERROR";
  }
  payload_ = payload;
  length_  = payload_size;
}

///////////////////////////////////////////////////////////////////////////////
uint32_t Frame::length() const noexcept {
  return length_;
}

///////////////////////////////////////////////////////////////////////////////
uint8_t Frame::type() const noexcept {
  return type_;
}

///////////////////////////////////////////////////////////////////////////////
uint8_t Frame::flags() const noexcept {
  return flags_;
}

///////////////////////////////////////////////////////////////////////////////
uint32_t Frame::id() const noexcept {
  return stream_id_;
}

///////////////////////////////////////////////////////////////////////////////
const Frame::Payload& Frame::payload() const noexcept {
  return payload_;
}

} //< namespace http2
