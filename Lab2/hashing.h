#pragma once
#include <vector>

namespace NTL {
    class ZZ;
}

namespace CryptoHelpers
{
    void Hash(const std::vector<NTL::ZZ>& source, NTL::ZZ& hashValue);
}
