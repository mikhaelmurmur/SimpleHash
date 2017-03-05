#include <NTL/ZZ.h>
#include <vector>
#include "constants.h"
#include "generalhelpers.h"

namespace CryptoHelpers
{
    std::vector<NTL::ZZ> g_roundKeys(4);

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

    void GenerateKeys()
    {
        auto rand = NTL::GetCurrentRandomStream();
        std::vector<unsigned char> randomValue(8);
        rand.get(randomValue.data(), 8);
        NTL::ZZ key(0);
        NTL::ZZFromBytes(key, randomValue.data(),8);
        g_roundKeys[0] = And32Bits(key);
        g_roundKeys[1] = And32Bits(key >> 32);
        g_roundKeys[2] = Xor32Bits(g_roundKeys[0]);
        g_roundKeys[3] = Xor32Bits(g_roundKeys[1]);
    }

    void Encrypt(const NTL::ZZ & source, NTL::ZZ & target)
    {
        GenerateKeys();
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
}
