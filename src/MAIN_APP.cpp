#include "config.h"

int main(){
	sf::RenderWindow win(sf::VideoMode({globalConfig::win_width, globalConfig::win_height}), "SFML");
	
	std::thread mouseTrack(trackMousePosition, std::ref(win));
	mouseTrack.detach();

	gameDataPackage package;
	
	//drawGameScreen(win);
	bool is_loop = true;
	int& previous_screen = globalConfig::previous_win;

	bool start = 1;
	bool is_new_game = 1;
	short is_multiplayer = -1;	// -1 = multiplayer mode
							// 0 = singleplayer as X
							// 1 = singleplayer as O
	bool first_turn = 0; // 0 for X, 1 for O
	std::string player1_name, player2_name;
	std::filesystem::path load_game_from("saves");
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
				package = drawPrepareScreen(win);
				previous_screen = 2;
				break;
			case 3:
				if(previous_screen == 4){
					drawGameScreen(
						win, false,
						package
					);
				} else{
					drawGameScreen(
						win, true,
						package
					);
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