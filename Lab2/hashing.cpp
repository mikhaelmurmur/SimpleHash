#include "hashing.h"
#include <NTL/ZZ.h>
#include "encryption.h"

namespace CryptoHelpers
{
    void Hash(const std::vector<NTL::ZZ>& source, NTL::ZZ& hashValue)
    {
        hashValue = 0;

        for(const auto& chunk: source)
        {
            auto newKey = hashValue^chunk;
            auto currentHash = hashValue;
            Encrypt(currentHash, hashValue, &newKey);
            hashValue ^= currentHash;
        }
    }

    void Hash(const std::vector<unsigned long long>& source, unsigned long long& hashValue)
    {
        hashValue = 0;

        for (const auto& chunk : source)
        {
            auto newKey = hashValue^chunk;
            auto currentHash = hashValue;
            Encrypt(currentHash, hashValue, &newKey);
            hashValue ^= currentHash;
        }
    }
}
