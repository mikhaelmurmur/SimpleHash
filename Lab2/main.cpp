#include <NTL/ZZ.h>
#include "filereader.h"
#include "encryption.h"

int main()
{
    std::vector<char> fileData;

    if(!CryptoHelpers::ReadFile("test.txt1", fileData))
    {
        return 1;
    }

    std::vector<NTL::ZZ> data;
    CryptoHelpers::ConvertByteToLong(fileData, data);
    NTL::ZZ encryptedData(1);
    CryptoHelpers::Encrypt(data[0], encryptedData);

    std::cout << encryptedData << '\n';
    std::cin.get();
}