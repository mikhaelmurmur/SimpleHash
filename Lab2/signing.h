#pragma once
#include <utility>

namespace NTL {
    class ZZ;
}

namespace CryptoHelpers
{
    bool Sign(const long long& hash, std::pair<NTL::ZZ, NTL::ZZ>& target);
}
