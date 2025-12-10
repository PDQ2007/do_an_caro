#ifndef PROCESS_H
#define PROCESS_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

void loadGame(
	gameDataPackage& package,
	unsigned int& x_score,
	unsigned int& y_score,
	std::vector<sf::Vector2i>& moves,
	std::vector<std::vector<short> >& cells,
	std::wstring& return_message // if successful, return "success", else return the error's content
);

// 
std::vector<sf::Vector2i> checkForWin(
	sf::Vector2i& last_move, 
	std::vector<std::vector<short> >& cells,
	bool& status
);

void saveGame(
	gameDataPackage& saveInfo,
	unsigned int x_score,
	unsigned int o_score,
	std::vector<sf::Vector2i>& moves
);

bool checkForDraw(std::vector<sf::Vector2i>& moves);

#endif