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
        auto shift = (newRightHalf >> 4) & 0xFFF8000;
        shift <<= 4;
        newRightHalf <<= 13;
        newRightHalf |= shift;
        newRightHalf = And32Bits(newRightHalf);

        input = 0;
        input |= newRightHalf;
    }

    template <typename T>
    T reverse(T n, size_t b = sizeof(T) * 8)
    {
        //        assert(b <= std::numeric_limits<T>::digits);

        T rv = 0;

        for (size_t i = 0; i < b; ++i, n >>= 1)
            rv = (rv >> 1) | (n & 0x01);

        return rv;
    }

    void RoundFunction(const unsigned long long& _input, const unsigned long long& key, unsigned long long& result, bool b = true)
    {
        auto input = _input ^ key;

        unsigned long long newRightHalf(0);

        for (auto byteIndex = 0u; byteIndex < 4; ++byteIndex)
        {
            //newRightHalf <<= 8;
            newRightHalf |= SBOX[(input >> (8 * byteIndex)) & 0xFF] << (8 * byteIndex);
        }

        auto shift = newRightHalf & 0xFFF80000;
        shift >>= 19;
        newRightHalf <<= 13;
        newRightHalf |= shift;
        newRightHalf = (newRightHalf) & 0xFFFFFFFF;

        result = newRightHalf;
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
        g_roundKeysInt[0] = (key >> 32) & 0xFFFFFFFF;
        g_roundKeysInt[1] = key & 0xFFFFFFFF;
        ReverseBytes(4,g_roundKeysInt[0]);
        ReverseBytes(4,g_roundKeysInt[1]);
        g_roundKeysInt[2] = (g_roundKeysInt[1] ^ 0xFFFFFFFF);
        g_roundKeysInt[3] = (g_roundKeysInt[0] ^ 0xFFFFFFFF);
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
        for (auto roundIndex = 0u; roundIndex < 4; ++roundIndex)
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
        for (auto& key : g_roundKeys)
        {
            key = 0;
        }
    }

    
    void Encrypt(const unsigned long long& source, unsigned long long& target, const unsigned long long* key = nullptr)
    {
        GenerateKeys(key);

        auto rightHalf = (source>>32) & 0xFFFFFFFF;
        auto leftHalf = source & 0xFFFFFFFF;
        ReverseBytes(4, rightHalf);
        ReverseBytes(4, leftHalf);
        std::swap(rightHalf, leftHalf);
        target = 0;
        for (auto roundIndex = 0u; roundIndex < 4; ++roundIndex)
        {
            auto tmp = 0ull;
            RoundFunction(rightHalf, g_roundKeysInt[roundIndex], tmp);
            tmp ^= leftHalf;
            rightHalf = leftHalf;
            leftHalf = tmp;
        }

        std::swap(rightHalf, leftHalf);

        target |= rightHalf<<32;
        target |= leftHalf;
        ReverseBytes(8,target);
    }

}
