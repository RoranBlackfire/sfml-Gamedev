#include <iostream>
#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <lua.hpp>
#include "luascript.h"


int main(){
    std::cout << "Hello World\n";
    LuaScript script(static_cast<std::string>("res/scripts/bello.lua"));
    //std::cout << std::string(script.get("player.filename")) << '\n' << (int)script.get("player.pos.x");
    std::string file = script.get<std::string>("player.filename");
    int posx = script.get<int>("player.pos.x");
    std::cout << file << '\n' << posx << '\n'; //check
    std::vector<int> v = script.getVector<int>("array");
    for (auto it = v.begin(); it != v.end(); ++it){
        std::cout << *it << '\n';
    }
    //check

    //this is to get all the keys of a table
    std::vector<std::string> player_keys = script.getTableKeys("player");

    for (auto it = player_keys.begin(); it != player_keys.end(); ++it){
        std::cout << *it << '/n';
    }

    std::vector<std::string> v2 = script.getVector<std::string>("barry");
    for (auto it = v2.begin(); it != v2.end(); ++it){
        std::cout << *it << '\n';
    }
    //check
}