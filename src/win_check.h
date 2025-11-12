#ifndef WIN_CHECK_H
#define WIN_CHECK_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <vector>

// 
std::vector<sf::Vector2i> checkForWin(
	sf::Vector2i& last_move, 
	std::vector<std::vector<char> >& cells,
	bool& status
);

#endif