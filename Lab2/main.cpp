#include <NTL/ZZ.h>
#include "filereader.h"
#include "generalhelpers.h"
#include "hashing.h"
#include "signing.h"


int main()
{
    std::vector<char> fileData;

    if(!CryptoHelpers::ReadFile("main.cpp", fileData))
    {
        return 1;
    }
    
    std::vector<unsigned long long> data; 
    CryptoHelpers::ConvertByteToLong(fileData, data);

    unsigned long long hashValue;

    CryptoHelpers::Hash(data, hashValue);

    std::cout <<std::hex<< hashValue << '\n';

    NTL::ZZ t(0);
    std::pair<NTL::ZZ, NTL::ZZ> p(t, t);
    CryptoHelpers::Sign(hashValue, p);
    std::cin.get();
}
