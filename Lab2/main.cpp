#include <NTL/ZZ.h>
#include "filereader.h"
#include "encryption.h"
#include "generalhelpers.h"

int main()
{
    std::vector<char> fileData;

    if(!CryptoHelpers::ReadFile("main.cpp", fileData))
    {
        return 1;
    }

    std::vector<NTL::ZZ> data;
    CryptoHelpers::ConvertByteToLong(fileData, data);
    NTL::ZZ encryptedData(1);
    for(auto& _data: data)
    {
        CryptoHelpers::Encrypt(_data, encryptedData);

        std::cout << encryptedData << '\n';
    }
    std::cin.get();
}
