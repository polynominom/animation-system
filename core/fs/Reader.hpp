#pragma once
#include "fs/FS.h"
#include "task/Task.hpp"

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
