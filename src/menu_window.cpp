#ifndef MENU_WINDOW_CPP
#define MENU_WINDOW_CPP

#include "config.h"

namespace {

namespace fonts{
	sf::Font minecraft("resources/fonts/minecraft-f2d-v1-42.ttf");
};

namespace textures{
};



namespace events{

	void nothing(){
		return;
	};

	void print(){
		std::cout << "Test\n";
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

	void playButton(sf::RenderWindow& win){
		
		std::optional<sf::Event> event;
		
		static text_button play_button{
			{
				"resources/menu_screen/light/normal_button.png",
				"resources/menu_screen/light/hovered_button.png",
				"resources/menu_screen/light/clicked_button.png",
				"resources/menu_screen/dark/normal_button.png",
				"resources/menu_screen/dark/hovered_button.png",
				"resources/menu_screen/dark/clicked_button.png"
			},
			{L"Play", L"Chõi"},
			30,
			{sf::Color::Black, sf::Color::White},
			{sf::Color::White, sf::Color::Black},
			4,
			{0.5, 0.5},
			fonts::minecraft,
			{0.5, 0.5},
			{600.f, 400.f},
			{300.f, 100.f}
		};

		play_button.draw(win, event);
		
	};

};

void drawForEachLoop(sf::RenderWindow& win, std::optional<sf::Event>& events, bool& start){
	win.clear();

	draw::backgroundAndLogo(win, start);

	draw::playButton(win);

	win.display();
};

bool loopMenuWindow(sf::RenderWindow& win, bool start){
	while(win.isOpen()){

		if(globalConfig::current_win != 1) return false;

		win.setSize({
			globalConfig::win_width, 
			globalConfig::win_height
		});

		std::optional<sf::Event> events;
		while(events = win.pollEvent()){
			if(events->is<sf::Event::Closed> ()){
				globalConfig::current_win = 0;
				return false;
			};

			drawForEachLoop(win, events, start);

		};

		drawForEachLoop(win, events, start);

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
