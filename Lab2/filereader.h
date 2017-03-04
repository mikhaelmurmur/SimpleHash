#pragma once
#include <vector>
#include <NTL/ZZ.h>

namespace CryptoHelpers
{
    bool ReadFile(const std::string& filename,std::vector<char>& data);

    void ConvertByteToLong(const std::vector<char>& source, std::vector<NTL::ZZ>& target);
}
