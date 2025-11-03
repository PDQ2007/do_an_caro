#ifndef PLAY_SCREEN_CPP
#define PLAY_SCREEN_CPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "config.h"

namespace { //make everything defined here local

	namespace strings{
		std::wstring
			title[2] = {
			L"This is a Play screen",
			L"Đây là Màn hình chuẩn bị chơi"
		},
			open_win1[2] = {
			L"Click here to open Menu screen",
			L"Bấm vào đây để mở Màn hình menu"
		},
			open_win3[2] = {
			L"Click here to open Game screen",
			L"Bấm vào đây để mở Màn hình game"
		},
			open_win4[2] = {
			L"Click here to open Settings screen",
			L"Bấm vào đây để mở Màn hình cài đặt"
		},
			open_win5[2] = {
			L"Click here to open In-game settings screen",
			L"Bấm vào đây để mở Màn hình cài đặt trong khi chơi"
		},
			change_lang[2] = {
			L"Click here to switch to Vietnamese",
			L"Bấm vào đây để đổi ngôn ngữ sang tiếng Anh"
		};
	};

	sf::Font myFont("resources/fonts/cour.ttf");

	sf::Text
		title(myFont, "", 50),
		win1(myFont, "", 30),
		win3(myFont, "", 30),
		win4(myFont, "", 30),
		win5(myFont, "", 30),
		change_lang(myFont, "", 30);

	namespace events{
		void Win1(sf::RenderWindow& win){
			if(win1.getGlobalBounds().contains(win.mapPixelToCoords(sf::Mouse::getPosition(win)))){
				globalConfig::current_win = 1;
			};
		};
		void Win3(sf::RenderWindow& win){
			if(win3.getGlobalBounds().contains(win.mapPixelToCoords(sf::Mouse::getPosition(win)))){
				print_sth(L"Game screen");
			};
		};
		void Win4(sf::RenderWindow& win){
			if(win4.getGlobalBounds().contains(win.mapPixelToCoords(sf::Mouse::getPosition(win)))){
				print_sth(L"Settings screen");
			};
		};
		void Win5(sf::RenderWindow& win){
			if(win5.getGlobalBounds().contains(win.mapPixelToCoords(sf::Mouse::getPosition(win)))){
				print_sth(L"In-game settings screen");
			};
		};
		void Change_lang(sf::RenderWindow& win){
			if(change_lang.getGlobalBounds().contains(win.mapPixelToCoords(sf::Mouse::getPosition(win)))){
				//std::cout << "change-lang ";
				globalConfig::language = (globalConfig::language + 1) % 2;
				globalConfig::re_init = true;
			};
		};
	};

	void initPlayScreen(sf::RenderWindow& win){
		title.setString(strings::title[globalConfig::language]);
		win1.setString(strings::open_win1[globalConfig::language]);
		win3.setString(strings::open_win3[globalConfig::language]);
		win4.setString(strings::open_win4[globalConfig::language]);
		win5.setString(strings::open_win5[globalConfig::language]);
		change_lang.setString(strings::change_lang[globalConfig::language]);

		title.setPosition({10, 10});
		win1.setPosition({30, 70});
		win3.setPosition({30, 110});
		win4.setPosition({30, 150});
		win5.setPosition({30, 190});
		change_lang.setPosition({30, 230});

	};

	void loopPlayScreen(sf::RenderWindow& win){

		bool is_loop = true;

		while(is_loop){

			if(globalConfig::current_win != 2 || globalConfig::re_init) return;

			win.setSize({globalConfig::win_width, globalConfig::win_height});

			// check for events
			while(std::optional event = win.pollEvent()){
				if(event->is<sf::Event::Closed> ()){
					globalConfig::current_win = 0;
					is_loop = false;
				} else if(event->is<sf::Event::MouseButtonPressed> ()){
					events::Win1(win);
					events::Win3(win);
					events::Win4(win);
					events::Win5(win);
					events::Change_lang(win);
				};
			};

			win.clear();

			win.draw(title);
			win.draw(win1);
			win.draw(win3);
			win.draw(win4);
			win.draw(win5);
			win.draw(change_lang);

			win.display();
		};
	};

};

void drawPlayScreen(sf::RenderWindow& win){
	initPlayScreen(win);
	//std::cout << "check";
	loopPlayScreen(win);
};

#endif