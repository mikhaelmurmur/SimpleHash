#include "generalhelpers.h"
#include "constants.h"


namespace CryptoHelpers
{
    void ConvertByteToLong(const std::vector<char>& source, std::vector<NTL::ZZ>& target)
    {
        target.resize(source.size() % LONG_BYTE_LENGTH == 0 ? source.size()/LONG_BYTE_LENGTH : source.size()/LONG_BYTE_LENGTH +1);

        auto targetIndex = 0;
        auto it = target.begin();
        (*it) = 0;
        for(auto byteIndex = 0;byteIndex<source.size();++byteIndex)
        {
            if (byteIndex && byteIndex % 8 == 0)
            {
                ++it;
            }
            *it <<= 8;
            *it += source[byteIndex];
        }
        /*for (const auto& byte : source)
        {
            ++targetIndex;

            (*it) <<= 8;
            (*it) += byte;

            if (targetIndex > 7)
            {
                targetIndex = 0;
                if ((it + 1) != target.end())
                {
                    ++it;
                    (*it) = 0;
                }
            }
        }*/
        if(targetIndex!=0)
        {
            *(it) <<= 8-targetIndex;
        }
    }

    NTL::ZZ And32Bits(const NTL::ZZ& target)
    {
        NTL::ZZ result((target >> 4) & 0xFFFFFFF);
        result <<= 4;
        result |= target & 0xF;
        return result;
    }

    NTL::ZZ Xor32Bits(const NTL::ZZ& target)
    {
        NTL::ZZ result((target >> 4) ^ 0xFFFFFFF);
        result <<= 4;
        result |= (target & 0xF) ^ 0xF;
        return result;
    }
}
