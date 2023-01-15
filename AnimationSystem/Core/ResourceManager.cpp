//
//  ResourceManager.cpp
//  AnimationSystem
//
//  Created by Çağrı Yıldız on 15.01.23.
//

#include "ResourceManager.hpp"
namespace AnimationSystem
{
    namespace
    {
        std::string get(const std::unordered_map<std::string, std::string>& _map, const std::string &key)
        {
            auto it = _map.find(key);
            assert(it != _map.end());
            return it->second;;
        }
    }
    std::string ResourceManager::getShaderPath(const std::string& name) const noexcept
    {
        return get(_shaderPathMap, name);
    }

    std::string ResourceManager::getCharacterPath(const std::string& name) const noexcept
    {
        return get(_characterPathMap, name);
    }
}
