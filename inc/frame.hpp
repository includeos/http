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

#ifndef HTTP_FRAME_HPP
#define HTTP_FRAME_HPP

#include <vector>
#include <cstdint>

#include "common.hpp"

namespace http2 {

/**
 * @brief This class is used to represent a generic frame
 * in the HTTP/2 protocol
 */
class Frame {
private:
  //-----------------------------------------------
  // Internal class type aliases
  //-----------------------------------------------
  using Length  = uint32_t;
  using Type    = uint8_t;
  using Flags   = uint8_t;
  using SID     = uint32_t;
  using Payload = std::vector<uint8_t>;
  //-----------------------------------------------
public:
  /**
   * Constructor to create a new frame
   *
   * @param type    - The type of frame
   * @param flags   - The flags to set on the frame
   * @param id      - The id of the stream
   * @param payload - The data to transport
   */
  explicit Frame(const Type type, const Flags flags, const SID id, const Payload& payload);

  /**
   * @brief Get the size of the payload
   *
   * @return The size of the payload
   */
  uint32_t length() const noexcept;

  /**
   * @brief Get the type of frame
   *
   * @return The type of frame
   */
  uint8_t type() const noexcept;

  /**
   * @brief Get the flags set on the frame
   *
   * @return The flags set on the frame
   */
  uint8_t flags() const noexcept;

  /**
   * @brief Get the id of the stream this frame
   * is associated with
   *
   * @return The id of the stream this frame
   * is associated with
   */
  uint32_t id() const noexcept;

  /**
   * @brief Get a read-only reference to the payload in this
   * frame
   *
   * @return A read-only reference to the payload in this
   * frame
   */
  const Payload& payload() const noexcept;
private:
  static constexpr uint16_t SETTINGS_MIN_FRAME_SIZE {16384};

  //-----------------------------------------------
  // Frame Headers
  //-----------------------------------------------
  Length  length_{};
  Type    type_{};
  Flags   flags_{};
  SID     stream_id_{};

  //-----------------------------------------------
  // Frame Payload
  //-----------------------------------------------
  Payload payload_;
}; //< class Frame

} //< namespace http2

#endif //< HTTP_FRAME_HPP
