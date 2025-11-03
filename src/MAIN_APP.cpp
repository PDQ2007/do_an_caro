#include <SFML/Graphics.hpp>
#include "game_window.h"
#include "different_window.h"
#include "config.h"

int main(){
	sf::RenderWindow win(sf::VideoMode({globalConfig::win_width, globalConfig::win_height}), "SFML");
	//drawGameScreen(win);
	bool is_loop = true;
	while(is_loop){
		switch(globalConfig::current_win){
			case 0:
				is_loop = false;
				break;
			case 3:
				drawGameScreen(win);
				break;
			case 5:
				drawADifferentWindow(win);
		};
	};
};