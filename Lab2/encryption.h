#pragma once

namespace NTL {
    class ZZ;
}

namespace CryptoHelpers
{

    void Encrypt(const NTL::ZZ& source, NTL::ZZ& target);

    void ResetKey();
}
