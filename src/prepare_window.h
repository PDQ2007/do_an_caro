#ifndef PREPARE_WINDOW_H
#define PREPARE_WINDOW_H

#include "config.h"

void drawPrepareScreen(
	sf::RenderWindow& win, 
	std::filesystem::path& load_game_from, 
	bool& is_new_game
);

#endif