#include "signing.h"
#include <NTL/ZZ.h>
#include "generalhelpers.h"

namespace CryptoHelpers
{
    void ModifyHashToLong(const long long& hash, NTL::ZZ& longModifiedHash)
    {
        longModifiedHash = 0;
        auto rhash = hash;
        ReverseBytes(8, rhash);
        ConvertHexStringToLong("00FFFFFFFFFFFF00", longModifiedHash); 

#pragma region dirtyhackforntl
        longModifiedHash *= NTL::power2_ZZ(16);
        longModifiedHash += ((rhash >> 48) & 0xFFFF);
        longModifiedHash *= NTL::power2_ZZ(16);
        longModifiedHash += ((rhash >> 32) & 0xFFFF);
        longModifiedHash *= NTL::power2_ZZ(16);
        longModifiedHash += ((rhash >> 16) & 0xFFFF);
        longModifiedHash *= NTL::power2_ZZ(16);
        longModifiedHash += ((rhash) & 0xFFFF);

#pragma endregion
    }

    bool CheckSignature(const long long& hash,const NTL::ZZ& signature,const NTL::ZZ& y,const NTL::ZZ& k)
    {
        NTL::ZZ a(0);
        NTL::ZZ p(0);
        ConvertHexStringToLong("9E93A4096E5416CED0242228014B67B5", a);
        ConvertHexStringToLong("AF5228967057FE1CB84B92511BE89A47", p);
        
        NTL::ZZ longModifiedHash;
        ModifyHashToLong(hash, longModifiedHash);
        NTL::ZZ tmp1(0);
        NTL::PowerMod(tmp1,signature, longModifiedHash, p);
        NTL::ZZ tmp2(0);
        NTL::ZZ tmp3(0);
        NTL::ZZ tmp4(0);    
        NTL::PowerMod(tmp3,a, k, p);
        NTL::MulMod(tmp4, signature, tmp3, p);
        NTL::PowerMod(tmp2,y, (tmp4), p);

        if (tmp2 == tmp1)
        {
            return true;
        }
        return false;
    }

    bool Sign(const long long& hash, NTL::ZZ& signature, NTL::ZZ& y, NTL::ZZ& k, NTL::ZZ& g, NTL::ZZ& rnd, NTL::ZZ& z, NTL::ZZ& x)
    {
        NTL::ZZ longModifiedHash;
        ModifyHashToLong(hash, longModifiedHash);

        GetRandom(rnd);

        NTL::ZZ a(0);
        NTL::ZZ p(0);
        NTL::ZZ q(0);
        ConvertHexStringToLong("9E93A4096E5416CED0242228014B67B5", a);
        ConvertHexStringToLong("AF5228967057FE1CB84B92511BE89A47", p);
        ConvertHexStringToLong("57A9144B382BFF0E5C25C9288DF44D23", q);
        NTL::PowerMod(z,
            a, rnd, p
        );
        x = NTL::RandomBits_ZZ(127);
        y = NTL::PowerMod(a, x, p);
        k = ((rnd*longModifiedHash - x*z) * NTL::InvMod(longModifiedHash, q)) % q;
        g = (x*z * NTL::InvMod(longModifiedHash,q)) % q;
        NTL::PowerMod(signature,a, g, p);
        return true;
    }
}
