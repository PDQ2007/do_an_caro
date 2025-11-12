#include <SFML/Graphics.hpp>
#include "game_window.h"
#include "different_window.h"
#include "config.h"

int main(){
	sf::RenderWindow win(sf::VideoMode({globalConfig::win_width, globalConfig::win_height}), "SFML");
	//drawGameScreen(win);
	bool is_loop = true;
	int previous_screen = 0;
	while(is_loop){
		switch(globalConfig::current_win){
			case 0:
				is_loop = false;
				break;
			case 3:
				if(previous_screen == 5){
					drawGameScreen(win, false);
				} else{
					drawGameScreen(win, true);
				};
				previous_screen = 3;
				break;
			case 5:
				drawADifferentWindow(win);
				previous_screen = 5;
				break;
		};
	};
};