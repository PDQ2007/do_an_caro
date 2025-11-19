#ifndef SETTINGS_WINDOW_CPP
#define SETTINGS_WINDOW_CPP

#include "config.h"

// make everything defined below local
namespace {

namespace settings_stats{
	unsigned short current_tab = 0; // 0 = general, 1 = in-game
};

namespace fonts{
	sf::Font
		minecraft("resources/fonts/minecraft-f2d-v1-42.ttf"),
		mono_signature("resources/fonts/MomoSignature-Regular.ttf");
};

namespace textures{
	sf::Texture background[2] = {
		sf::Texture("resources/settings_screen/light/background.png"),
		sf::Texture("resources/settings_screen/dark/background.png")
	};
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

	void tab_general_clicked(){
		settings_stats::current_tab = 0;
	};

	void tab_inGame_clicked(){
		settings_stats::current_tab = 1;
	};

	void tab_backToPreviousScreen_clicked(){
		globalConfig::current_win = globalConfig::previous_win;
	};

};

namespace draw{

	void backgroundAndTitle(sf::RenderWindow& win, bool& start){

		static sf::Sprite background(textures::background[globalConfig::dark_mode]);
		static sf::Text title(fonts::mono_signature);
		static std::wstring strings[2] = {
			L"Settings",
			L"Cài đặt"
		};
		
		if(start){
			background.setTexture(textures::background[globalConfig::dark_mode]);
			title.setString(strings[globalConfig::language]);
			if(globalConfig::dark_mode){
				title.setFillColor(sf::Color::White);
				title.setOutlineColor(sf::Color::Black);
			} else{
				title.setFillColor(sf::Color::Black);
				title.setOutlineColor(sf::Color::White);
			};
			title.setOutlineThickness(4);
			title.setCharacterSize(50);
			title.setOrigin({
				title.getLocalBounds().size.x / 2.f,
				title.getLocalBounds().size.y / 2.f
			});
			title.setPosition({600.f, 100.f});
		};

		win.draw(background);
		win.draw(title);
	};

	void settingsBox(sf::RenderWindow& win, bool& start){
		sf::RectangleShape box({1100.f, 500.f});
		sf::RectangleShape line({5.f, 500.f});
		if(start){
			box.setPosition({50.f, 175.f});
			line.setPosition({300.f, 175.f});
			if(globalConfig::dark_mode){
				box.setFillColor(sf::Color(5, 0, 69, 150));
				line.setFillColor(sf::Color(0, 0, 0));
			} else{
				box.setFillColor(sf::Color(255, 233, 161, 150));
				line.setFillColor(sf::Color(163, 150, 103));
			};
		};
		win.draw(box);
		win.draw(line);
	};

	bool init_tabs(
		sf::RenderWindow& win,
		std::optional<sf::Event>& event,
		std::wstring strings[2],
		sf::Text& text_obj,
		sf::RectangleShape& bounds_obj,
		sf::FloatRect bounds,
		bool& start
	){
		static bool return_val;
		short& lang = globalConfig::language;
		if(start){
			bounds_obj.setSize(bounds.size);
			bounds_obj.setPosition(bounds.position);
			text_obj.setFont(fonts::minecraft);
			text_obj.setString(strings[lang]);
			text_obj.setOutlineThickness(4);
			text_obj.setCharacterSize(20);
			text_obj.setOrigin({
				text_obj.getLocalBounds().size.x / 2.f,
				text_obj.getLocalBounds().size.y / 2.f
			});
			auto t = bounds_obj.getGlobalBounds();
			text_obj.setPosition({
				static_cast<float> (t.position.x + 0.5 * t.size.x),
				static_cast<float> (t.position.y + 0.5 * t.size.y)
			});
		};
		if(globalConfig::dark_mode){
			text_obj.setFillColor(sf::Color::White);
			text_obj.setOutlineColor(sf::Color::Black);
		} else{
			text_obj.setFillColor(sf::Color::Black);
			text_obj.setOutlineColor(sf::Color::White);
		};
		if(bounds_obj.getGlobalBounds().contains(win.mapPixelToCoords(sf::Mouse::getPosition(win)))){
			if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && event->is<sf::Event::MouseButtonPressed> ()){
				return_val = true;
			} else{
				return_val = false;
			};
			bounds_obj.setFillColor(sf::Color(0, 0, 0, 150));
		} else{
			bounds_obj.setFillColor(sf::Color(0, 0, 0, 0));
			return_val = false;
		};
		return return_val;
	};

	void tabs_general(sf::RenderWindow& win, std::optional<sf::Event>& event, bool& start){
		static std::wstring strings[2] = {
			L"General",
			L"Chung"
		};
		static sf::Text text_obj(fonts::minecraft);
		static sf::RectangleShape bounds;
		bool is_event = init_tabs(
			win, event, strings, text_obj, bounds, {{50, 175}, {250, 100}}, start
		);
		win.draw(bounds);
		win.draw(text_obj);
		if(is_event){
			std::thread processEvent(events::tab_general_clicked);
			processEvent.detach();
		};
	};

	void tabs_inGame(sf::RenderWindow& win, std::optional<sf::Event>& event, bool& start){
		static std::wstring strings[2] = {
			L"In-game",
			L"Trong trận"
		};
		static sf::Text text_obj(fonts::minecraft);
		static sf::RectangleShape bounds;
		bool is_event = init_tabs(
			win, event, strings, text_obj, bounds, {{50, 275}, {250, 100}}, start
		);
		win.draw(bounds);
		win.draw(text_obj);
		if(is_event){
			std::thread processEvent(events::tab_inGame_clicked);
			processEvent.detach();
		};
	};

	void tabs_backToPreviousScreen(sf::RenderWindow& win, std::optional<sf::Event>& event, bool& start){
		static std::wstring strings[2] = {
			L"Back",
			L"Trở về"
		};
		static sf::Text text_obj(fonts::minecraft);
		static sf::RectangleShape bounds;
		bool is_event = init_tabs(
			win, event, strings, text_obj, bounds, {{50, 575}, {250, 100}}, start
		);
		win.draw(bounds);
		win.draw(text_obj);
		if(is_event){
			std::thread processEvent(events::tab_backToPreviousScreen_clicked);
			processEvent.detach();
		};
	};

	namespace general{

		bool initOptions(
			sf::RenderWindow& win, 
			std::optional<sf::Event>& event,
			sf::Text& label_text_obj,
			sf::Text& button_text_obj,
			sf::Sprite& button,
			sf::Font font[2], //0 = label's font, 1 = button text's font
			std::wstring label_strings[2],
			std::wstring text_strings[2],
			unsigned short character_size,
			unsigned short outline_thickness,
			float y_pos,
			bool& start
		){

			// set label position and size

			if(start){
				label_text_obj.setFont(font[0]),
				button_text_obj.setFont(font[1]);
				label_text_obj.setString(label_strings[globalConfig::language]);
				label_text_obj.setOutlineThickness(outline_thickness);
				label_text_obj.setCharacterSize(character_size);
				label_text_obj.setPosition({325.f, y_pos});

				if(globalConfig::dark_mode){
					label_text_obj.setFillColor(sf::Color::White);
					label_text_obj.setOutlineColor(sf::Color::Black);
				} else{
					label_text_obj.setFillColor(sf::Color::Black);
					label_text_obj.setOutlineColor(sf::Color::White);
				};

			};

			

#define X button
#define X_ label_text_obj
#define Y(t) t.getGlobalBounds().size.y

			static sf::Color fill_color[2] = {sf::Color::Black, sf::Color::White};

			return setupTextButton(
				start,
				win,
				button,
				button_text_obj,
				textures::text_button,
				{
					{
						600,
						X_.getGlobalBounds().position.y
						- (Y(X) - Y(X_)) / 2.f
					}, 
					{
						150,
						50
					}
				},
				font[1],
				text_strings,
				character_size,
				fill_color,
				fill_color,
				0,
				event
			);

#undef X
#undef X_
#undef Y(t)

		};
		
		void language(sf::RenderWindow& win, std::optional<sf::Event>& event, bool& start){
			static sf::Text
				label_text_obj(fonts::minecraft),
				button_text_obj(fonts::minecraft);
			static std::wstring 
				label_strings[2] = {L"Choose language:", L"Chọn ngôn ngữ:"},
				text_strings[2] = {L"Tiếng Việt", L"English"};
			static sf::Sprite button(textures::text_button[0]);

			static sf::Font font[2] = {fonts::minecraft, fonts::minecraft};
			
			bool is_event = initOptions(
				win, 
				event,
				label_text_obj,
				button_text_obj,
				button,
				font,
				label_strings,
				text_strings,
				20,
				3,
				200,
				start
			);

			if(is_event){
				globalConfig::language = (globalConfig::language + 1) % 2;
			};
			
			win.draw(label_text_obj);
			win.draw(button);
			win.draw(button_text_obj);
		};

		void dark_mode(sf::RenderWindow& win, std::optional<sf::Event>& event, bool& start){
			static sf::Text
				label_text_obj(fonts::minecraft),
				button_text_obj(fonts::minecraft);
			static std::wstring 
				label_strings[2] = {L"Dark mode:", L"Chế độ tối:"},
				text_strings[2][2] = {
					{L"Tắt", L"Off"},
					{L"Bật", L"On"}
				};
			static sf::Sprite button(textures::text_button[0]);

			static sf::Font font[2] = {fonts::minecraft, fonts::minecraft};

			bool is_event = initOptions(
				win, 
				event,
				label_text_obj,
				button_text_obj,
				button,
				font,
				label_strings,
				text_strings[globalConfig::dark_mode],
				20,
				3,
				300,
				start
			);

			if(is_event){
				globalConfig::dark_mode = (globalConfig::dark_mode + 1) % 2;
			};

			win.draw(label_text_obj);
			win.draw(button);
			win.draw(button_text_obj);
		};

		void sound_on(sf::RenderWindow& win, std::optional<sf::Event>& event, bool& start){
			static sf::Text
				label_text_obj(fonts::minecraft),
				button_text_obj(fonts::minecraft);
			static std::wstring 
				label_strings[2] = {L"Sound:", L"Âm thanh:"},
				text_strings[2][2] = {
					{L"Tắt", L"Off"},
					{L"Bật", L"On"}
			};
			static sf::Sprite button(textures::text_button[0]);

			static sf::Font font[2] = {fonts::minecraft, fonts::minecraft};

			bool is_event = initOptions(
				win, 
				event,
				label_text_obj,
				button_text_obj,
				button,
				font,
				label_strings,
				text_strings[globalConfig::sound_on],
				20,
				3,
				400,
				start
			);

			if(is_event){
				globalConfig::sound_on = (globalConfig::sound_on + 1) % 2;
			};

			win.draw(label_text_obj);
			win.draw(button);
			win.draw(button_text_obj);
		};

	};


};

void drawForEachLoop(sf::RenderWindow& win, std::optional<sf::Event>& event, bool& re_init){
	
	win.setSize({
		globalConfig::win_width, 
		globalConfig::win_height
	});

	win.clear();

	draw::backgroundAndTitle(win, re_init);

	draw::settingsBox(win, re_init);

	draw::tabs_general(win, event, re_init);

	draw::tabs_inGame(win, event, re_init);

	draw::tabs_backToPreviousScreen(win, event, re_init);

	draw::general::language(win, event, re_init);
	draw::general::dark_mode(win, event, re_init);
	draw::general::sound_on(win, event, re_init);

	win.display();

};

void loopSettingsScreen(sf::RenderWindow& win){
	
	bool re_init = true;

	while(win.isOpen()){

		std::optional<sf::Event> event;

		if(globalConfig::current_win != 4) return;
		
		while(event = win.pollEvent()){
			if(event->is<sf::Event::Closed> ()){
				globalConfig::current_win = 0;
				return;
			} else{
				drawForEachLoop(win, event, re_init);
			};
		};

		drawForEachLoop(win, event, re_init);

	};

};

}; // make everything defined above local

void drawSettingsScreen(sf::RenderWindow& win){
	loopSettingsScreen(win);
};

#endif
