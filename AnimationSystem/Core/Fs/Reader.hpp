#ifndef READER_HPP
#define READER_HPP

#include <filesystem>
#include <fstream>
#include <Core/Task/Task.hpp>

namespace AnimationSystem
{
    template <typename R>
    class Reader
    {

    public:
        Reader(std::filesystem::path filePath) : _filePath(filePath){};
        R read();

    private:
        std::filesystem::path _filePath;
    };

} // namespace AnimationSystem

#endif
