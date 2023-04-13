//
//  BlendTreeParser.hpp
//  AnimationSystem
//
//  Created by Çağrı Yıldız on 02.04.23.
//

#ifndef BlendTreeParser_h
#define BlendTreeParser_h

#include <BlendTree.hpp>

namespace AnimationSystem {

    template<typename readerAPI>
    class BlendTreeParser
    {
        static BlendTree parse(const std::string &filename);
    };
}
#endif /* BlendTreeParser_h */
