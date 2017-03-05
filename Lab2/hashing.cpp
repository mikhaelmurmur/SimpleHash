#include "hashing.h"
#include <NTL/ZZ.h>
#include "encryption.h"

namespace CryptoHelpers
{
    void Hash(const std::vector<NTL::ZZ>& source, NTL::ZZ& hashValue)
    {
        hashValue = 0;

        for(auto& chunk: source)
        {
            auto newKey = hashValue^chunk;
            auto currentHash = hashValue;
            Encrypt(currentHash, hashValue, &newKey);
            hashValue ^= currentHash;
        }
    }
}
