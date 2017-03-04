
#include "filereader.h"
#include <fstream>
#include "constants.h"


namespace CryptoHelpers
{
    bool ReadFile(const std::string& filename, std::vector<char>& data)
    {
        std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::ate);

        if (!file.is_open())
        {
            return false;
        }

        auto fileSize = file.tellg();

        data.resize(fileSize);

        file.seekg(0, std::ios::beg);
        file.read(static_cast<char*>(&data[0]), fileSize);
        file.close();

        return true;
    }

    void ConvertByteToLong(const std::vector<char>& source, std::vector<NTL::ZZ>& target)
    {
        target.resize(std::ceil(source.size() / LONG_BYTE_LENGTH));

        if(target.size()==0)
        {
            target.resize(1);//dirty hack
        }
        auto targetIndex = 0;
        auto it = target.begin();
        (*it) = 0;
        for (const auto& byte : source)
        {
            ++targetIndex;

            (*it) <<= 8;
            (*it) += byte;

            if (targetIndex > 7)
            {
                ++it;
                (*it) = 0;
            }
        }
    }
}
