#include "config.h"

int main(){
	sf::RenderWindow win(sf::VideoMode({globalConfig::win_width, globalConfig::win_height}), "SFML");
	
	std::thread mouseTrack(trackMousePosition, std::ref(win));
	mouseTrack.detach();
	
	//drawGameScreen(win);
	bool is_loop = true;
	int& previous_screen = globalConfig::previous_win;
	while(is_loop){
		switch(globalConfig::current_win){
			case 0:
				is_loop = false;
				break;
			case 1:
				drawMenuWindow(win);
				previous_screen = 1;
				break;
			case 2:
				drawPrepareScreen(win);
				previous_screen = 2;
				break;
			case 3:
				if(previous_screen == 5){
					drawGameScreen(win, false);
				} else{
					drawGameScreen(win, true);
				};
				previous_screen = 3;
				break;
			case 4:
				drawSettingsScreen(win);
				previous_screen = 4;
				break;
			case 5:
				drawADifferentWindow(win);
				previous_screen = 5;
				break;
		};
	};
};