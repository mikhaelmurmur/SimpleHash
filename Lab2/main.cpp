#include <NTL/ZZ.h>
#include "filereader.h"
#include "generalhelpers.h"
#include "hashing.h"
#include "signing.h"
#include <fstream>

int ProcessSinging(const CryptoHelpers::SCommandLineOptions& options);

int main(int argc, char *argv[])
{
    CryptoHelpers::SCommandLineOptions options;

    options.m_filename = argv[2];
    if(strcmp(argv[1], "-sign")==0)
    {
        options.m_mode = CryptoHelpers::Signing;
    } else if(strcmp(argv[1], "-check") == 0)
    {
        if(argc<3)
        {
            return 0;
        }
        options.m_mode = CryptoHelpers::Checking;
        options.m_fileWithSignature = argv[3];
    }

    if(options.m_mode==CryptoHelpers::Signing)
    {
        ProcessSinging(options);
    }
}


int ProcessSinging(const CryptoHelpers::SCommandLineOptions& options)
{
    std::vector<char> fileData;
    std::ofstream outputFile(options.m_filename + ".sig");
    outputFile << "------------------------------\n";
    outputFile << options.m_filename << std::endl;
    if (!CryptoHelpers::ReadFile(options.m_filename, fileData))
    {
        return 1;
    }

    unsigned long long hashValue = 0;

    if (fileData.size() != 0)
    {
        std::vector<unsigned long long> data;
        CryptoHelpers::ConvertByteToLong(fileData, data);
        CryptoHelpers::Hash(data, hashValue);
    }
    outputFile << "H = " << std::hex << hashValue << '\n';


    NTL::ZZ signature(0);
    NTL::ZZ y(0);
    NTL::ZZ k(0);
    NTL::ZZ g(0);
    NTL::ZZ z(0);
    NTL::ZZ u(0);
    CryptoHelpers::Sign(hashValue,signature,y,k,g,u,z);
    std::string tmp;
    CryptoHelpers::ConvertLongToString(y, tmp);
    outputFile << "Y = " << tmp << std::endl; 
    CryptoHelpers::ConvertLongToString(k, tmp);
    outputFile << "K = " << tmp << std::endl; 
    CryptoHelpers::ConvertLongToString(signature, tmp);
    outputFile << "S = " << tmp << std::endl;
    outputFile << "------------------------------\n";


    std::ofstream outputadd(options.m_filename + ".sig.add");
    outputadd << "------------------------------\n";
    outputadd << options.m_filename << std::endl;
    CryptoHelpers::ConvertLongToString(u, tmp);
    outputadd << "U = " << tmp << std::endl;
    CryptoHelpers::ConvertLongToString(z, tmp);
    outputadd << "Z = " << tmp << std::endl;
    CryptoHelpers::ConvertLongToString(g, tmp);
    outputadd << "G = " << tmp << std::endl;
    outputadd << "------------------------------\n";

    return 0;
}