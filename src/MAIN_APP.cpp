#include <SFML/Graphics.hpp>
#include <iostream>
#include "menu_screen.h"
#include "play_screen.h"
#include "config.h"
#include <fcntl.h>
#include <io.h>
#include <string>

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    sf::RenderWindow window(sf::VideoMode({globalConfig::win_width, globalConfig::win_height}), "Code structure demo");
    while(window.isOpen()){
        globalConfig::re_init = false;
        switch(globalConfig::current_win){
        case 0:
            window.close();
            break;
        case 1:
            drawMenuScreen(window);
            break;
        case 2:
            drawPlayScreen(window);
            break;
        };
    };
}