//
//  Input.hpp
//  SaplingEngine, SFML Input Wrapper
//


#ifndef Input_hpp
#define Input_hpp

#include "SaplingEngine.hpp"
#include <string>
#include <map>


struct InputAxis {
    std::string name;
    int postiveKey;
    int negativeKey;
    
    InputAxis(std::string  name, int pKey, int nKey);
};

struct Key {
    bool justPressed = false;
    bool justReleased = false;
    bool pressed = false;
    
    Key() = default;
};

using ActionsMap = std::map<std::string, std::vector<int>>;
using AxisMap = std::map<std::string, std::shared_ptr<InputAxis>>;
using KeyMap = std::map<int, std::shared_ptr<Key>>;


/*
 TODO: support mouse inputs
 TODO: support controller inputs
 */
class Input {
    ActionsMap m_actionsMap;
    AxisMap m_axisMap;
    KeyMap m_keyMap;
    
    auto getKey(int key) -> bool;
    auto getKeyDown(int key) -> bool;
    auto getKeyUp(int key) -> bool;
    
public:
    void update(sf::RenderWindow& window);
    void makeAction(const std::string& name, const std::vector<int>& keycodes);
    auto isAction(const std::string& name) -> bool;
    auto isActionDown(const std::string& name) -> bool;
    auto isActionUp(const std::string& name) -> bool;

    void makeAxis(const std::string& name, int positiveKey, int negativeKey);
    auto getAxis(const std::string& name) -> int;

    auto getMouseDown(int button) -> bool;
    
    Input();
};



#endif /* Input_hpp */
