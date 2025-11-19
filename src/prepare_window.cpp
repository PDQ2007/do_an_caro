#ifndef PREPARE_WINDOW_CPP
#define PREPARE_WINDOW_CPP

#include "config.h"

namespace { //make everything below local

namespace fonts{
	sf::Font minecraft("resources/fonts/minecraft-f2d-v1-42.ttf");
};

namespace textures{
	sf::Texture
		background("resources/prepare_screen/light/background.png"),
		load_list_frame[2] = {
			sf::Texture("resources/prepare_screen/light/load_game_list_box.png"),
			sf::Texture("resources/prepare_screen/dark/load_game_list_box.png")
		},
		text_button[6] = {
			sf::Texture("resources/menu_screen/light/normal_button.png"),
			sf::Texture("resources/menu_screen/light/hovered_button.png"),
			sf::Texture("resources/menu_screen/light/clicked_button.png"),
			sf::Texture("resources/menu_screen/dark/normal_button.png"),
			sf::Texture("resources/menu_screen/dark/hovered_button.png"),
			sf::Texture("resources/menu_screen/dark/clicked_button.png")
		},
		back_button[2] = {
			sf::Texture("resources/prepare_screen/light/normal_back.png"),
			sf::Texture("resources/prepare_screen/light/hovered_back.png")
		},
		go_next_button[6] = {
			sf::Texture("resources/prepare_screen/light/normal_next.png"),
			sf::Texture("resources/prepare_screen/dark/normal_next.png"),
			sf::Texture("resources/prepare_screen/light/hovered_next.png"),
			sf::Texture("resources/prepare_screen/dark/hovered_next.png"),
			sf::Texture("resources/prepare_screen/light/clicked_next.png"),
			sf::Texture("resources/prepare_screen/dark/clicked_next.png")
		},
		go_back_button[6] = {
			sf::Texture("resources/prepare_screen/light/normal_goback.png"),
			sf::Texture("resources/prepare_screen/dark/normal_goback.png"),
			sf::Texture("resources/prepare_screen/light/hovered_goback.png"),
			sf::Texture("resources/prepare_screen/dark/hovered_goback.png"),
			sf::Texture("resources/prepare_screen/light/clicked_goback.png"),
			sf::Texture("resources/prepare_screen/dark/clicked_goback.png")
		};
};



namespace internalStats{

	char stage = 0;
		// 0 = asking for load game or new game
		// 1 = screen for game loading
		// 2 = screen for new game
	
	std::vector<std::string> 
		single_player_game_list,
		multiplayer_game_list;

};

namespace process {
	void loadListOfGames(){
		std::string path = "saves";
		internalStats::single_player_game_list.clear();
		internalStats::multiplayer_game_list.clear();
		for(const auto& entry: std::filesystem::directory_iterator(path)){
			if(entry.path().extension().string() == "txt")
				internalStats::multiplayer_game_list.push_back(entry.path().stem().string());
			else if(entry.path().extension().string() == "txtx")
				internalStats::single_player_game_list.push_back(entry.path().stem().string());
		};
	};
};

namespace events{
	
	void loadGameClicked(){
		internalStats::stage = 1;
	};
	
	void newGameClicked(){
		std::cout << "new game clicked\n";
	};

	void goBackClicked(){		

		if(internalStats::stage > 0) internalStats::stage = 0;
		else{
			globalConfig::current_win = 1;
		};

	};

	void goNextListClicked(){
		std::cout << "Next clicked\n";
	};

	void goBackListClicked(){
		std::cout << "Back clicked\n";
	};

};

namespace draw{

	void background(sf::RenderWindow& win){
		static sf::Sprite background(textures::background);
		background.setScale({
			globalConfig::win_width / background.getLocalBounds().size.x,
			globalConfig::win_height / background.getLocalBounds().size.y
		});
		win.draw(background);
	};

	void goBackButton(sf::RenderWindow& win, std::optional<sf::Event> event, bool& init){
		static sf::Sprite go_back_button(textures::back_button[0]);
		if(init){
			go_back_button.setScale({
				75.f / go_back_button.getLocalBounds().size.x,
				75.f / go_back_button.getLocalBounds().size.y
			});
			go_back_button.setPosition({50.f, 50.f});
		};
		if(go_back_button.getGlobalBounds().contains(win.mapPixelToCoords(sf::Mouse::getPosition(win)))){
			if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)
				&& event->is<sf::Event::MouseButtonPressed> ()
			){
				std::thread processEvent(events::goBackClicked);
				processEvent.detach();
			};
			go_back_button.setTexture(textures::back_button[1]);
		} else{
			go_back_button.setTexture(textures::back_button[0]);
		};
		win.draw(go_back_button);
	};

	// ------------ STAGE 0 -----------------
	void stageZero(sf::RenderWindow& win, std::optional<sf::Event> event, bool& init){
		static sf::Sprite
			load_game_button(textures::text_button[0]),
			new_game_button(textures::text_button[0]);
		static sf::Text
			load_game_text(fonts::minecraft),
			new_game_text(fonts::minecraft);
		static std::wstring
			load_game_strings[2] = {
				L"Load game",
				L"Chơi ván cũ"
			},
			new_game_strings[2] = {
				L"New game",
				L"Chơi ván mới"
			};
		static sf::Color
			fill_colors[2] = {sf::Color::Black, sf::Color::White},
			outline_colors[2] = {sf::Color::White, sf::Color::Black};

		bool is_event = setupTextButton(
			init,
			win,
			load_game_button,
			load_game_text,
			textures::text_button,
			{{275, 350}, {300, 100}},
			fonts::minecraft,
			load_game_strings,
			30,
			fill_colors,
			outline_colors,
			4,
			event
		);

		win.draw(load_game_button);
		win.draw(load_game_text);

		if(is_event){
			std::thread processEvent(events::loadGameClicked);
			processEvent.detach();
		};

		is_event = setupTextButton(
			init,
			win,
			new_game_button,
			new_game_text,
			textures::text_button,
			{{615, 350}, {300, 100}},
			fonts::minecraft,
			new_game_strings,
			30,
			fill_colors,
			outline_colors,
			4,
			event
		);
		win.draw(new_game_button);
		win.draw(new_game_text);

		if(is_event){
			std::thread processEvent(events::newGameClicked);
			processEvent.detach();
		};
	};

	// ------------ STAGE 1 -----------------

	//void gameNames

	void stageOne(sf::RenderWindow& win, std::optional<sf::Event> event, bool& init){
		process::loadListOfGames();

		// draw backframe
		static sf::Sprite backframe(textures::load_list_frame[0]);
		if(globalConfig::dark_mode){
			backframe.setTexture(textures::load_list_frame[1]);
		} else{
			backframe.setTexture(textures::load_list_frame[0]);
		};
		if(init){
			backframe.setPosition({50, 100});
			backframe.setScale({
				1100.f / backframe.getLocalBounds().size.x,
				650.f / backframe.getLocalBounds().size.y
			});
		};
		win.draw(backframe);
		
		// draw go_next button
		static sf::Sprite go_next_button(textures::go_next_button[0]);
		bool is_event = setupIconButton(
			init,
			win,
			go_next_button,
			textures::go_next_button,
			{{625, 615}, {100, 100}},
			event
		);
		if(is_event){
			std::thread processEvent(events::goNextListClicked);
			processEvent.detach();
		};
		win.draw(go_next_button);

		// draw go_back button
		static sf::Sprite go_back_button(textures::go_next_button[0]);
		is_event = setupIconButton(
			init,
			win,
			go_back_button,
			textures::go_back_button,
			{{475, 615}, {100, 100}},
			event
		);
		if(is_event){
			std::thread processEvent(events::goBackListClicked);
			processEvent.detach();
		};
		win.draw(go_back_button);
	};

};

void drawForEachLoop(sf::RenderWindow& win, std::optional<sf::Event>& event, bool init){

	win.clear();

	draw::background(win);
	draw::goBackButton(win, event, init);

	if(internalStats::stage == 0){

		draw::stageZero(win, event, init);

	} else if(internalStats::stage == 1){

		draw::stageOne(win, event, init);

	};

	win.display();

};

// return true if re-init is needed
bool loopPrepareScreen(sf::RenderWindow& win, bool& init){
	
	while(win.isOpen()){

		if(globalConfig::current_win != 2) return false;
		
		std::optional<sf::Event> event;

		while(event = win.pollEvent()){
			if(event->is<sf::Event::Closed> ()){
				globalConfig::current_win = 0;
				return false;
			};
			
			drawForEachLoop(win, event, init);

		};

		drawForEachLoop(win, event, init);

	};
};

}; //make everything above local

void drawPrepareScreen(sf::RenderWindow& win){
	bool start = true;
	while(start){
		start = loopPrepareScreen(win, start);
	};
};

#endif