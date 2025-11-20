#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

void drawGameScreen(	
	sf::RenderWindow& win,
	bool start,
	bool is_new_game,
	short is_multiplayer,	// -1 = multiplayer mode
							// 0 = singleplayer as X
							// 1 = singleplayer as O
	bool first_turn, // 0 for X, 1 for O
	std::string player1_name,
	std::string player2_name,
	std::filesystem::path& load_game_from
);

#endif
