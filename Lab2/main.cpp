#include <NTL/ZZ.h>
#include "filereader.h"
#include "generalhelpers.h"
#include "hashing.h"
#include "signing.h"
#include <fstream>
#include <sstream>

int ProcessSinging(const CryptoHelpers::SCommandLineOptions& options);
int ProcessChecking(const CryptoHelpers::SCommandLineOptions& options);

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
    else
    {
        ProcessChecking(options);
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
    NTL::ZZ x(0);
    CryptoHelpers::Sign(hashValue,signature,y,k,g,u,z,x);
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

int ProcessChecking(const CryptoHelpers::SCommandLineOptions& options)
{
    std::vector<char> fileData;
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


    NTL::ZZ signature(0);
    NTL::ZZ y(0);
    NTL::ZZ k(0);
    NTL::ZZ g(0);
    NTL::ZZ z(0);
    NTL::ZZ u(0);
    NTL::ZZ x(0);
    CryptoHelpers::Sign(hashValue, signature, y, k, g, u, z, x);

    std::ifstream fileWithSignature(options.m_fileWithSignature);
    std::string _tmp;
    std::getline(fileWithSignature, _tmp);
    std::getline(fileWithSignature, _tmp);
    std::getline(fileWithSignature, _tmp);
    _tmp = std::move(_tmp.substr(4, _tmp.size() - 4));
    size_t fileHash = 0;
    std::stringstream sstream;
    sstream << std::hex << hashValue;
    std::string _hashStr = sstream.str();


    if (_tmp.compare(_hashStr)!=0)
    {
        std::cout << "Wrong hash value!\n";
        return 2;
    }


    std::getline(fileWithSignature, _tmp);
    _tmp = std::move(_tmp.substr(4, _tmp.size() - 4));
    CryptoHelpers::ConvertHexStringToLong(_tmp, y);

    std::getline(fileWithSignature, _tmp);
    _tmp = std::move(_tmp.substr(4, _tmp.size() - 4));
    CryptoHelpers::ConvertHexStringToLong(_tmp, k);

    std::getline(fileWithSignature, _tmp);
    _tmp = std::move(_tmp.substr(4, _tmp.size() - 4));
    CryptoHelpers::ConvertHexStringToLong(_tmp, signature);


    if(CryptoHelpers::CheckSignature(hashValue,signature,y,k))
    {
        std::cout << "Signature is correct!\n";
    }
    else
    {
        std::cout << "Signature is wrong!\n";
    }



    return 0;
}