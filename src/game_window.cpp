#ifndef GAME_WINDOW_CPP
#define GAME_WINDOW_CPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "game_window.h"
#include "config.h"

namespace gameStat{
	std::vector<sf::Vector2i> moves;
	std::vector<std::vector<bool> > cells(16, std::vector<bool> (16, 0));
	int turn_of = 0, first_turn = 0; //0 = x, 1 = O
};


//static sf::RenderWindow game_win(sf::VideoMode({1200, 800}), "My Window Title");

static sf::View caroTableView(sf::FloatRect({0.f, 0.f}, {800.f, 800.f}));

namespace texture{
	static sf::Texture
		caro_table[2] = {
			sf::Texture("resources/game_screen/light/caro_table.png"),
			sf::Texture("resources/game_screen/dark/caro_table.png")
		},
		background[2] = {
			sf::Texture("resources/game_screen/light/background.png"),
			sf::Texture("resources/game_screen/dark/background.png")
		},
		x_character[2] = {
			sf::Texture("resources/game_screen/light/X.png"),
			sf::Texture("resources/game_screen/dark/X.png")
		},
		o_character[2] = {
			sf::Texture("resources/game_screen/light/O.png"),
			sf::Texture("resources/game_screen/dark/O.png")
		},
		normal_undo_button[2] = {
			sf::Texture("resources/game_screen/light/undo_button.png"),
			sf::Texture("resources/game_screen/dark/undo_button.png")
		},
		hovered_undo_button[2] = {
			sf::Texture("resources/game_screen/light/hovered_undo_button.png"),
			sf::Texture("resources/game_screen/dark/hovered_undo_button.png")
		},
		clicked_undo_button[2] = {
			sf::Texture("resources/game_screen/light/clicked_undo_button.png"),
			sf::Texture("resources/game_screen/dark/clicked_undo_button.png")
		},
		normal_switch_light_dark_button[2] = {
			sf::Texture("resources/game_screen/light/switch_to_dark.png"),
			sf::Texture("resources/game_screen/dark/switch_to_light.png")
		},
		hovered_switch_light_dark_button[2] = {
			sf::Texture("resources/game_screen/light/hovered_switch_to_dark.png"),
			sf::Texture("resources/game_screen/dark/hovered_switch_to_light.png")
		},
		clicked_switch_light_dark_button[2] = {
			sf::Texture("resources/game_screen/light/clicked_switch_to_dark.png"),
			sf::Texture("resources/game_screen/dark/clicked_switch_to_light.png")
		},
		normal_settings_button[2] = {
			sf::Texture("resources/game_screen/light/settings_button.png"),
			sf::Texture("resources/game_screen/dark/settings_button.png")
		},
		hovered_settings_button[2] = {
			sf::Texture("resources/game_screen/light/hovered_settings_button.png"),
			sf::Texture("resources/game_screen/dark/hovered_settings_button.png")
		},
		clicked_settings_button[2] = {
			sf::Texture("resources/game_screen/light/clicked_settings_button.png"),
			sf::Texture("resources/game_screen/dark/clicked_settings_button.png")
		};
};

sf::Sprite
	caro_table(texture::caro_table[globalConfig::dark_mode]),
	background(texture::background[globalConfig::dark_mode]),
	x_character(texture::x_character[globalConfig::dark_mode]),
	o_character(texture::o_character[globalConfig::dark_mode]),
	undo_button(texture::normal_undo_button[globalConfig::dark_mode]),
	switch_light_dark_button(texture::normal_switch_light_dark_button[globalConfig::dark_mode]),
	settings_button(texture::normal_settings_button[globalConfig::dark_mode]);


//-------------------CONTROL SECTION------------------------

namespace events{

	//tested by Quang
	void addAMove(sf::Vector2f pos){
		
		//if mouse is clicked...
		
		if(!sf::FloatRect({0.f, 0.f}, {800.f, 800.f}).contains(pos)) return;
		int _x = (int)pos.x / 50, _y = (int)pos.y / 50;
		if(_x > 15 || _x < 0 || _y > 15 || _y < 0) return;
		if(gameStat::cells[_x][_y]) return;
		gameStat::moves.push_back(sf::Vector2i(_x, _y));
		gameStat::cells[_x][_y] = true;
	};

	//tested by Quang
	void undoMove(sf::RenderWindow& win){
		
		//if mouse is clicked...

		if(!undo_button.getGlobalBounds().contains(win.mapPixelToCoords(sf::Mouse::getPosition(win))))
			return;
		if(!gameStat::moves.empty()){
			sf::Vector2i& cell = gameStat::moves[gameStat::moves.size()-1];
			gameStat::cells[cell.x][cell.y] = false;
			gameStat::moves.pop_back();
		};
	};

	//tested by Quang
	void switchLightDarkMode(sf::RenderWindow &win, bool& re_init){
		
		//if mouse is clicked...

		if(!switch_light_dark_button.getGlobalBounds().contains(win.mapPixelToCoords(sf::Mouse::getPosition(win))))
			return;

		globalConfig::dark_mode = (globalConfig::dark_mode + 1) % 2;

		re_init = true;

	};

	void openInGameSettings(sf::RenderWindow &win){

		//if mouse is clicked...

		if(!settings_button.getGlobalBounds().contains(win.mapPixelToCoords(sf::Mouse::getPosition(win))))
			return;

		std::cout << "clicked";

		globalConfig::current_win = 5;

	};

};


//-------------------DRAW SECTION------------------------

namespace draw{

	//tested by Quang
	void drawUndoButton(sf::RenderWindow& win){
		undo_button.setScale({90.f/globalConfig::win_width, 90.f/globalConfig::win_width});
		undo_button.setPosition(win.mapPixelToCoords({1070, 670}));
		if(undo_button.getGlobalBounds().contains(win.mapPixelToCoords(sf::Mouse::getPosition(win)))){
			if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
				undo_button.setTexture(texture::clicked_undo_button[globalConfig::dark_mode]);
			} else{
				undo_button.setTexture(texture::hovered_undo_button[globalConfig::dark_mode]);
			};
		} else{
			undo_button.setTexture(texture::normal_undo_button[globalConfig::dark_mode]);
		}
		win.draw(undo_button);
	};

	//tested by Quang
	void drawSwitchLightDarkModeButton(sf::RenderWindow& win){
		switch_light_dark_button.setScale({90.f/globalConfig::win_width, 90.f/globalConfig::win_width});
		switch_light_dark_button.setPosition(win.mapPixelToCoords({960, 670}));
		if(switch_light_dark_button.getGlobalBounds().contains(win.mapPixelToCoords(sf::Mouse::getPosition(win)))){
			if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
				switch_light_dark_button.setTexture(texture::clicked_switch_light_dark_button[globalConfig::dark_mode]);
			} else{
				switch_light_dark_button.setTexture(texture::hovered_switch_light_dark_button[globalConfig::dark_mode]);
			};
		} else{
			switch_light_dark_button.setTexture(texture::normal_switch_light_dark_button[globalConfig::dark_mode]);
		}
		win.draw(switch_light_dark_button);
	};

	//tested by Quang
	void drawSettingsButton(sf::RenderWindow& win){
		settings_button.setScale({90.f/globalConfig::win_width, 90.f/globalConfig::win_width});
		settings_button.setPosition(win.mapPixelToCoords({850, 670}));
		if(settings_button.getGlobalBounds().contains(win.mapPixelToCoords(sf::Mouse::getPosition(win)))){
			if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
				settings_button.setTexture(texture::clicked_settings_button[globalConfig::dark_mode]);
			} else{
				settings_button.setTexture(texture::hovered_settings_button[globalConfig::dark_mode]);
			};
		} else{
			settings_button.setTexture(texture::normal_settings_button[globalConfig::dark_mode]);
		}
		win.draw(settings_button);
	};

};



//tested by Quang
void initGameScreen(bool is_re_init = false){

	caroTableView.setViewport(sf::FloatRect(
		{40.f/globalConfig::win_width, 40.f/globalConfig::win_height}, 
		{720.f/globalConfig::win_width, 720.f/globalConfig::win_height})
	);

	if(!is_re_init) gameStat::moves.clear();

	caro_table.setTexture(texture::caro_table[globalConfig::dark_mode]);
	background.setTexture(texture::background[globalConfig::dark_mode]);

	x_character.setTexture(texture::x_character[globalConfig::dark_mode]);
	o_character.setTexture(texture::o_character[globalConfig::dark_mode]);

	x_character.setOrigin({0.5, 0.5});
	o_character.setOrigin({0.5, 0.5});

	x_character.setScale({
		50.f/texture::x_character[globalConfig::dark_mode].getSize().x,
		50.f/texture::x_character[globalConfig::dark_mode].getSize().y
	});

	o_character.setScale({
		50.f/texture::o_character[globalConfig::dark_mode].getSize().x,
		50.f/texture::o_character[globalConfig::dark_mode].getSize().y
	});

};

//tested by Quang
bool loopGameScreen(sf::RenderWindow& win){
	
	bool re_init = false;

	int delay = 0;

	while(win.isOpen()){

		if(globalConfig::current_win != 3){
			if(++delay == 100) return re_init;
		} else delay = 0;

		win.setSize({
			globalConfig::win_width, 
			globalConfig::win_height
		});

		//check for events
		while(std::optional event = win.pollEvent()){
			
			// close window
			if(event->is<sf::Event::Closed> ()){
				win.close();
				globalConfig::current_win = 0;
				re_init = false;
			};

			//draw a move
			if(event->is<sf::Event::MouseButtonPressed> () &&
				sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				events::addAMove(win.mapPixelToCoords(sf::Mouse::getPosition(win), caroTableView));
				events::undoMove(win);
				events::switchLightDarkMode(win, re_init);
				events::openInGameSettings(win);
			};

		};

		if(re_init){
			return re_init;
		};

		//draw
		
		win.clear();

		// - draw background
		win.draw(background);

		// - draw undo button
		draw::drawUndoButton(win);
		draw::drawSwitchLightDarkModeButton(win);
		draw::drawSettingsButton(win);

		// - draw caro table and moves
		win.setView(caroTableView);
		win.draw(caro_table);

		for(int i = 0; i < gameStat::moves.size(); ++i){
			sf::Vector2i& move = gameStat::moves[i];
			if(i % 2 == 0){
				x_character.setPosition({
					static_cast<float>(50 * move.x),
					static_cast<float>(50 * move.y)
				});
				win.draw(x_character);
			} else{
				o_character.setPosition({
					static_cast<float>(50 * move.x),
					static_cast<float>(50 * move.y)
					});
				win.draw(o_character);
			};
		};

		win.setView(win.getDefaultView());
		win.display();

	};

	return re_init;
};

//tested by Quang
void drawGameScreen(sf::RenderWindow& win){
	bool re_init = false;
	do{
		initGameScreen(re_init);
		re_init = loopGameScreen(win);
	} while(re_init);
};

#endif