#include <NTL/ZZ.h>
#include "filereader.h"
#include "generalhelpers.h"
#include "hashing.h"


int main()
{
    std::vector<char> fileData;

    if(!CryptoHelpers::ReadFile("test.txt1", fileData))
    {
        return 1;
    }

    std::vector<NTL::ZZ> data;
    CryptoHelpers::ConvertByteToLong(fileData, data);

    NTL::ZZ hashValue;

    CryptoHelpers::Hash(data, hashValue);

    std::cout << hashValue << '\n';
    std::cin.get();
}
