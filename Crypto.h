#pragma once
#include <vector>
#include <cstdint>

std::vector<uint8_t> Xor(const std::vector<uint8_t>& buffer, uint8_t key)
{
    std::vector<uint8_t> result;
    result.reserve(buffer.size());

    for (uint8_t b : buffer) {
        result.push_back(b ^ key);
    }

    return result;
}


inline uint64_t FNV1a_HashBuffer(const std::vector<uint8_t>& data)
{
    const uint64_t FNV_OFFSET_BASIS = 0xcbf29ce484222325ULL;
    const uint64_t FNV_PRIME = 0x100000001b3ULL;

    uint64_t hash = FNV_OFFSET_BASIS;

    for (uint8_t b : data)
    {
        hash ^= b;
        hash *= FNV_PRIME;
    }

    return hash;
}


inline uint64_t FNV1a_HashFile(const std::string& path)
{
    const uint64_t FNV_OFFSET_BASIS = 0xcbf29ce484222325ULL;
    const uint64_t FNV_PRIME = 0x100000001b3ULL;

    uint64_t hash = FNV_OFFSET_BASIS;

    std::ifstream file(path, std::ios::binary);
    if (!file)
        return 0;

    char buffer[4096];
    while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0)
    {
        std::streamsize count = file.gcount();
        for (std::streamsize i = 0; i < count; ++i)
        {
            hash ^= static_cast<uint8_t>(buffer[i]);
            hash *= FNV_PRIME;
        }
    }

    return hash;
}
