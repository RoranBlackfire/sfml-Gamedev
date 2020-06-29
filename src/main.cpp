#include <iostream>
#include <SFML/Main.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <lua.hpp>
#include "LuaScript.h"

//note: run test.cpp to see a good example

int main(){
    std::cout << "hello SFML and ImGui\n";
    sf::RenderWindow window(sf::VideoMode(640, 480), "Main Window");
    window.setFramerateLimit(120);
    sf::Clock clock;

    //renders a blank window
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
        }
    }
}