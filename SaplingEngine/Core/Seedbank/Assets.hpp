//
//  Assets.hpp
//  ECS_Engine
//
//  Created by Tuukka Virtanen on 1/9/24.
//

#ifndef Assets_hpp
#define Assets_hpp

#include <string>
#include <map>
#include <unordered_map>

#include <SFML/Graphics.hpp>


class Assets {
    std::unordered_map<std::string, std::shared_ptr<sf::Texture>> m_textures;

public:
    Assets();

    void addTexture(const std::string& name, const std::string& path);
    auto getTexture(const std::string& name) -> std::shared_ptr<sf::Texture>;
};

#endif /* Assets_hpp */
