#ifndef GAME_WINDOW_CPP
#define GAME_WINDOW_CPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "game_window.h"
#include "config.h"
#include "win_check.h"

std::string intToString(int n){
	std::string st = "";
	do{
		st = (char)((n % 10) + (int)'0') + st;
		n /= 10;
	} while(n > 0);
	return st;
};

namespace gameStat{

	bool is_win = false;
	std::vector<sf::Vector2i> moves;
	std::vector<std::vector<char> > cells(16, std::vector<char> (16, 0));
	// 0 = empty cells, 1 = X's cell, 2 = O's cell

	int first_turn = 1; //0 = X, 1 = O

	int x_score = 0, y_score = 0;
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
		},
		most_recent_move[2] = {
			sf::Texture("resources/game_screen/light/the_most_recent_move.png"),
			sf::Texture("resources/game_screen/dark/the_most_recent_move.png")
		};
};

sf::RectangleShape 
	rect_shape({50.f, 50.f}),
	scoreboard({1150.f - 945.f, 630.f - 500.f}),
	popup_box({380, 170});

sf::Sprite
	caro_table(texture::caro_table[globalConfig::dark_mode]),
	background(texture::background[globalConfig::dark_mode]),
	x_character(texture::x_character[globalConfig::dark_mode]),
	o_character(texture::o_character[globalConfig::dark_mode]),
	undo_button(texture::normal_undo_button[globalConfig::dark_mode]),
	switch_light_dark_button(texture::normal_switch_light_dark_button[globalConfig::dark_mode]),
	settings_button(texture::normal_settings_button[globalConfig::dark_mode]),
	most_recent_move(texture::most_recent_move[globalConfig::dark_mode]);

sf::Font
	minecraft_font("resources/fonts/minecraft-f2d-v1-42.ttf");

sf::Text
	XO_turn_to_play[2] = {
		sf::Text(minecraft_font, L"Turn of"),
		sf::Text(minecraft_font, L"Lượt chơi")
	},
	scoreboard_title[2] = {
		sf::Text(minecraft_font, L"Scoreboard"),
		sf::Text(minecraft_font, L"Bảng điểm")
	},
	XO_text[2] = {
		sf::Text(minecraft_font, L"X"),
		sf::Text(minecraft_font, L"O")
	},
	XO_score[2] = {
		sf::Text(minecraft_font),
		sf::Text(minecraft_font)
	},
	win_prompt[2] = {
		sf::Text(minecraft_font, L" has won."),
		sf::Text(minecraft_font, L" đã thắng."),
	};


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
		gameStat::cells[_x][_y] = ((gameStat::moves.size() + gameStat::first_turn) % 2) + 1;
	};

	//tested by Quang
	void undoMove(sf::RenderWindow& win){
		
		//if mouse is clicked...

		if(!undo_button.getGlobalBounds().contains(win.mapPixelToCoords(sf::Mouse::getPosition(win))))
			return;
		if(!gameStat::moves.empty()){
			sf::Vector2i& cell = gameStat::moves[gameStat::moves.size()-1];
			gameStat::cells[cell.x][cell.y] = 0;
			gameStat::moves.pop_back();
		};
	};

	//tested by Quang
	bool switchLightDarkMode(sf::RenderWindow &win){
		
		//if mouse is clicked...

		if(!switch_light_dark_button.getGlobalBounds().contains(win.mapPixelToCoords(sf::Mouse::getPosition(win))))
			return false;

		globalConfig::dark_mode = (globalConfig::dark_mode + 1) % 2;

		return true;

	};

	void openInGameSettings(sf::RenderWindow &win){

		//if mouse is clicked...

		if(!settings_button.getGlobalBounds().contains(win.mapPixelToCoords(sf::Mouse::getPosition(win))))
			return;

		globalConfig::current_win = 5;

	};

};


//-------------------DRAW SECTION------------------------

namespace draw{

	void drawAllMoves(sf::RenderWindow& win){
		for(int i = 0; i < gameStat::moves.size(); ++i){
			sf::Vector2i& move = gameStat::moves[i];
			if((i + gameStat::first_turn) % 2 == 0){
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
	};

	void drawLastMoveMarker(sf::RenderWindow& win){
		if(gameStat::moves.size() > 0){
			sf::Vector2i& last = gameStat::moves[gameStat::moves.size()-1];
			most_recent_move.setPosition({
				static_cast<float>(50 * last.x - 2),
				static_cast<float>(50 * last.y - 2)
			});
			win.draw(most_recent_move);
		};
	};

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

	void drawTurnOfXOText(sf::RenderWindow& win){

		win.draw(XO_turn_to_play[globalConfig::language]);

		float Size = 110.f;
		sf::Vector2i pos = {800, 530};

		if((gameStat::moves.size() + gameStat::first_turn) % 2 == 0){
			x_character.setPosition(win.mapPixelToCoords(pos));
			x_character.setScale({
				Size/texture::x_character[globalConfig::dark_mode].getSize().x,
				Size/texture::x_character[globalConfig::dark_mode].getSize().y
			});
			win.draw(x_character);

			//reset x, o scale for later draw
			x_character.setScale({
				50.f/texture::x_character[globalConfig::dark_mode].getSize().x,
				50.f/texture::x_character[globalConfig::dark_mode].getSize().y
			});

		} else{
			o_character.setPosition(win.mapPixelToCoords(pos));
			o_character.setScale({
				Size/texture::o_character[globalConfig::dark_mode].getSize().x,
				Size/texture::o_character[globalConfig::dark_mode].getSize().y
			});
			win.draw(o_character);

			//reset x, o scale for later draw
			o_character.setScale({
				50.f/texture::o_character[globalConfig::dark_mode].getSize().x,
				50.f/texture::o_character[globalConfig::dark_mode].getSize().y
			});
		};

	};

	void drawMouseHoverIntoCells(sf::RenderWindow& win){
		sf::RectangleShape square({40.f, 40.f});
		if(globalConfig::dark_mode){
			square.setFillColor(sf::Color(0, 0, 0, 150));
		} else{
			square.setFillColor(sf::Color(255, 255, 255, 150));
		};
		sf::Vector2f t = win.mapPixelToCoords(sf::Mouse::getPosition(win), caroTableView);
		if(!sf::FloatRect({0.f,0.f}, {800.f, 800.f}).contains(t)){
			//std::cout << "Return ";
			return;
		};
		short
			_x = static_cast<int>(t.x / 50),
			_y = static_cast<int>(t.y / 50);
		if(_x > 15 || _y > 15){
			//std::cout << "Return2 ";
			return;
		};
		square.setPosition({
			static_cast<float> (50 * _x + 5),
			static_cast<float> (50 * _y + 5)
		});
		win.draw(square);

	};

	void drawWinningIndication(sf::RenderWindow& win){
		static unsigned short delay_flash = 0;
		if(gameStat::moves.size() < 5) return;
		std::vector<sf::Vector2i> won_cells = checkForWin(
			gameStat::moves[gameStat::moves.size()-1],
			gameStat::cells,
			gameStat::is_win
		);
		if(gameStat::is_win){
			delay_flash = (delay_flash + 1) % 1000;
			if(delay_flash > 500) return;
			for(sf::Vector2i i: won_cells){
				rect_shape.setPosition({50.f * i.x, 50.f * i.y});
				win.draw(rect_shape);
			};
		} else delay_flash = 0;
	};

	void drawScoreboard(sf::RenderWindow& win){
		win.draw(scoreboard);
		win.draw(scoreboard_title[globalConfig::language]);
		win.draw(XO_text[0]);
		win.draw(XO_text[1]);
		win.draw(XO_score[0]);
		win.draw(XO_score[1]);
	};

	void drawWinningPrompt(sf::RenderWindow& win, bool& first){
		win.draw(popup_box);
		if(first){
			sf::FloatRect bounds = win_prompt[globalConfig::language].getLocalBounds();
			win_prompt[globalConfig::language].setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
			win_prompt[globalConfig::language].setString(
				((((gameStat::moves.size() + gameStat::first_turn) % 2) == 1) ? "X" : "O")
				+ win_prompt[globalConfig::language].getString()
			);
			win_prompt[globalConfig::language].setPosition({980, 330});
			if(globalConfig::dark_mode){
				win_prompt[globalConfig::language].setFillColor(sf::Color::White);
			} else{
				win_prompt[globalConfig::language].setFillColor(sf::Color::Black);
			};
		};
		win.draw(win_prompt[globalConfig::language]);
	};
};



//tested by Quang
void initGameScreen(bool start, sf::RenderWindow& win){

	caroTableView.setViewport(sf::FloatRect(
		{40.f/globalConfig::win_width, 40.f/globalConfig::win_height}, 
		{720.f/globalConfig::win_width, 720.f/globalConfig::win_height})
	);

	if(start) gameStat::moves.clear();

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

	most_recent_move.setTexture(texture::most_recent_move[globalConfig::dark_mode]);
	most_recent_move.setScale({
		50.f/texture::most_recent_move[globalConfig::dark_mode].getSize().x,
		50.f/texture::most_recent_move[globalConfig::dark_mode].getSize().y
	});

	XO_turn_to_play[globalConfig::language].setCharacterSize(25);
	XO_turn_to_play[globalConfig::language].setOutlineThickness(3);
	sf::FloatRect bounds = XO_turn_to_play[globalConfig::language].getLocalBounds();
	XO_turn_to_play[globalConfig::language].setOrigin({bounds.size.x / 2.f, 0});
	XO_turn_to_play[globalConfig::language].setPosition(win.mapPixelToCoords({875, 500}));

	scoreboard.setPosition({945, 500});
	
	scoreboard_title[globalConfig::language].setCharacterSize(25);
	scoreboard_title[globalConfig::language].setOutlineThickness(3);
	bounds = scoreboard_title[globalConfig::language].getLocalBounds();
	scoreboard_title[globalConfig::language].setOrigin({bounds.size.x / 2.f, 0});
	scoreboard_title[globalConfig::language].setPosition(win.mapPixelToCoords({1068, 500}));

	XO_text[0].setCharacterSize(50);
	XO_text[0].setOutlineThickness(3);
	XO_text[0].setFillColor(sf::Color::Red);
	bounds = XO_text[1].getLocalBounds();
	XO_text[0].setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
	XO_text[0].setPosition(win.mapPixelToCoords({1000, 540}));

	XO_text[1].setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
	XO_text[1].setCharacterSize(50);
	XO_text[1].setPosition(win.mapPixelToCoords({1090, 540}));
	XO_text[1].setOutlineThickness(3);
	XO_text[1].setFillColor(sf::Color::Green);

	XO_score[0].setString(intToString(gameStat::x_score));
	XO_score[0].setOutlineThickness(3);
	XO_score[0].setCharacterSize(30);
	XO_score[0].setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
	bounds = XO_score[0].getLocalBounds();
	XO_score[0].setPosition(win.mapPixelToCoords({1000, 590}));

	XO_score[1].setString(intToString(gameStat::y_score));
	XO_score[1].setCharacterSize(30);
	XO_score[1].setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
	XO_score[1].setOutlineThickness(3);
	bounds = XO_score[1].getLocalBounds();
	XO_score[1].setPosition(win.mapPixelToCoords({1090, 590}));
	
	popup_box.setPosition(win.mapPixelToCoords({790, 300}));
	
	if(globalConfig::dark_mode){

		rect_shape.setFillColor(sf::Color(255, 255, 0, 150));
		scoreboard.setFillColor(sf::Color(0, 0, 0, 100));

		XO_turn_to_play[globalConfig::language].setFillColor(sf::Color::White);
		XO_turn_to_play[globalConfig::language].setOutlineColor(sf::Color::Black);

		scoreboard_title[globalConfig::language].setFillColor(sf::Color::White);
		scoreboard_title[globalConfig::language].setOutlineColor(sf::Color::Black);
		
		XO_text[0].setOutlineColor(sf::Color::Black);
		XO_text[1].setOutlineColor(sf::Color::Black);

		XO_score[0].setFillColor(sf::Color::White);
		XO_score[1].setFillColor(sf::Color::White);
		
		popup_box.setFillColor(sf::Color(4, 50, 84, 150));

	} else{

		rect_shape.setFillColor(sf::Color(255, 0, 0, 100));
		scoreboard.setFillColor(sf::Color(255, 255, 255, 150));

		XO_turn_to_play[globalConfig::language].setFillColor(sf::Color::Black);
		XO_turn_to_play[globalConfig::language].setOutlineColor(sf::Color::White);

		scoreboard_title[globalConfig::language].setFillColor(sf::Color::Black);
		scoreboard_title[globalConfig::language].setOutlineColor(sf::Color::White);

		XO_text[0].setOutlineColor(sf::Color::White);
		XO_text[1].setOutlineColor(sf::Color::White);

		XO_score[0].setFillColor(sf::Color::Black);
		XO_score[1].setFillColor(sf::Color::Black);

		popup_box.setFillColor(sf::Color(239, 247, 7, 150));
	};

};

//tested by Quang
//return value:
//		false -> the game_screen terminated (move to another screen);
//		true -> re-init the game_screen.
bool loopGameScreen(sf::RenderWindow& win){

	int delay = 0;

	while(win.isOpen()){

		if(globalConfig::current_win != 3){
			if(++delay == 100) return false;
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
				return false;
			};

			//draw a move
			if(event->is<sf::Event::MouseButtonPressed> () &&
				sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				if(!gameStat::is_win){
					events::addAMove(win.mapPixelToCoords(sf::Mouse::getPosition(win), caroTableView));
					events::undoMove(win);
				};
				if(events::switchLightDarkMode(win)) return true;
				events::openInGameSettings(win);
			};

		};

		//draw
		
		win.clear();

		// - draw background
		win.draw(background);
		
		// - draw scoreboard
		draw::drawScoreboard(win);

		// - draw win prompt
		{
			static bool first = true;
			if(gameStat::is_win){
				draw::drawWinningPrompt(win, first);
				first = false;
			} else first = true;
		};

		// - draw undo button
		draw::drawUndoButton(win);
		draw::drawSwitchLightDarkModeButton(win);
		draw::drawSettingsButton(win);

		// - draw players' turn status
		draw::drawTurnOfXOText(win);

		// - draw caro table and moves
		win.setView(caroTableView);
		win.draw(caro_table);

		// - draw mouse hover into cells
		draw::drawMouseHoverIntoCells(win);

		// - draw winning indication
		draw::drawWinningIndication(win);

		// - draw all moves onto the caro table
		draw::drawAllMoves(win);

		// - draw indication for the most recent moves
		draw::drawLastMoveMarker(win);

		win.setView(win.getDefaultView());

		std::cout << "Mouse pos: "
			<< sf::Mouse::getPosition(win).x << "  " << sf::Mouse::getPosition(win).y << '\n';
		win.display();

	};

	return false;
};

//tested by Quang
void drawGameScreen(sf::RenderWindow& win, bool start){
	bool re_init = start;
	do{
		initGameScreen(!re_init, win);
		re_init = loopGameScreen(win);
	} while(re_init);
};

#endif