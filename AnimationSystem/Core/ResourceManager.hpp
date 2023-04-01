//
//  ResourceManager.hpp
//  AnimationSystem
//
//  Created by Çağrı Yıldız on 15.01.23.
//

#ifndef ResourceManager_hpp
#define ResourceManager_hpp

#include <string>
#include <unordered_map>

namespace AnimationSystem
{
    class ResourceManager
    {
    public:
        void registerShaderName(const std::string &name, const std::string &path) { _shaderPathMap[name] = path; }
        std::string getShaderPath(const std::string &name) const noexcept;
        
        void registerCharacterName(const std::string &name, const std::string &path) { _characterPathMap[name] = path; }
        std::string getCharacterPath(const std::string &name) const noexcept;
        
        void registerBlendSpecificationName(const std::string &name, const std::string &path) { _blendSpecificationMap[name] = path; }
        std::string getBlendSpecificationPath(const std::string &name) const noexcept;

    private:
        std::unordered_map<std::string,std::string> _shaderPathMap;
        std::unordered_map<std::string,std::string> _characterPathMap;
        std::unordered_map<std::string, std::string> _blendSpecificationMap;
    };
} // namespace AnimationSytem

#endif /* ResourceManager_hpp */
