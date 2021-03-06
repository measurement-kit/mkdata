#include <string.h>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "mkdata.hpp"

// clang-format off
const uint8_t binary_input[] = {
  0x57, 0xe5, 0x79, 0xfb, 0xa6, 0xbb, 0x0d, 0xbc, 0xce, 0xbd, 0xa7, 0xa0,
  0xba, 0xa4, 0x78, 0x78, 0x12, 0x59, 0xee, 0x68, 0x39, 0xa4, 0x07, 0x98,
  0xc5, 0x3e, 0xbc, 0x55, 0xcb, 0xfe, 0x34, 0x3c, 0x7e, 0x1b, 0x5a, 0xb3,
  0x22, 0x9d, 0xc1, 0x2d, 0x6e, 0xca, 0x5b, 0xf1, 0x10, 0x25, 0x47, 0x1e,
  0x44, 0xe2, 0x2d, 0x60, 0x08, 0xea, 0xb0, 0x0a, 0xcc, 0x05, 0x48, 0xa0,
  0xf5, 0x78, 0x38, 0xf0, 0xdb, 0x3f, 0x9d, 0x9f, 0x25, 0x6f, 0x89, 0x00,
  0x96, 0x93, 0xaf, 0x43, 0xac, 0x4d, 0xc9, 0xac, 0x13, 0xdb, 0x22, 0xbe,
  0x7a, 0x7d, 0xd9, 0x24, 0xa2, 0x52, 0x69, 0xd8, 0x89, 0xc1, 0xd1, 0x57,
  0xaa, 0x04, 0x2b, 0xa2, 0xd8, 0xb1, 0x19, 0xf6, 0xd5, 0x11, 0x39, 0xbb,
  0x80, 0xcf, 0x86, 0xf9, 0x5f, 0x9d, 0x8c, 0xab, 0xf5, 0xc5, 0x74, 0x24,
  0x3a, 0xa2, 0xd4, 0x40, 0x4e, 0xd7, 0x10, 0x1f
};
// clang-format on

TEST_CASE("mkdata allows to check whether input is UTF-8") {
  SECTION("for UTF-8 input") {
    std::string s = "Arturo Filastò vive a Leinì";  // (scusa)
    REQUIRE(mk::data::contains_valid_utf8(s));
  }

  SECTION("for binary input") {
    REQUIRE(!mk::data::contains_valid_utf8(std::string{
          (const char *)binary_input, sizeof(binary_input)}));
  }
}

const char *binary_input_encoded =
    "V+V5+6a7DbzOvaeguqR4eBJZ7mg5pAeYx"
    "T68Vcv+NDx+G1qzIp3BLW7KW/EQJUceRO"
    "ItYAjqsArMBUig9Xg48Ns/nZ8lb4kAlpO"
    "vQ6xNyawT2yK+en3ZJKJSadiJwdFXqgQr"
    "otixGfbVETm7gM+G+V+djKv1xXQkOqLUQ"
    "E7XEB8=";

TEST_CASE("mkdata allows to encode binary input to base64") {
  SECTION("for binary input") {
    std::string data{(const char *)binary_input, sizeof(binary_input)};
    REQUIRE(mk::data::base64_encode(std::move(data)) == binary_input_encoded);
  }
}
