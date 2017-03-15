#pragma once
#include <utility>

namespace NTL {
    class ZZ;
}

namespace CryptoHelpers
{
    bool Sign(const long long& hash, NTL::ZZ& signature, NTL::ZZ& y, NTL::ZZ& k, NTL::ZZ& g, NTL::ZZ& u, NTL::ZZ& z);
}
