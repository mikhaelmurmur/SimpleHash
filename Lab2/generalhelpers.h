#pragma once
#include <vector>
#include <NTL/ZZ.h>


namespace CryptoHelpers
{
    void ConvertByteToLong(const std::vector<char>& source, std::vector<NTL::ZZ>& target);

    void ConvertByteToLong(const std::vector<char>& source, std::vector<unsigned long long>& target);

    NTL::ZZ And32Bits(const NTL::ZZ& target);

    NTL::ZZ Xor32Bits(const NTL::ZZ& target);

    bool ConvertHexStringToLong(const std::string& source, NTL::ZZ& target);

    void ReverseBytes(int bytesNumber, unsigned long long& value);
    void ReverseBytes(int bytesNumber, long long& value);

    bool GetRandom(NTL::ZZ& result);
}
