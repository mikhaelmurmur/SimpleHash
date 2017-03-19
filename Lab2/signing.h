#pragma once
#include <utility>

namespace NTL {
    class ZZ;
}

namespace CryptoHelpers
{
    bool Sign(const long long& hash, NTL::ZZ& signature, NTL::ZZ& y, NTL::ZZ& k, NTL::ZZ& g, NTL::ZZ& u, NTL::ZZ& z, NTL::ZZ& x);
    bool CheckSignature(const long long& hash, const NTL::ZZ& signature, const NTL::ZZ& y, const NTL::ZZ& k);
}
