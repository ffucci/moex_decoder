#include <gtest/gtest.h>

#include "simba_decoder/simba_decoder.h"
#include "simba_decoder/simba_types.h"

namespace task::tests {

static std::vector<std::byte> TEST_ORDER_UPDATE_DATA = {
    std::byte{0xfb}, std::byte{0xf},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x56}, std::byte{0x0},  std::byte{0x9},  std::byte{0x0},
    std::byte{0xff}, std::byte{0x10}, std::byte{0x8d}, std::byte{0xf5},
    std::byte{0xbf}, std::byte{0xa8}, std::byte{0x8c}, std::byte{0x17},
    std::byte{0x21}, std::byte{0xee}, std::byte{0x8d}, std::byte{0xf5},
    std::byte{0xbf}, std::byte{0xa8}, std::byte{0x8c}, std::byte{0x17},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x0},  std::byte{0x0},
    std::byte{0x32}, std::byte{0x0},  std::byte{0xf},  std::byte{0x0},
    std::byte{0x44}, std::byte{0x4d}, std::byte{0x4},  std::byte{0x0},
    std::byte{0x66}, std::byte{0x4c}, std::byte{0x0},  std::byte{0x0},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x17}, std::byte{0x1c},
    std::byte{0x38}, std::byte{0xb3}, std::byte{0x14}, std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x1},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x1},  std::byte{0x10}, std::byte{0x20}, std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xcd}, std::byte{0x31}, std::byte{0x28}, std::byte{0x0},
    std::byte{0x13}, std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x2},  std::byte{0x31}};

static std::vector<std::byte> TEST_ORDER_EXECUTION_DATA = {
    std::byte{0xfe}, std::byte{0x14}, std::byte{0x0},  std::byte{0x0},
    std::byte{0x76}, std::byte{0x5},  std::byte{0x8},  std::byte{0x0},
    std::byte{0xa0}, std::byte{0x9a}, std::byte{0x22}, std::byte{0x9},
    std::byte{0x91}, std::byte{0xa9}, std::byte{0x8c}, std::byte{0x17},
    std::byte{0xbd}, std::byte{0x61}, std::byte{0x1c}, std::byte{0x9},
    std::byte{0x91}, std::byte{0xa9}, std::byte{0x8c}, std::byte{0x17},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x0},  std::byte{0x0},
    std::byte{0x32}, std::byte{0x0},  std::byte{0xf},  std::byte{0x0},
    std::byte{0x44}, std::byte{0x4d}, std::byte{0x4},  std::byte{0x0},
    std::byte{0xa7}, std::byte{0xcd}, std::byte{0xf},  std::byte{0x0},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x45}, std::byte{0x1a},
    std::byte{0xc0}, std::byte{0xe4}, std::byte{0xfa}, std::byte{0x4e},
    std::byte{0x2},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x90}, std::byte{0x1},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x2},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xd2}, std::byte{0x5a}, std::byte{0x25}, std::byte{0x0},
    std::byte{0x2e}, std::byte{0x2},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x30}, std::byte{0x4a}, std::byte{0x0},
    std::byte{0x10}, std::byte{0x0},  std::byte{0x44}, std::byte{0x4d},
    std::byte{0x4},  std::byte{0x0},  std::byte{0xa7}, std::byte{0xcd},
    std::byte{0xf},  std::byte{0x0},  std::byte{0xf6}, std::byte{0x1a},
    std::byte{0x45}, std::byte{0x1a}, std::byte{0xc0}, std::byte{0xe4},
    std::byte{0xfa}, std::byte{0x4e}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x8f}, std::byte{0x1},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x80}, std::byte{0x8f},
    std::byte{0xf},  std::byte{0x4d}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x1},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xf4}, std::byte{0xf1},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xf6}, std::byte{0x1a},
    std::byte{0x45}, std::byte{0x1a}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x2},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xd2}, std::byte{0x5a},
    std::byte{0x25}, std::byte{0x0},  std::byte{0x2f}, std::byte{0x2},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x1},  std::byte{0x30},
    std::byte{0x4a}, std::byte{0x0},  std::byte{0x10}, std::byte{0x0},
    std::byte{0x44}, std::byte{0x4d}, std::byte{0x4},  std::byte{0x0},
    std::byte{0x89}, std::byte{0xcd}, std::byte{0xf},  std::byte{0x0},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x45}, std::byte{0x1a},
    std::byte{0x80}, std::byte{0x8f}, std::byte{0xf},  std::byte{0x4d},
    std::byte{0x2},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x80}, std::byte{0x8f}, std::byte{0xf},  std::byte{0x4d},
    std::byte{0x2},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x1},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xf4}, std::byte{0xf1}, std::byte{0x0},  std::byte{0x0},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x45}, std::byte{0x1a},
    std::byte{0x1},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x4},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xd2}, std::byte{0x5a}, std::byte{0x25}, std::byte{0x0},
    std::byte{0x30}, std::byte{0x2},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x2},  std::byte{0x31}, std::byte{0x4a}, std::byte{0x0},
    std::byte{0x10}, std::byte{0x0},  std::byte{0x44}, std::byte{0x4d},
    std::byte{0x4},  std::byte{0x0},  std::byte{0xa7}, std::byte{0xcd},
    std::byte{0xf},  std::byte{0x0},  std::byte{0xf6}, std::byte{0x1a},
    std::byte{0x45}, std::byte{0x1a}, std::byte{0xc0}, std::byte{0xe4},
    std::byte{0xfa}, std::byte{0x4e}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x8a}, std::byte{0x1},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xc0}, std::byte{0x9c},
    std::byte{0x12}, std::byte{0x4d}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x5},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xf5}, std::byte{0xf1},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xf6}, std::byte{0x1a},
    std::byte{0x45}, std::byte{0x1a}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x2},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xd2}, std::byte{0x5a},
    std::byte{0x25}, std::byte{0x0},  std::byte{0x31}, std::byte{0x2},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x1},  std::byte{0x30},
    std::byte{0x4a}, std::byte{0x0},  std::byte{0x10}, std::byte{0x0},
    std::byte{0x44}, std::byte{0x4d}, std::byte{0x4},  std::byte{0x0},
    std::byte{0x5e}, std::byte{0xca}, std::byte{0xf},  std::byte{0x0},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x45}, std::byte{0x1a},
    std::byte{0xc0}, std::byte{0x9c}, std::byte{0x12}, std::byte{0x4d},
    std::byte{0x2},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xc0}, std::byte{0x9c}, std::byte{0x12}, std::byte{0x4d},
    std::byte{0x2},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x5},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xf5}, std::byte{0xf1}, std::byte{0x0},  std::byte{0x0},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x45}, std::byte{0x1a},
    std::byte{0x1},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x4},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xd2}, std::byte{0x5a}, std::byte{0x25}, std::byte{0x0},
    std::byte{0x32}, std::byte{0x2},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x2},  std::byte{0x31}, std::byte{0x4a}, std::byte{0x0},
    std::byte{0x10}, std::byte{0x0},  std::byte{0x44}, std::byte{0x4d},
    std::byte{0x4},  std::byte{0x0},  std::byte{0xa7}, std::byte{0xcd},
    std::byte{0xf},  std::byte{0x0},  std::byte{0xf6}, std::byte{0x1a},
    std::byte{0x45}, std::byte{0x1a}, std::byte{0xc0}, std::byte{0xe4},
    std::byte{0xfa}, std::byte{0x4e}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x87}, std::byte{0x1},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x20}, std::byte{0x4b},
    std::byte{0x1d}, std::byte{0x4d}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x3},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xf6}, std::byte{0xf1},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xf6}, std::byte{0x1a},
    std::byte{0x45}, std::byte{0x1a}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x2},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xd2}, std::byte{0x5a},
    std::byte{0x25}, std::byte{0x0},  std::byte{0x33}, std::byte{0x2},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x1},  std::byte{0x30},
    std::byte{0x4a}, std::byte{0x0},  std::byte{0x10}, std::byte{0x0},
    std::byte{0x44}, std::byte{0x4d}, std::byte{0x4},  std::byte{0x0},
    std::byte{0xe8}, std::byte{0xc2}, std::byte{0xf},  std::byte{0x0},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x45}, std::byte{0x1a},
    std::byte{0x20}, std::byte{0x4b}, std::byte{0x1d}, std::byte{0x4d},
    std::byte{0x2},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x20}, std::byte{0x4b}, std::byte{0x1d}, std::byte{0x4d},
    std::byte{0x2},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x3},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xf6}, std::byte{0xf1}, std::byte{0x0},  std::byte{0x0},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x45}, std::byte{0x1a},
    std::byte{0x1},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x4},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xd2}, std::byte{0x5a}, std::byte{0x25}, std::byte{0x0},
    std::byte{0x34}, std::byte{0x2},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x2},  std::byte{0x31}, std::byte{0x4a}, std::byte{0x0},
    std::byte{0x10}, std::byte{0x0},  std::byte{0x44}, std::byte{0x4d},
    std::byte{0x4},  std::byte{0x0},  std::byte{0xa7}, std::byte{0xcd},
    std::byte{0xf},  std::byte{0x0},  std::byte{0xf6}, std::byte{0x1a},
    std::byte{0x45}, std::byte{0x1a}, std::byte{0xc0}, std::byte{0xe4},
    std::byte{0xfa}, std::byte{0x4e}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x82}, std::byte{0x1},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xc0}, std::byte{0xd1},
    std::byte{0x1e}, std::byte{0x4d}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x5},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xf7}, std::byte{0xf1},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xf6}, std::byte{0x1a},
    std::byte{0x45}, std::byte{0x1a}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x2},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xd2}, std::byte{0x5a},
    std::byte{0x25}, std::byte{0x0},  std::byte{0x35}, std::byte{0x2},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x1},  std::byte{0x30},
    std::byte{0x4a}, std::byte{0x0},  std::byte{0x10}, std::byte{0x0},
    std::byte{0x44}, std::byte{0x4d}, std::byte{0x4},  std::byte{0x0},
    std::byte{0x6c}, std::byte{0xc2}, std::byte{0xf},  std::byte{0x0},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x45}, std::byte{0x1a},
    std::byte{0xc0}, std::byte{0xd1}, std::byte{0x1e}, std::byte{0x4d},
    std::byte{0x2},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xc0}, std::byte{0xd1}, std::byte{0x1e}, std::byte{0x4d},
    std::byte{0x2},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x5},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xf7}, std::byte{0xf1}, std::byte{0x0},  std::byte{0x0},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x45}, std::byte{0x1a},
    std::byte{0x1},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x4},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xd2}, std::byte{0x5a}, std::byte{0x25}, std::byte{0x0},
    std::byte{0x36}, std::byte{0x2},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x2},  std::byte{0x31}, std::byte{0x4a}, std::byte{0x0},
    std::byte{0x10}, std::byte{0x0},  std::byte{0x44}, std::byte{0x4d},
    std::byte{0x4},  std::byte{0x0},  std::byte{0xa7}, std::byte{0xcd},
    std::byte{0xf},  std::byte{0x0},  std::byte{0xf6}, std::byte{0x1a},
    std::byte{0x45}, std::byte{0x1a}, std::byte{0xc0}, std::byte{0xe4},
    std::byte{0xfa}, std::byte{0x4e}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x81}, std::byte{0x1},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x60}, std::byte{0x58},
    std::byte{0x20}, std::byte{0x4d}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x1},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xf8}, std::byte{0xf1},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xf6}, std::byte{0x1a},
    std::byte{0x45}, std::byte{0x1a}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x2},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xd2}, std::byte{0x5a},
    std::byte{0x25}, std::byte{0x0},  std::byte{0x37}, std::byte{0x2},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x1},  std::byte{0x30},
    std::byte{0x4a}, std::byte{0x0},  std::byte{0x10}, std::byte{0x0},
    std::byte{0x44}, std::byte{0x4d}, std::byte{0x4},  std::byte{0x0},
    std::byte{0xdf}, std::byte{0xbd}, std::byte{0xf},  std::byte{0x0},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x45}, std::byte{0x1a},
    std::byte{0x60}, std::byte{0x58}, std::byte{0x20}, std::byte{0x4d},
    std::byte{0x2},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x60}, std::byte{0x58}, std::byte{0x20}, std::byte{0x4d},
    std::byte{0x2},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x1},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xf8}, std::byte{0xf1}, std::byte{0x0},  std::byte{0x0},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x45}, std::byte{0x1a},
    std::byte{0x1},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x4},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xd2}, std::byte{0x5a}, std::byte{0x25}, std::byte{0x0},
    std::byte{0x38}, std::byte{0x2},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x2},  std::byte{0x31}, std::byte{0x4a}, std::byte{0x0},
    std::byte{0x10}, std::byte{0x0},  std::byte{0x44}, std::byte{0x4d},
    std::byte{0x4},  std::byte{0x0},  std::byte{0xa7}, std::byte{0xcd},
    std::byte{0xf},  std::byte{0x0},  std::byte{0xf6}, std::byte{0x1a},
    std::byte{0x45}, std::byte{0x1a}, std::byte{0xc0}, std::byte{0xe4},
    std::byte{0xfa}, std::byte{0x4e}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x80}, std::byte{0x1},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0xdf},
    std::byte{0x21}, std::byte{0x4d}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x1},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xf9}, std::byte{0xf1},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xf6}, std::byte{0x1a},
    std::byte{0x45}, std::byte{0x1a}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x2},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xd2}, std::byte{0x5a},
    std::byte{0x25}, std::byte{0x0},  std::byte{0x39}, std::byte{0x2},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x1},  std::byte{0x30},
    std::byte{0x4a}, std::byte{0x0},  std::byte{0x10}, std::byte{0x0},
    std::byte{0x44}, std::byte{0x4d}, std::byte{0x4},  std::byte{0x0},
    std::byte{0x6},  std::byte{0x9a}, std::byte{0xf},  std::byte{0x0},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x45}, std::byte{0x1a},
    std::byte{0x0},  std::byte{0xdf}, std::byte{0x21}, std::byte{0x4d},
    std::byte{0x2},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0xdf}, std::byte{0x21}, std::byte{0x4d},
    std::byte{0x2},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x1},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xf9}, std::byte{0xf1}, std::byte{0x0},  std::byte{0x0},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x45}, std::byte{0x1a},
    std::byte{0x1},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x4},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xd2}, std::byte{0x5a}, std::byte{0x25}, std::byte{0x0},
    std::byte{0x3a}, std::byte{0x2},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x2},  std::byte{0x31}, std::byte{0x4a}, std::byte{0x0},
    std::byte{0x10}, std::byte{0x0},  std::byte{0x44}, std::byte{0x4d},
    std::byte{0x4},  std::byte{0x0},  std::byte{0xa7}, std::byte{0xcd},
    std::byte{0xf},  std::byte{0x0},  std::byte{0xf6}, std::byte{0x1a},
    std::byte{0x45}, std::byte{0x1a}, std::byte{0xc0}, std::byte{0xe4},
    std::byte{0xfa}, std::byte{0x4e}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x7f}, std::byte{0x1},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0xdf},
    std::byte{0x21}, std::byte{0x4d}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x1},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xfa}, std::byte{0xf1},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xf6}, std::byte{0x1a},
    std::byte{0x45}, std::byte{0x1a}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x2},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xd2}, std::byte{0x5a},
    std::byte{0x25}, std::byte{0x0},  std::byte{0x3b}, std::byte{0x2},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x1},  std::byte{0x30},
    std::byte{0x4a}, std::byte{0x0},  std::byte{0x10}, std::byte{0x0},
    std::byte{0x44}, std::byte{0x4d}, std::byte{0x4},  std::byte{0x0},
    std::byte{0x58}, std::byte{0xac}, std::byte{0xf},  std::byte{0x0},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x45}, std::byte{0x1a},
    std::byte{0x0},  std::byte{0xdf}, std::byte{0x21}, std::byte{0x4d},
    std::byte{0x2},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0xdf}, std::byte{0x21}, std::byte{0x4d},
    std::byte{0x2},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x1},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xfa}, std::byte{0xf1}, std::byte{0x0},  std::byte{0x0},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x45}, std::byte{0x1a},
    std::byte{0x1},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x4},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xd2}, std::byte{0x5a}, std::byte{0x25}, std::byte{0x0},
    std::byte{0x3c}, std::byte{0x2},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x2},  std::byte{0x31}, std::byte{0x4a}, std::byte{0x0},
    std::byte{0x10}, std::byte{0x0},  std::byte{0x44}, std::byte{0x4d},
    std::byte{0x4},  std::byte{0x0},  std::byte{0xa7}, std::byte{0xcd},
    std::byte{0xf},  std::byte{0x0},  std::byte{0xf6}, std::byte{0x1a},
    std::byte{0x45}, std::byte{0x1a}, std::byte{0xc0}, std::byte{0xe4},
    std::byte{0xfa}, std::byte{0x4e}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x7d}, std::byte{0x1},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0xdf},
    std::byte{0x21}, std::byte{0x4d}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xfb}, std::byte{0xf1},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xf6}, std::byte{0x1a},
    std::byte{0x45}, std::byte{0x1a}, std::byte{0x2},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x2},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0xd2}, std::byte{0x5a},
    std::byte{0x25}, std::byte{0x0},  std::byte{0x3d}, std::byte{0x2},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x1},  std::byte{0x30},
    std::byte{0x4a}, std::byte{0x0},  std::byte{0x10}, std::byte{0x0},
    std::byte{0x44}, std::byte{0x4d}, std::byte{0x4},  std::byte{0x0},
    std::byte{0x18}, std::byte{0xad}, std::byte{0xf},  std::byte{0x0},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x45}, std::byte{0x1a},
    std::byte{0x0},  std::byte{0xdf}, std::byte{0x21}, std::byte{0x4d},
    std::byte{0x2},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0xdf}, std::byte{0x21}, std::byte{0x4d},
    std::byte{0x2},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x2},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xfb}, std::byte{0xf1}, std::byte{0x0},  std::byte{0x0},
    std::byte{0xf6}, std::byte{0x1a}, std::byte{0x45}, std::byte{0x1a},
    std::byte{0x1},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x4},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x0},  std::byte{0x0},  std::byte{0x0},  std::byte{0x0},
    std::byte{0xd2}, std::byte{0x5a}, std::byte{0x25}, std::byte{0x0},
    std::byte{0x3e}, std::byte{0x2},  std::byte{0x0},  std::byte{0x0},
    std::byte{0x2},  std::byte{0x31},
};

class SIMBADecoderTestFixture : public ::testing::Test {
  void SetUp() override {}

 protected:
  simba::decoder::MessageHandlers message_handlers;
};

TEST_F(
    SIMBADecoderTestFixture,
    GIVEN_byte_array_WHEN_decoding_single_order_update_THEN_decode_correctly) {
  simba::decoder::MessageHandlers message_handlers;
  std::vector<simba::types::OrderUpdate> decoded_orders;
  message_handlers.order_update_handler =
      [&decoded_orders](const simba::types::OrderUpdate &order_update) {
        decoded_orders.push_back(order_update);
      };
  task::simba::decoder::SIMBADecoder simba_decoder_{message_handlers};

  simba_decoder_.decode_message(TEST_ORDER_UPDATE_DATA);
  EXPECT_EQ(decoded_orders.size(), 1);
  EXPECT_EQ(simba_decoder_.market_header().message_size, 86);
  EXPECT_EQ(simba_decoder_.sbe_header().template_id, 15);
  EXPECT_EQ(simba_decoder_.sbe_header().block_length, 50);

  auto order = decoded_orders.back();
  EXPECT_EQ(order.order_id, 2024116201390623846);
  EXPECT_EQ(order.order_price, 1356600);
  EXPECT_EQ(order.order_volume, 1);
  EXPECT_EQ(order.rpt_seq, 19);
  EXPECT_EQ(order.security_id, 2634189);
  EXPECT_EQ(order.action, simba::types::MDUpdateAction::Delete);
  EXPECT_EQ(order.side, simba::types::MDEntryType::Offer);
}

TEST_F(SIMBADecoderTestFixture,
       GIVEN_byte_array_WHEN_decoding_long_message_THEN_decode_correctly) {
  simba::decoder::MessageHandlers message_handlers;
  std::vector<simba::types::OrderExecution> decoded_orders;
  std::vector<simba::types::OrderUpdate> decoded_order_update;
  message_handlers.order_update_handler =
      [&decoded_order_update](const simba::types::OrderUpdate &order_update) {
        decoded_order_update.push_back(order_update);
      };
  message_handlers.order_execution_handler =
      [&decoded_orders](const simba::types::OrderExecution &order_update) {
        decoded_orders.push_back(order_update);
      };
  task::simba::decoder::SIMBADecoder simba_decoder_{message_handlers};

  simba_decoder_.decode_message(TEST_ORDER_EXECUTION_DATA);
  EXPECT_EQ(decoded_orders.size(), 16);
  EXPECT_EQ(decoded_order_update.size(), 1);

  const auto order = decoded_order_update.back();
  EXPECT_EQ(order.order_id, 1892948862244474279);
  EXPECT_EQ(order.order_price,
            9915000000);  // for the multiplier we need to divide 1e5
  EXPECT_EQ(order.order_volume, 400);
  EXPECT_EQ(order.action, simba::types::MDUpdateAction::New);
  EXPECT_EQ(order.side, simba::types::MDEntryType::Bid);

  EXPECT_EQ(simba_decoder_.market_header().sequence_number, 5374);
  EXPECT_EQ(simba_decoder_.market_header().message_size, 1398);

  EXPECT_EQ(simba_decoder_.sbe_header().template_id, 16);
  EXPECT_EQ(simba_decoder_.sbe_header().block_length, 74);
  EXPECT_EQ(order.side, simba::types::MDEntryType::Bid);

  const auto execution = decoded_orders[0];
  EXPECT_EQ(execution.order_id, 1892948862244474279);
  EXPECT_EQ(execution.order_price,
            9915000000);  // for the multiplier we need to divide 1e5
  EXPECT_EQ(execution.remaining_quantity, 399);
  EXPECT_EQ(execution.trade_price, 9882800000);
  EXPECT_EQ(execution.trade_volume, 1);
  EXPECT_EQ(execution.security_id, 2448082);
  EXPECT_EQ(execution.side, simba::types::MDEntryType::Bid);

  const auto execution2 = decoded_orders[1];
  EXPECT_EQ(execution2.order_id, 1892948862244474249);
  EXPECT_EQ(execution2.order_price,
            9882800000);  // for the multiplier we need to divide 1e5
  EXPECT_EQ(execution2.remaining_quantity, 0);
  EXPECT_EQ(execution2.trade_price, 9882800000);
  EXPECT_EQ(execution2.trade_volume, 1);
  EXPECT_EQ(execution2.security_id, 2448082);
  EXPECT_EQ(execution2.side, simba::types::MDEntryType::Offer);
}

}  // namespace task::tests
