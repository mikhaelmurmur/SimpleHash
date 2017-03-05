#pragma once
#include <vector>
#include <NTL/ZZ.h>


namespace CryptoHelpers
{
    void ConvertByteToLong(const std::vector<char>& source, std::vector<NTL::ZZ>& target);

    NTL::ZZ And32Bits(const NTL::ZZ& target);

    NTL::ZZ Xor32Bits(const NTL::ZZ& target);
}
