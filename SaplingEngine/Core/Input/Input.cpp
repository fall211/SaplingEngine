//
//  Input.cpp
//  ECS_Engine
//
//  Created by Tuukka Virtanen on 1/16/24.
//

#include "Input.hpp"

InputAxis::InputAxis(std::string  name, const int pKey, const int nKey) : name(std::move(name)), postiveKey(pKey), negativeKey(nKey){}



Input::Input(){
    m_actionsMap = std::map<std::string, std::vector<int>>();
    m_axisMap = std::map<std::string, std::shared_ptr<InputAxis>>();
    m_keyMap = std::map<int, std::shared_ptr<Key>>();
}

void Input::update(sf::RenderWindow& window){
    
    for (const auto& pair : m_keyMap){
        const std::shared_ptr<Key> key = pair.second;
        key->justPressed = false;
        key->justReleased = false;
    }
    
    sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
            
            else if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased) {

                // check if there is a key for this code in m_keyMap
                if (m_keyMap.count(event.key.code) == 0) continue;

                const std::shared_ptr<Key> key = m_keyMap[event.key.code];
                
                if (event.type == sf::Event::KeyPressed){
                    key->justPressed = true;
                    key->pressed = true;
                } else {
                    key->justReleased = true;
                    key->pressed = false;
                }

            }
            else if (event.type == sf::Event::MouseButtonPressed) {

            }
            else if (event.type == sf::Event::MouseButtonReleased) {

            }
        }
}

auto Input::getKey(const int key) -> bool{
    return m_keyMap[key]->pressed;
}
auto Input::getKeyDown(const int key) -> bool{
    return m_keyMap[key]->justPressed;
}
auto Input::getKeyUp(const int key) -> bool{
    return m_keyMap[key]->justReleased;
}

void Input::makeAction(const std::string& name, const std::vector<int>& keycodes){
    /*
     TODO: return if an action with the given name already exists
     */
    
    m_actionsMap.insert({name, keycodes});
    for (const auto& k : keycodes){
        if (m_keyMap.count(k) == 0){
            auto key = std::make_shared<Key>();
            m_keyMap.insert({k, key});
        }
    }
}

auto Input::isAction(const std::string& name) -> bool{
    for (const auto& key : m_actionsMap[name]){
        if (getKey(key)) return true;
    }
    return false;
}


auto Input::isActionDown(const std::string& name) -> bool{
    for (const auto& key : m_actionsMap[name]){
        if (getKeyDown(key)) return true;
    }
    return false;
}

auto Input::isActionUp(const std::string& name) -> bool{
    for (const auto& key : m_actionsMap[name]){
        if (getKeyUp(key)) return true;
    }
    return false;
}

void Input::makeAxis(const std::string& name, const int positiveKey, const int negativeKey){
    /*
     TODO: allow multiple keys to provide positive/negative key input
     TODO: return if an axis with the given name already exists
     */
    
    m_axisMap.insert({name, std::make_shared<InputAxis>(name, positiveKey, negativeKey)});
    
    // check if the keys are registered already, if not, add them to m_keyMap
    if (m_keyMap.count(positiveKey) == 0){
        auto key = std::make_shared<Key>();
        m_keyMap.insert({positiveKey, key});
    }
    if (m_keyMap.count(negativeKey) == 0){
        auto key = std::make_shared<Key>();
        m_keyMap.insert({negativeKey, key});
    }

}

auto Input::getAxis(const std::string& name) -> int{
    const std::shared_ptr<InputAxis> axis = m_axisMap[name];
    const int pos = getKey(axis->postiveKey) ? 1 : 0;
    const int neg = getKey(axis->negativeKey) ? 1 : 0;
    return pos - neg;
}

auto Input::getMouseDown(int button) -> bool {
    return false;
}
