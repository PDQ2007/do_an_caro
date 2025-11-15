#ifndef DIFFERENT_WINDOW_CPP
#define DIFFERENT_WINDOW_CPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include "config.h"

namespace {

namespace fonts{
	sf::Font minecraft("resources/fonts/minecraft-f2d-v1-42.ttf");
};

namespace texts{

	std::string text_settings = "Settings";

	sf::Text settings(fonts::minecraft, text_settings, 30);

};

void initWindowScreen(sf::RenderWindow& win){
	
};

void loopWindowScreen(sf::RenderWindow& win){
	while(win.isOpen()){

		if(globalConfig::current_win != 5 && globalConfig::current_win != 1) break;

		win.setSize({globalConfig::win_width, globalConfig::win_height});

		while(std::optional event = win.pollEvent()){
			if(event->is<sf::Event::Closed> ()){
				win.close();
				globalConfig::current_win = 0;
			};
			if(event->is<sf::Event::MouseButtonPressed> ()){
				if(texts::settings.getGlobalBounds().contains(
					win.mapPixelToCoords(sf::Mouse::getPosition(win))
					)
				){
					globalConfig::current_win = 3;
				};
			};
		};

		win.clear();
		win.draw(texts::settings);
		win.display();
	};
};

};

void drawADifferentWindow(sf::RenderWindow& win){
	initWindowScreen(win);
	loopWindowScreen(win);
};

#endif