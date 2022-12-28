#include "Reader.hpp"
#include <type_traits>
#include <iostream>

namespace AnimationSystem
{
    namespace
    {
        std::string readFile(std::filesystem::path filePath)
        {
            std::string content;
            std::fstream fileStream(filePath, std::ios::in);
            if (!fileStream.is_open())
                return "";

            std::string line = "";
            while (!fileStream.eof())
            {
                std::getline(fileStream, line);
                content.append(line + "\n");
            }

            fileStream.close();
            return content;
        }
    }

    // /* Generic implementation */
    // TODO: read mesh objects -> mesh reader that reads from bytes in buffered stream?
    template <typename R>
    R Reader<R>::read()
    {
        std::cout << "not implemented \n";
        return R{};
    }

    // /* String specialized implementation */
    template <>
    std::string Reader<std::string>::read()
    {
        return readFile(_filePath);
    }
} // namespace AnimationSystem
