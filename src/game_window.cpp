#ifndef GAME_WINDOW_CPP
#define GAME_WINDOW_CPP

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>


namespace globalConfig{
	int win_width = 1200, win_height = 800;
	bool dark_mode = 0;
};


namespace gameStat{
	std::vector<sf::Vector2i> moves;
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
		};
};

sf::Sprite
	caro_table(texture::caro_table[globalConfig::dark_mode]),
	background(texture::background[globalConfig::dark_mode]),
	x_character(texture::x_character[globalConfig::dark_mode]),
	o_character(texture::o_character[globalConfig::dark_mode]),
	undo_button(texture::normal_undo_button[globalConfig::dark_mode]);


//-------------------CONTROL SECTION------------------------

//tested by Quang
void addAMove(sf::Vector2f pos){
	if(!sf::FloatRect({0.f, 0.f}, {800.f, 800.f}).contains(pos)) return;
	int _x = (int)pos.x / 50, _y = (int)pos.y / 50;
	if(_x > 15 || _x < 0 || _y > 15 || _y < 0) return;
	gameStat::moves.push_back(sf::Vector2i(_x, _y));
};

void undoMove(){
	if(!gameStat::moves.empty()){
		gameStat::moves.pop_back();
	};
};


//-------------------DRAW SECTION------------------------

//tested by Quang
void initGameScreen(bool dark = false){

	caroTableView.setViewport(sf::FloatRect(
		{40.f/globalConfig::win_width, 40.f/globalConfig::win_height}, 
		{720.f/globalConfig::win_width, 720.f/globalConfig::win_height})
	);

	gameStat::moves.clear();

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
void loopGameScreen(sf::RenderWindow& win){
	while(win.isOpen()){
		
		//check for events
		while(std::optional event = win.pollEvent()){
			
			// close window
			if(event->is<sf::Event::Closed> ())
				win.close();

			//draw a move
			if(event->is<sf::Event::MouseButtonPressed> () &&
				sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
			{
				addAMove(win.mapPixelToCoords(sf::Mouse::getPosition(win), caroTableView));
			};

		};

		//draw
		
		win.clear();

		// - draw background
		win.draw(background);

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
};

//tested by Quang
void draw_game_screen(sf::RenderWindow& win){
	initGameScreen();
	loopGameScreen(win);
};

#endif