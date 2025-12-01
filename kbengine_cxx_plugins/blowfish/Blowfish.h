#pragma once
#include <cstdint>
#include <cstddef>
#include <array>
#include <vector>
#include <string>

class Blowfish {
public:
    // P-array: 18 32-bit words
    // S-boxes: 4 x 256 32-bit words
    struct Key {
        std::array<uint32_t, 18> P;
        std::array<std::array<uint32_t, 256>, 4> S;
    };

    Blowfish();
    // set key from raw bytes
    void setKey(const uint8_t* key, size_t keyLen);
    void setKeyFromString(const std::string& key); // convenience

    // Encrypt/decrypt a single 64-bit block (in-place)
    // Input as two 32-bit halves (left, right)
    void encryptBlock(uint32_t& left, uint32_t& right) const;
    void decryptBlock(uint32_t& left, uint32_t& right) const;

    // helpers to use 64-bit buffer
    static uint32_t read_u32_be(const uint8_t* p);
    static void write_u32_be(uint8_t* p, uint32_t v);

private:
    Key key_;
    static const Key& initialKeyConstants();
    uint32_t F(uint32_t x) const;
    void keySchedule(const uint8_t* key, size_t keyLen);
};
