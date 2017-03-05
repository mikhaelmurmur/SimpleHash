#include <NTL/ZZ.h>
#include <vector>
#include "constants.h"
#include "generalhelpers.h"

namespace CryptoHelpers
{
    std::vector<NTL::ZZ> g_roundKeys(4);
    std::vector<unsigned long long> g_roundKeysInt(4);

    void RoundFunction(NTL::ZZ& input, const NTL::ZZ& key)
    {
        input ^= key;

        NTL::ZZ newRightHalf(0);

        for (auto byteIndex = 0u; byteIndex < 4; ++byteIndex)
        {
            unsigned char byte = 0;
            NTL::BytesFromZZ(&byte, (input >> (8 * byteIndex)) & 0xFF, 1);
            newRightHalf |= SBOX[byte] << (8 * byteIndex);
        }
        auto shift = (newRightHalf>>4) & 0xFFF8000;
        shift <<= 4;
        newRightHalf <<= 13;
        newRightHalf |= shift;
        newRightHalf = And32Bits(newRightHalf);

        input = 0;
        input |= newRightHalf;
    }

    void RoundFunction(unsigned long long& _input, const unsigned long long& key)
    {
        auto input =_input^ key;

        unsigned long long newRightHalf(0);

        for (auto byteIndex = 0u; byteIndex < 4; ++byteIndex)
        {
            newRightHalf |= SBOX[(_input >> (8 * byteIndex)) & 0xFF] << (8 * byteIndex);
        }
        auto shift = (newRightHalf >> 4) & 0xFFF8000;
        shift <<= 4;
        newRightHalf <<= 13;
        newRightHalf |= shift;
        newRightHalf = (newRightHalf)& 0xFFFFFFFF;

        _input = 0;
        _input |= newRightHalf;
    }

    void GenerateRoundKeys(const NTL::ZZ& key)
    {
        g_roundKeys[0] = And32Bits(key);
        g_roundKeys[1] = And32Bits(key >> 32);
        g_roundKeys[2] = Xor32Bits(g_roundKeys[0]);
        g_roundKeys[3] = Xor32Bits(g_roundKeys[1]);
    }

    void GenerateRoundKeys(const unsigned long long& key)
    {
        g_roundKeysInt[0] = key& 0xFFFFFFFF;
        g_roundKeysInt[1] = (key >> 32)& 0xFFFFFFF;
        g_roundKeysInt[2] = !(g_roundKeysInt[0]);
        g_roundKeysInt[3] = !(g_roundKeysInt[1]);
    }

    void GenerateKeys(const NTL::ZZ* _key = nullptr)
    {
        if (!_key)
        {
            auto rand = NTL::GetCurrentRandomStream();
            std::vector<unsigned char> randomValue(8);
            rand.get(randomValue.data(), 8);
            NTL::ZZ key(0);
            NTL::ZZFromBytes(key, randomValue.data(), 8);
            GenerateRoundKeys(key);
            return;
        }
        GenerateRoundKeys(*_key);
    }

    void GenerateKeys(const unsigned long long* _key = nullptr)
    {
        if (!_key)
        {
            //auto rand = NTL::GetCurrentRandomStream();
            //std::vector<unsigned char> randomValue(8);
            //rand.get(randomValue.data(), 8);
            //NTL::ZZ key(0);
            //NTL::ZZFromBytes(key, randomValue.data(), 8);
            //GenerateRoundKeys(key);
            return;
        }
        GenerateRoundKeys(*_key);
    }

    void Encrypt(const NTL::ZZ & source, NTL::ZZ & target, const NTL::ZZ* key = nullptr)
    {
        GenerateKeys(key);
        auto rightHalf = And32Bits(source);
        auto leftHalf = And32Bits(source >> 32);
        target = 0;
        for(auto roundIndex = 0u;roundIndex<4;++roundIndex)
        {
            RoundFunction(rightHalf, g_roundKeys[roundIndex]);
            rightHalf ^= leftHalf;
            std::swap(rightHalf, leftHalf);
        }

        std::swap(rightHalf, leftHalf);

        target += 0;
        target |= rightHalf;
        target |= leftHalf << 32;
    }

    void ResetKey()
    {
        for(auto& key : g_roundKeys)
        {
            key = 0;
        }
    }

    void Encrypt(const unsigned long long& source, unsigned long long& target, const unsigned long long* key = nullptr)
    {
        GenerateKeys(key);
        auto rightHalf = source & 0xFFFFFFFF;
        auto leftHalf = (source >> 32) & 0xFFFFFFFF;
        target = 0;
        for (auto roundIndex = 0u; roundIndex<4; ++roundIndex)
        {
            RoundFunction(rightHalf, g_roundKeysInt[roundIndex]);
            rightHalf ^= leftHalf;
            std::swap(rightHalf, leftHalf);
        }

        std::swap(rightHalf, leftHalf);

        target |= rightHalf;
        target |= leftHalf << 32;
    }

}
