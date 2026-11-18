#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "packets.hpp"

std::uint16_t crc16(const std::uint8_t* data, std::size_t len);
std::vector<std::uint8_t> encode(const GimbalCmd& cmd);

class Decoder {
private:
    std::vector<std::uint8_t> buffer_;
    std::size_t cursor_ = 0;
public:
    void feed(const std::uint8_t* data, std::size_t len);
    bool next(GimbalState& out);
};