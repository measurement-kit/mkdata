// Part of Measurement Kit <https://measurement-kit.github.io/>.
// Measurement Kit is free software under the BSD license. See AUTHORS
// and LICENSE for more information on the copying conditions.
#ifndef MEASUREMENT_KIT_MKDATA_HPP
#define MEASUREMENT_KIT_MKDATA_HPP

#include <string>

namespace mk {
namespace data {

/// contains_valid_utf8 returns true if the @p data string contains a valid
/// UTF-8 sequence of bytes, and returns false otherwise.
bool contains_valid_utf8(const std::string &data) noexcept;

/// base64_encode encodes @p data as base64. Returns the encoded string.
std::string base64_encode(std::string data) noexcept;

}  // namespace data
}  // namespace mk

// MKDATA_INLINE_IMPL controls whether to include the inline implementation.
#ifdef MKDATA_INLINE_IMPL

namespace mk {
namespace data {

// === BEGIN{ http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ ===
// clang-format off
//
// Portions Copyright (c) 2008-2009 Bjoern Hoehrmann <bjoern@hoehrmann.de>
// See http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ for details.

#define UTF8_ACCEPT 0
#define UTF8_REJECT 1

static const uint8_t mkdata_utf8d[] = {
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 00..1f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 20..3f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 40..5f
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0, // 60..7f
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9,9, // 80..9f
  7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7, // a0..bf
  8,8,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2, // c0..df
  0xa,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x4,0x3,0x3, // e0..ef
  0xb,0x6,0x6,0x6,0x5,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8, // f0..ff
  0x0,0x1,0x2,0x3,0x5,0x8,0x7,0x1,0x1,0x1,0x4,0x6,0x1,0x1,0x1,0x1, // s0..s0
  1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,1,1,1,1,0,1,0,1,1,1,1,1,1, // s1..s2
  1,2,1,1,1,1,1,2,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1, // s3..s4
  1,2,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,1,1,1,3,1,3,1,1,1,1,1,1, // s5..s6
  1,3,1,1,1,1,1,3,1,3,1,1,1,1,1,1,1,3,1,1,1,1,1,1,1,1,1,1,1,1,1,1, // s7..s8
};

static uint32_t inline
mkdata_decode_utf8(uint32_t* state, uint32_t* codep, uint32_t byte) noexcept {
  uint32_t type = mkdata_utf8d[byte];

  *codep = (*state != UTF8_ACCEPT) ?
    (byte & 0x3fu) | (*codep << 6) :
    (0xff >> type) & (byte);

  *state = mkdata_utf8d[256 + *state*16 + type];
  return *state;
}

// clang-format on
// === }END http://bjoern.hoehrmann.de/utf-8/decoder/dfa/ ===

bool contains_valid_utf8(const std::string &data) noexcept {
  uint32_t codepoint{};
  uint32_t state{};
  for (size_t i = 0; i < data.size(); ++i) {
    (void)mkdata_decode_utf8(&state, &codepoint, (uint8_t)data[i]);
  }
  return state == UTF8_ACCEPT;
}

// === BEGIN{ René Nyffenegger BASE64 code ===
/*-
 * base64.cpp and base64.h
 *
 * Portions Copyright (C) 2004-2008 René Nyffenegger
 *
 * This source code is provided 'as-is', without any express or implied
 * warranty. In no event will the author be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this source code must not be misrepresented; you must not
 *    claim that you wrote the original source code. If you use this source code
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 *
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original source code.
 *
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * René Nyffenegger rene.nyffenegger@adp-gmbh.ch
 */

static inline std::string b64_encode(
    const uint8_t *base, size_t len) noexcept {
  static const std::string b64_table =
      "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
      "abcdefghijklmnopqrstuvwxyz"
      "0123456789+/";
  std::string res;
  uint8_t in[3];
  uint8_t out[4];
  int state = 0;
  auto copy = [&]() {
    // The following code has been reorganized to avoid -Wconversion
    // -Werror error generated by Clang on macOS, as well as to avoid
    // -Werror=conversion as generated by GCC on GNU/Linux
    {
      auto n = (in[0] & 0xfc) >> 2;
      assert(n <= 0xff);
      out[0] = (uint8_t)n;
    }
    {
      auto n = ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4);
      assert(n <= 0xff);
      out[1] = (uint8_t)n;
    }
    {
      auto n = ((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6);
      assert(n <= 0xff);
      out[2] = (uint8_t)n;
    }
    {
      auto n = in[2] & 0x3f;
      assert(n <= 0xff);
      out[3] = (uint8_t)n;
    }
    for (int idx = 0; idx < 4; ++idx) {
      res += (idx <= state) ? b64_table[out[idx]] : '=';
    }
  };
  while (len-- > 0) {
    in[state++] = *(base++);
    if (state == 3) {
      copy();
      state = 0;
    }
  }
  if (state != 0) {
    for (int idx = state; idx < 3; ++idx) {
      in[idx] = '\0';
    }
    copy();
  }
  return res;
}
// === }END René Nyffenegger BASE64 code ===

std::string base64_encode(std::string data) noexcept {
  return b64_encode((const uint8_t *)data.c_str(), data.size());
}

}  // namespace data
}  // namespace mk
#endif  // MKDATA_INLINE_IMPL
#endif  // MEASUREMENT_KIT_MKDATA_HPP
