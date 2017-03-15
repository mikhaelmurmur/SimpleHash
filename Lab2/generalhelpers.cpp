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
        
        if(targetIndex!=0)
        {
            *(it) <<= 8-targetIndex;
        }
    }

    void ConvertByteToLong(const std::vector<char>& source, std::vector<unsigned long long>& target)
    {
        target.resize(source.size() % LONG_BYTE_LENGTH == 0 ? source.size() / LONG_BYTE_LENGTH : source.size() / LONG_BYTE_LENGTH + 1);

        auto targetIndex = 0;
        auto it = target.begin();
        (*it) = 0;
        for (auto byteIndex = 0; byteIndex<source.size(); ++byteIndex)
        {
            if (byteIndex && byteIndex % 8 == 0)
            {
                ++it;
            }
            *it <<= 8;
            *it += source[byteIndex];
        }

        if(source.size()%LONG_BYTE_LENGTH!=0)//padding
        {
            auto diff = source.size() % LONG_BYTE_LENGTH;
            target[target.size() - 1] <<= 1;
            target[target.size() - 1] |= 1;
            target[target.size() - 1] <<= (8*(8-diff)-1);
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

    bool ConvertHexStringToLong(const std::string& source, NTL::ZZ& target)
    {
        target = 0;
        NTL::ZZ power(1);
        for(auto it = source.rbegin();it!=source.rend();++it)
        {
            auto digit = *it;
            if(('0'<=digit)&&(digit<='9'))
            {
                target += power*static_cast<int>(digit - '0');
            } else
                if(('a' <= tolower(digit)) && (tolower(digit) <= 'f'))
                {
                    target += power*(static_cast<int>(tolower(digit) - 'a')+10);
                }
                else
                    return false;
            power *= 16;
        }
        return true;
    }

    bool GetRandom(NTL::ZZ& result)
    {
        result = std::move(NTL::RandomBits_ZZ(128));
        return true;
    }

    void ConvertLongToString(const NTL::ZZ& _source, std::string& target)
    {
        target = "";
        auto source = _source;
        while(source>0)
        {
            unsigned char ch;
            NTL::BytesFromZZ(&ch, source & 0xf, 1);
            if(ch<10)
            {
                target += std::to_string(ch);
            }
            else
            {
                switch (ch)
                {
                case 10:
                    target += 'a';
                    break;
                case 11:
                    target += 'b';
                    break;
                case 12:
                    target += 'c';
                    break;
                case 13:
                    target += 'd';
                    break;
                case 14:
                    target += 'e';
                    break;
                case 15:
                    target += 'f';
                    break;
                }
            }
            source >>= 4;
        }
        std::reverse(target.begin(), target.end());
    }

    void ReverseBytes(int bytesNumber, unsigned long long& value)
    {
        auto newValue = 0ull;
        for (int i = 0; i<bytesNumber; ++i)
        {
            newValue <<= 8;
            newValue |= value & 0xFF;
            value >>= 8;
        }
        value = newValue;
    }

    void ReverseBytes(int bytesNumber, long long& value)
    {
        auto newValue = 0ull;
        for (int i = 0; i<bytesNumber; ++i)
        {
            newValue <<= 8;
            newValue |= value & 0xFF;
            value >>= 8;
        }
        value = newValue;
    }

}
