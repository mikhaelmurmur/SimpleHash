#pragma once
#include <memory>

namespace NTL {
    class ZZ;
}

namespace CryptoHelpers
{
    void Encrypt(const NTL::ZZ& source, NTL::ZZ& target,const NTL::ZZ* key = nullptr);

    void ResetKey();
}
