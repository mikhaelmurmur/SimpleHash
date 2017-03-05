
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

    
}
