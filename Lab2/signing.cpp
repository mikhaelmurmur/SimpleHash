#include "signing.h"
#include <NTL/ZZ.h>
#include "generalhelpers.h"

namespace CryptoHelpers
{

    bool Sign(const long long& hash, std::pair<NTL::ZZ, NTL::ZZ>& target)
    {
        NTL::ZZ longModifiedHash(0);
        auto rhash = hash;
        ReverseBytes(8,rhash);
        ConvertHexStringToLong("00FFFFFFFFFFFF00", longModifiedHash);
        std::cout << rhash << std::endl;
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
        std::cout << longModifiedHash << std::endl;
        NTL::ZZ rnd(0);
        GetRandom(rnd);

        NTL::ZZ a(0);
        NTL::ZZ p(0);
        NTL::ZZ q(0);
        ConvertHexStringToLong("9E93A4096E5416CED0242228014B67B5", a);
        ConvertHexStringToLong("AF5228967057FE1CB84B92511BE89A47", p);
        ConvertHexStringToLong("57A9144B382BFF0E5C25C9288DF44D23", q);
        //ConvertHexStringToLong("1E6E5C1C66987ADC518A4578727AF081", rnd);
        NTL::ZZ z(0);
        NTL::PowerMod(z,
            a, rnd, p
        );
        NTL::ZZ x(0);
        //ConvertHexStringToLong("64B4AAD84A063A7F85F4923F0B4A3E62", x);
        x = NTL::RandomBits_ZZ(127);
        std::cout << "x = " << x << '\n';
        NTL::ZZ y(0);
        y = NTL::PowerMod(a, x, p);
        std::cout << "y = " << y << '\n';


        NTL::ZZ k(0);

        k = ((rnd*longModifiedHash - x*z) * NTL::InvMod(longModifiedHash, q)) % q;
        
        NTL::ZZ g(0);
        g = (x*z * NTL::InvMod(longModifiedHash,q)) % q;
        NTL::ZZ signature(0);
        NTL::PowerMod(signature,a, g, p);
        std::cout << signature << std::endl;

        NTL::ZZ tmp1(0);
        NTL::PowerMod(tmp1,signature, longModifiedHash, p);
        std::cout << tmp1 << std::endl;
        NTL::ZZ tmp2(0);
        NTL::ZZ tmp3(0);
        NTL::ZZ tmp4(0);
        NTL::PowerMod(tmp3,a, k, p);
        NTL::MulMod(tmp4, signature, tmp3, p);
        NTL::PowerMod(tmp2,y, (tmp4), p);
        std::cout << tmp2 << std::endl;

        if (tmp2 == tmp1)
        {
            std::cout << "Success!!!!\n";
        }
        return true;
    }
}
