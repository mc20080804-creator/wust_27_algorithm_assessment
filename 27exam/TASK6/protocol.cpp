#include "protocol.hpp"

#include <cstring>

namespace {

void appendU32(std::vector<std::uint8_t>& frame,
    std::uint32_t value) {//写入内容，直接调用写入时间
    for (int shift = 0; shift < 32; shift += 8) {
        frame.push_back(static_cast<std::uint8_t>((value >> shift) & 0xFF));
    }
}

void appendFloat(std::vector<std::uint8_t>& frame, float value) {//写入角度
    std::uint32_t bits;
    static_assert(sizeof(bits) == sizeof(value));
    std::memcpy(&bits, &value, sizeof(bits));//这里是取地址
    appendU32(frame, bits);
}

}

std::uint16_t crc16(const std::uint8_t* data, std::size_t len) {
    std::uint16_t crc = 0xFFFF;
    for (std::size_t i = 0; i < len; ++i) {
        crc ^= static_cast<std::uint16_t>(data[i] << 8);
        for (int bit = 0; bit < 8; ++bit) {
            const bool highBitSet = (crc & 0x8000) != 0;
            crc = static_cast<std::uint16_t>(crc << 1);
            if (highBitSet) {
                crc ^= 0x1021;
            }
        }
    }

    return crc;
}

std::vector<std::uint8_t> encode(const GimbalCmd& cmd) {
    std::vector<std::uint8_t> frame{0xA5, 13, 0x01};

    appendU32(frame, cmd.time_ms);
    appendFloat(frame, cmd.yaw);
    appendFloat(frame, cmd.pitch);
    frame.push_back(cmd.fire);

    const std::uint16_t crc = crc16(frame.data(), frame.size());
    frame.push_back(static_cast<std::uint8_t>(crc & 0xFF));
    frame.push_back(static_cast<std::uint8_t>(crc >> 8));

    return frame;
}
namespace {

std::uint32_t readU32(const std::uint8_t* bytes) {
    std::uint32_t value = 0;
    for (int i = 0; i < 4; ++i) {
        value |= static_cast<std::uint32_t>(bytes[i]) << (8 * i);
    }
    return value;
}

float readFloat(const std::uint8_t* bytes) {
    const std::uint32_t bits = readU32(bytes);
    float value;
    std::memcpy(&value, &bits, sizeof(value));
    return value;
}

bool crcMatches(const std::uint8_t* frame, std::size_t frameLength) {
    const std::size_t crcOffset = frameLength - 2;
    const auto received = static_cast<std::uint16_t>
    (frame[crcOffset] | (frame[crcOffset + 1] << 8));
    return crc16(frame, crcOffset) == received;
}

}

void Decoder::feed(const std::uint8_t* data, std::size_t len) {
    if (cursor_ != 0) {
        buffer_.erase(buffer_.begin(), buffer_.begin() + cursor_);
        cursor_ = 0;
    }
    if (len != 0) {
        buffer_.insert(buffer_.end(), data, data + len);
    }
}

bool Decoder::next(GimbalState& out) {
    constexpr std::size_t statePayloadLength = 17;
    constexpr std::size_t frameOverhead = 5;
    constexpr std::size_t stateLength = statePayloadLength + frameOverhead;

    while (cursor_ < buffer_.size()) {
        const auto* frame = buffer_.data() + cursor_;
        const std::size_t available = buffer_.size() - cursor_;

        if (frame[0] != 0xA5) {
            ++cursor_;
            continue;
        }
        if (available < 3) {
            return false;
        }

        const std::size_t payloadLength = frame[1];
        const std::uint8_t command = frame[2];
        if (payloadLength > 64 ||
            (command == 0x02 && payloadLength != statePayloadLength) ||
            (command == 0x01 && payloadLength != 13)) {
            ++cursor_;
            continue;
        }

        const std::size_t frameLength = payloadLength + frameOverhead;
        if (available < frameLength) {
            // 长度可能受损，检查后面有没有已经收齐的状态帧。
            std::size_t offset = 1;
            for (; offset + stateLength <= available; ++offset) {
                const auto* candidate = frame + offset;
                if (candidate[0] == 0xA5 &&
                    candidate[1] == statePayloadLength &&
                    candidate[2] == 0x02 &&
                    crcMatches(candidate, stateLength)) {
                    break;
                }
            }
            if (offset + stateLength > available) {
                return false;
            }
            cursor_ += offset;
            continue;
        }

        if (!crcMatches(frame, frameLength)) {
            ++cursor_;
            continue;
        }

        cursor_ += frameLength;
        if (command != 0x02) {
            continue;
        }

        out.time_ms = readU32(frame + 3);
        out.yaw = readFloat(frame + 7);
        out.pitch = readFloat(frame + 11);
        out.bullet_speed = readFloat(frame + 15);
        out.enemy_color = frame[19];
        return true;
    }
    return false;
}