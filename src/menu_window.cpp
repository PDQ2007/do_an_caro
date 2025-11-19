#ifndef MENU_WINDOW_CPP
#define MENU_WINDOW_CPP

#include "config.h"

namespace {

namespace fonts{
	sf::Font minecraft("resources/fonts/minecraft-f2d-v1-42.ttf");
};

namespace textures{
	sf::Texture text_button[6] = {
		sf::Texture("resources/menu_screen/light/normal_button.png"),
		sf::Texture("resources/menu_screen/light/hovered_button.png"),
		sf::Texture("resources/menu_screen/light/clicked_button.png"),
		sf::Texture("resources/menu_screen/dark/normal_button.png"),
		sf::Texture("resources/menu_screen/dark/hovered_button.png"),
		sf::Texture("resources/menu_screen/dark/clicked_button.png")
	};
};

namespace events{

	void nothing(){
		return;
	};

	void playButtonClicked(){
		for(int delay = 0; delay <= 1000000; ++delay){
		};
		globalConfig::current_win = 2;
	};

	void settingsButtonClicked(){
		for(int delay = 0; delay <= 1000000; ++delay){
		};
		globalConfig::current_win = 4;
	};

};

namespace draw{

	void backgroundAndLogo(sf::RenderWindow& win, bool& start){
		static sf::Texture texture_background[2] = {
			sf::Texture("resources/menu_screen/light/background.png"),
			sf::Texture("resources/menu_screen/dark/background.png")
		};

		static sf::Sprite background(texture_background[globalConfig::dark_mode]);
		win.draw(background);

		static sf::Texture texture_logo("resources/menu_screen/light/logo.png");
		static sf::Sprite logo(texture_logo);

		if(start){
			background.setTexture(texture_background[globalConfig::dark_mode]);
			logo.setScale({
				400.f / texture_logo.getSize().x,
				400.f / texture_logo.getSize().x
			});
			logo.setOrigin({
				logo.getLocalBounds().size.x / 2.f,
				logo.getLocalBounds().size.y / 2.f
			});
			logo.setPosition({
				600.f,
				200.f
			});
		};

		win.draw(logo);

	};

	void playButton(
		sf::RenderWindow& win,
		std::optional<sf::Event>& event
	){
		
		static sf::Sprite button(textures::text_button[0]);
		static sf::Text text(fonts::minecraft);
		static std::wstring strings[2] = {
			L"Play",
			L"Chơi"
		};
		static sf::Color colors[2][2] =
		{ // Fill Colors
			{
				sf::Color::Black,
				sf::Color::White
			},
		  // Outline Colors
			{
				sf::Color::White,
				sf::Color::Black
			}
		};

		static bool value;
		value = setupTextButton(
			true, win, button, text,
			textures::text_button,
			{{450, 350}, {300, 100}},
			fonts::minecraft,
			strings,
			30,
			colors[0],
			colors[1],
			4,
			event
		);

		//process event
		if(value){
			std::thread processEvent(events::playButtonClicked);
			processEvent.detach();
		};

		win.draw(button);
		win.draw(text);
		
	};

	void settingsButton(
		sf::RenderWindow& win,
		std::optional<sf::Event>& event
	){
		static sf::Sprite button(textures::text_button[0]);
		static sf::Text text(fonts::minecraft);
		static std::wstring strings[2] = {
			L"Settings",
			L"Cài đặt"
		};
		static sf::Color colors[2][2] =
		{ // Fill Colors
			{
				sf::Color::Black,
				sf::Color::White
			},
			// Outline Colors
			{
				sf::Color::White,
				sf::Color::Black
			}
		};

		static bool value;

		value = setupTextButton(
			true, win, button, text,
			textures::text_button,
			{{450, 500}, {300, 100}},
			fonts::minecraft,
			strings,
			30,
			colors[0],
			colors[1],
			4,
			event
		);

		//process event
		if(value){
			std::thread processEvent(events::settingsButtonClicked);
			processEvent.detach();
		};

		win.draw(button);
		win.draw(text);
	};

};

void drawForEachLoop(sf::RenderWindow& win, std::optional<sf::Event>& event, bool& start){
	
	win.setSize({
		globalConfig::win_width, 
		globalConfig::win_height
	});
	
	win.clear();

	draw::backgroundAndLogo(win, start);

	draw::playButton(win, event);

	draw::settingsButton(win, event);

	win.display();
};

bool loopMenuWindow(sf::RenderWindow& win, bool start){
	while(win.isOpen()){

		if(globalConfig::current_win != 1) return false;

		std::optional<sf::Event> event;
		while(event = win.pollEvent()){
			if(event->is<sf::Event::Closed> ()){
				globalConfig::current_win = 0;
				return false;
			};

			drawForEachLoop(win, event, start);

		};

		drawForEachLoop(win, event, start);

		start = false;
	};
};

}; // blank namespace => make everything defined here local

// the only function to be call to open the menu view
void drawMenuWindow(sf::RenderWindow& win){
	bool re_init = false;
	do{
		re_init = loopMenuWindow(win, true);
	} while(re_init);
};



#endif
