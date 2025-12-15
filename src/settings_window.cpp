#ifndef SETTINGS_WINDOW_CPP
#define SETTINGS_WINDOW_CPP

#include "config.h"

// make everything defined below local
namespace {

namespace settings_stats{
	unsigned short current_tab = 0, VERY_previous_tab = 0; // 0 = general, 1 = in-game
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
			if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && event && event->is<sf::Event::MouseButtonPressed> ()){
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

	void tabs_general(sf::RenderWindow& win, std::optional<sf::Event>& event, bool& start, bool& is_input_warning){
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
		if(is_event && !is_input_warning){
			std::thread processEvent(events::tab_general_clicked);
			processEvent.detach();
		};
	};

	void tabs_inGame(sf::RenderWindow& win, std::optional<sf::Event>& event, bool& start, bool& is_input_warning){
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
		if(is_event && !is_input_warning){
			std::thread processEvent(events::tab_inGame_clicked);
			processEvent.detach();
		};
	};

	void tabs_backToPreviousScreen(sf::RenderWindow& win, std::optional<sf::Event>& event, bool& start, bool& is_input_warning){
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
		if(is_event && !is_input_warning){
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
#undef Y

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

	namespace in_game {
		
		namespace multiplayer {

			void drawInputs(sf::RenderWindow& win, std::optional<sf::Event>& event, std::vector<int>& warning_list){

				auto drawBox = [&win, &event, &warning_list](
					char idx,
					std::vector<std::wstring> label_text_strings,
					sf::FloatRect bounds,
					int max_char,
					std::string& input_string
				) {
					
					//draw_warning	
					static sf::RectangleShape box_obj[3];
					static sf::Text 
						input_text_obj[3] = {
							sf::Text(fonts::minecraft),
							sf::Text(fonts::minecraft),
							sf::Text(fonts::minecraft)
						},
						label_text_obj[3] = {
							sf::Text(fonts::minecraft),
							sf::Text(fonts::minecraft),
							sf::Text(fonts::minecraft)
						};
					std::vector<sf::Color> 
						box_fill_colors = {
							sf::Color(255, 223, 148),
							sf::Color(14, 2, 117)
						},
						box_outline_colors = {
							sf::Color(18, 2, 156),
							sf::Color(214, 150, 0)
						},
						box_selected_outline_colors = {
							sf::Color(2, 250, 35),
							sf::Color(255, 0, 0)
						},
						text_fill_colors = {
							sf::Color::Black,
							sf::Color::White
						},
						text_outline_colors = {
							sf::Color::White,
							sf::Color::Black
						};
					wchar_t status = L'0';
					static bool isBoxSelected[3] = {0, 0, 0};
					setupInputBox(
						true,
						win,
						box_obj[idx],
						label_text_obj[idx],
						input_text_obj[idx],
						bounds,
						5,
						box_fill_colors,
						box_outline_colors,
						box_selected_outline_colors,
						fonts::minecraft,
						label_text_strings,
						20,
						text_fill_colors,
						text_outline_colors,
						4,
						event,
						input_string,
						max_char,
						isBoxSelected[idx],
						status //A = max_length warning, B = empty_warning
					);
					win.draw(box_obj[idx]);
					win.draw(input_text_obj[idx]);
					win.draw(label_text_obj[idx]);

					//process selection
					if(isBoxSelected[idx]){
						isBoxSelected[(idx + 1) % 3] = false;
						isBoxSelected[(idx + 2) % 3] = false;
					};

					//process warning
					if(status == L'A'){
						warning_list[0] = -1;
					} else if(status == L'B'){
						warning_list[1] = -1;
					} else if(status != L'0'){
						warning_list[2] = -1;
						warning_list[3] = static_cast<int> (status);
					};
				};

				drawBox(
					0,
					{L"Save name", L"Tên ván chơi"},
					{{350, 240}, {750, 60}},
					38,
					globalConfig::current_package.save_name
				);

				drawBox(
					1,
					{L"Player X's name", L"Tên người chơi X"},
					{{350, 360}, {350, 60}},
					20,
					globalConfig::current_package.playerX_name
				);

				drawBox(
					2,
					{L"Player O's name", L"Tên người chơi O"},
					{{750, 360}, {350, 60}},
					20,
					globalConfig::current_package.playerO_name
				);
				
			};

#define WARNING_SHOWTIME 10000
#define WARNING_FLASHINGTIME 2000
			void drawWarnings(sf::RenderWindow&win, std::optional<sf::Event>& event, std::vector<int>& warning_list){
				static std::wstring warning_strings[3][2] = {
					{
						L"Name cannot be too long.",
						L"Tên không thể quá dài."
					},
					{
						L"Name cannot be empty.",
						L"Tên không thể để trống."
					},
					{
						L"Character 'A' is not allowed.",
						L"Kí tự 'A' không được cho phép."
					}
				};
				char idx[3] = {0, 1, 2};
				
				auto decreaseWarning = [](int& count){
					if(count == -1) count = WARNING_SHOWTIME;
					else count = (--count >= 0) ? count : 0;
				};

				decreaseWarning(warning_list[0]);
				decreaseWarning(warning_list[1]);
				decreaseWarning(warning_list[2]);

				if(warning_list[2] > 0){
					// Replace the 'X' in the string with the invalid character
					warning_strings[2][0][11] = static_cast<wchar_t> (warning_list[3]); 
					warning_strings[2][1][7] = static_cast<wchar_t> (warning_list[3]); 
				};

				for(int i = 0; i < 2; ++i){
					for(int j = i + 1; j < 3; ++j){
						if(warning_list[idx[i]] < warning_list[idx[j]]){
							std::swap(idx[i], idx[j]);
						};
					};
				};

				//draw warning texts
				static sf::Text warning_text_obj(fonts::minecraft);
				static int count_flashing = 0;
				count_flashing = (count_flashing + 1) % (int)(3.0/2 * (float)WARNING_FLASHINGTIME);
				for(int i = 0; i < 3; ++i){
					if(warning_list[idx[0]] == 0){
						count_flashing = 0;
						break;
					};
					if(warning_list[idx[i]] > 0){
						int outline_thickness = 4;
						if(count_flashing < WARNING_FLASHINGTIME){
							outline_thickness = 4;
						} else{
							outline_thickness = 0;
						};
						std::vector<sf::Color> 
							text_fill_colors = {
								sf::Color(222, 2, 2),
								sf::Color(252, 61, 61)
							},
							text_outline_colors = {
								sf::Color::Yellow,
								sf::Color::Yellow
							};
						setUpTextObj(
							true,
							win,
							warning_text_obj,
							text_fill_colors,
							text_outline_colors,
							outline_thickness,
							20,
							{0.5, 0.5},
							{600, static_cast<float> (100 + 50 * i)},
							warning_strings[idx[i]]
						);
						win.draw(warning_text_obj);
					} else{
						break;
					};
				};

			};

			void drawAll(sf::RenderWindow& win, std::optional<sf::Event>& event, bool& is_input_warning){
				static std::vector<int> warning_list = {0, 0, 0, 0};
				// 0th element => max_length warning
				// 1st element => empty_warning
				// 2nd element => invalid input
				// 3rd element => invalid character

				if((globalConfig::current_win != 4) || (settings_stats::current_tab != settings_stats::VERY_previous_tab)){
					warning_list = {0, 0, 0, 0};
				};

				is_input_warning = (globalConfig::current_package.save_name.size() == 0)
					|| (globalConfig::current_package.playerX_name.size() == 0)
					|| (globalConfig::current_package.playerO_name.size() == 0);

				drawInputs(win, event, warning_list);

				drawWarnings(win, event, warning_list);
			};
		};

		namespace singleplayer {

			void difficultyOption(sf::RenderWindow& win, std::optional<sf::Event>& event, bool& init){

				// draw label
				static sf::Text label_obj(fonts::minecraft);
				std::vector<sf::Color>
					fill_colors = {sf::Color::Black, sf::Color::White},
					outline_colors = {sf::Color::White, sf::Color::Black};
				std::wstring label_strings[2] = {
					L"Difficulty:",
					L"Độ khó:"
				};
				setUpTextObj(
					init,
					win,
					label_obj,
					fill_colors,
					outline_colors,
					4,
					30,
					{0, 0.5},
					{300, 600},
					label_strings
				);
				win.draw(label_obj);

				sf::Color
					text_fill_colors[2] = {sf::Color::Black, sf::Color::White},
					text_outline_colors[2] = {sf::Color::White, sf::Color::Black};
				static sf::Sprite button_obj(textures::text_button[0]);
				static sf::Text text_obj(fonts::minecraft);
				std::wstring text_strings[3][2] = {
					{
						L"Easy",
						L"Dễ"
					},
				{
					L"Medium",
					L"Trung bình"
				},
				{
					L"Hard",
					L"Khó"
				}
				};
				bool is_event = setupTextButton(
					init,
					win,
					button_obj,
					text_obj,
					textures::text_button,
					{{440, 550}, {300, 60}},
					fonts::minecraft,
					text_strings[gameStats::difficulty],
					25,
					text_fill_colors,
					text_outline_colors,
					4,
					event
				);

				if(is_event){
					gameStats::difficulty = (gameStats::difficulty + 1) % 3;
				};

				win.draw(button_obj);
				win.draw(text_obj);

			};

		};
	};


};

void drawForEachLoop(sf::RenderWindow& win, std::optional<sf::Event>& event, bool& re_init){
	
	static bool tab_change = false;

	win.setSize({
		globalConfig::win_width, 
		globalConfig::win_height
	});

	static bool is_input_warning = false;

	win.clear();

	draw::backgroundAndTitle(win, re_init);

	draw::settingsBox(win, re_init);

	draw::tabs_general(win, event, re_init, is_input_warning);

	if(globalConfig::previous_win == 3)
		draw::tabs_inGame(win, event, re_init, is_input_warning);

	draw::tabs_backToPreviousScreen(win, event, re_init, is_input_warning);

	if(settings_stats::current_tab == 0){
		draw::general::language(win, event, re_init);
		draw::general::dark_mode(win, event, re_init);
		draw::general::sound_on(win, event, re_init);
	} else if(settings_stats::current_tab == 1){
		draw::in_game::multiplayer::drawAll(win, event, is_input_warning);
		if(!gameStats::saveInfo.is_multiplayer){
			draw::in_game::singleplayer::difficultyOption(win, event, re_init);
		};
	};

	win.display();

	if(settings_stats::current_tab != settings_stats::VERY_previous_tab && !tab_change){
		tab_change = true;
	} else{
		tab_change = false;
		settings_stats::VERY_previous_tab = settings_stats::current_tab;
	};

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
	settings_stats::current_tab = 0;
	settings_stats::VERY_previous_tab = 0;
	loopSettingsScreen(win);
};

#endif
