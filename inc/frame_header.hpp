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

#ifndef HTTP_FRAME_HEADER_HPP
#define HTTP_FRAME_HEADER_HPP

#include <cstdint>

/*
 * @macro
 *
 * Symbol to specify the maximum size of the payload in
 * a frame
 */
#ifndef SETTINGS_MAX_FRAME_SIZE
#define SETTINGS_MAX_FRAME_SIZE 16777215
#endif

namespace http2 {

/**
 * @brief This enum consist of the mappings between
 * frame labels to their respective codes
 */
enum class Type : uint8_t {
  DATA,
  HEADERS,
  PRIORITY,
  RST_STREAM,
  SETTINGS,
  PUSH_PROMISE,
  PING,
  GOAWAY,
  WINDOW_UPDATE,
  CONTINUATION,
  INVALID //< Internal symbol used to identify an erroneous type
}; //< enum Type

/**
 * @brief This enum consist of the valid flags that can be set
 * on a frame
 */
enum Flag : uint8_t {
  NONE        = 0x00,
  ACK         = 0x01,
  END_STREAM  = 0x01,
  END_HEADERS = 0x04,
  PADDED      = 0x08,
  PRIORITY    = 0x20
}; //< enum Flag

/**
 * @brief This class is used to represent a frame header
 * in the HTTP/2 protocol
 */
class Frame_header {
private:
  //-----------------------------------------------
  // Internal class type aliases
  //-----------------------------------------------
  using Length  = uint32_t;
  using Flags   = uint8_t;
  using SID     = uint32_t;
  //-----------------------------------------------
public:
  /**
   * @brief Constructor
   *
   * @param length - The length of the payload
   * @param type   - The type of frame
   * @param flags  - The flags to set on the frame
   * @param id     - The id of the associated stream
   */
  explicit Frame_header(const Length length = 0U, const Type type = Type::INVALID,
                        const Flags flags = NONE, const SID id = 0U) noexcept;

  /**
   * @brief Get the size of the payload
   *
   * @return The size of the payload
   */
  Length length() const noexcept;

  /**
   * @brief Get the type of frame
   *
   * @return The type of frame
   */
  Type type() const noexcept;

  /**
   * @brief Get the flags set on the frame
   *
   * @return The flags set on the frame
   */
  Flags flags() const noexcept;

  /**
   * @brief Get the id of the stream this frame
   * is associated with
   *
   * @return The id of the stream this frame
   * is associated with
   */
  SID id() const noexcept;

  /**
   * @brief Check if the frame is valid
   *
   * @return true if the frame is valid, false otherwise
   */
  bool valid() const noexcept;
private:
  static constexpr uint16_t SETTINGS_MIN_FRAME_SIZE {16384};

  //-----------------------------------------------
  // Class Data Members
  //-----------------------------------------------
  Length  length_;
  Type    type_;
  Flags   flags_;
  SID     stream_id_;
  //-----------------------------------------------
}; //< class Frame_header

} //< namespace http2

#endif //< HTTP_FRAME_HEADER_HPP
