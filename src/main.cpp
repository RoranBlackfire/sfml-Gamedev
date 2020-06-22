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
    std::cout << file << '\n' << posx << '\n';
}