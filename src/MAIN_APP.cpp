#include <SFML/Graphics.hpp>
#include "game_window.h"

int main(){
	sf::RenderWindow win(sf::VideoMode({1200, 800}), "SFML");
	draw_game_screen(win);
};