#ifndef WIN_CHECK_CPP
#define WIN_CHECK_CPP

#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

bool inside(short n){
	return (n >= 0 && n < 16);
};

std::vector<sf::Vector2i> checkForWin(
	sf::Vector2i& last_move, 
	std::vector<std::vector<char> >& cells,
	bool& status
){
	std::vector<sf::Vector2i> result;
	result.push_back(last_move);
	bool status2 = true;
	auto check = [&last_move, &cells, &status, &status2, &result](short _x, short _y){
		int count = 1, i = 0;
		bool end_loop = false;
		while(!end_loop){
			end_loop = true;
			++i;
			if(
				(inside(last_move.x + (i * _x))) && (inside(last_move.y + (i * _y))) &&
				(cells[last_move.x + (i * _x)][last_move.y + (i * _y)] == cells[last_move.x][last_move.y])
			){
				++count;
				result.push_back(sf::Vector2i(last_move.x + (i * _x), last_move.y + (i * _y)));
				end_loop = false;
			};
			if(
				(inside(last_move.x - (i * _x))) && (inside(last_move.y - (i * _y))) &&
				cells[last_move.x - (i * _x)][last_move.y - (i * _y)] == cells[last_move.x][last_move.y]
			){
				++count;
				result.push_back(sf::Vector2i(last_move.x - (i * _x), last_move.y - (i * _y)));
				end_loop = false;
			};
			if(count >= 5) status2 *= false;
			else status2 *= true;
		};
	};
	
	check(1, 0);
	check(0, 1);
	check(1, 1);
	check(-1, 1);

	status = !status2;

	return result;
};

#endif