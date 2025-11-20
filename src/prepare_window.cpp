#ifndef PREPARE_WINDOW_CPP
#define PREPARE_WINDOW_CPP

#include "config.h"

namespace { //make everything below local

namespace fonts{
	sf::Font 
		minecraft("resources/fonts/minecraft-f2d-v1-42.ttf"),
		mono_signature("resources/fonts/MomoSignature-Regular.ttf");
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

#define SECOND 60
#define MINUTE 60
#define HOUR 24
#define DAY	31
#define MONTH 12
#define YEAR 255

// this struct stores saves' data
struct Saves_data{
	bool is_multiplayer;
	std::string name;
	unsigned int time[6]; // = {SECOND, MINUTE, HOUR, DAY, MONTH, YEAR}
	std::filesystem::path path;

	//constructor
	Saves_data(bool input_bool, std::string input_name, unsigned int input_time[6], std::filesystem::path path){
		this->is_multiplayer = input_bool;
		this->name = input_name;
		std::copy(input_time, input_time + 6, this->time);
		this->path = path;
	};

	// compare: a < b => a is newer than b
	bool operator< (Saves_data other){
		for(int i = 5; i >= 0; --i){
			if(this->time[i] > other.time[i]) return true;
			else if(this->time[i] < other.time[i]) return false;
		};
		return false;
	};

};

#undef HOUR
#undef MINUTE
#undef SECOND
#undef DAY
#undef MONTH
#undef YEAR

namespace internalStats{

	char stage = 0;
		// 0 = asking for load game or new game
		// 1 = screen for game loading
		// 2 = screen for new game

	char status = 0;
		// 0 = nothing
		// 1 = game loaded
		// 2 = new game created
	
	std::vector<Saves_data> list_of_saves;

};

namespace newGameData{

	bool
		is_new_game = false,
		is_multiplayer = false;
	std::string 
		save_name = "Untitled_save",
		playerX = "Unnamed_X",
		playerO = "Unnamed_O";
	int first_turn = 0; // 0 = X, 1 = O
	std::filesystem::path path{"saves"};

	void clear(){
		save_name = "Untitled_save";
		playerX = "Unnamed_X";
		playerO = "Unnamed_O";
		first_turn = 0;
	};

	gameDataPackage convertToPackage(){
		gameDataPackage package = {
			is_new_game,
			is_multiplayer,
			first_turn,
			save_name,
			playerX,
			playerO,
			path
		};
		return package;
	};

};

//ChatGPT's generated test code
void generate_random_saves() {
	std::random_device rd;
	std::mt19937 rng(rd());

	std::uniform_int_distribution<int> second_dist(0, 59);
	std::uniform_int_distribution<int> minute_dist(0, 59);
	std::uniform_int_distribution<int> hour_dist(0, 23);
	std::uniform_int_distribution<int> day_dist(1, 28);
	std::uniform_int_distribution<int> month_dist(1, 12);
	std::uniform_int_distribution<int> year_dist(2020, 2025);

	std::uniform_int_distribution<int> bool_dist(0, 1);

	std::uniform_int_distribution<int> name_len_dist(5, 10);
	std::uniform_int_distribution<int> char_dist_int('a', 'z');  
	// ^ FIX: MSVC does not allow uniform_int_distribution<char>

	for (int k = 0; k < 14; ++k) {
		// random name
		int len = name_len_dist(rng);
		std::string name;
		name.reserve(len);

		for (int i = 0; i < len; ++i) {
			char c = static_cast<char>(char_dist_int(rng)); // FIX
			name.push_back(c);
		}

		unsigned int time_val[6];
		time_val[0] = second_dist(rng);
		time_val[1] = minute_dist(rng);
		time_val[2] = hour_dist(rng);
		time_val[3] = day_dist(rng);
		time_val[4] = month_dist(rng);
		time_val[5] = year_dist(rng);

		internalStats::list_of_saves.emplace_back(bool_dist(rng), name, time_val, "path");
	}
};


namespace process {

	//written by ChatGPT
	void get_last_write_time(const std::filesystem::path& p, unsigned int out[6]) {

		// 1. Read the filesystem time
		auto ftime = std::filesystem::last_write_time(p);

		// 2. Convert to system_clock::time_point
		auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
			ftime - std::filesystem::file_time_type::clock::now()
			+ std::chrono::system_clock::now()
		);

		// 3. Convert to time_t
		std::time_t tt = std::chrono::system_clock::to_time_t(sctp);

		// 4. convert to broken-down local time (safe)
		std::tm tm_buf{};
#if defined(_WIN32)
		localtime_s(&tm_buf, &tt);
#else
		localtime_r(&tt, &tm_buf);
#endif

		// 5. fill: {SECOND, MINUTE, HOUR, DAY, MONTH, YEAR}
		out[0] = static_cast<unsigned char>(tm_buf.tm_sec);
		out[1] = static_cast<unsigned char>(tm_buf.tm_min);
		out[2] = static_cast<unsigned char>(tm_buf.tm_hour);
		out[3] = static_cast<unsigned char>(tm_buf.tm_mday);
		out[4] = static_cast<unsigned char>(tm_buf.tm_mon + 1);
		out[5] = static_cast<unsigned char>(tm_buf.tm_year + 1900);

	};

	void loadListOfGames(){

#define LIST internalStats::list_of_saves

		std::string path = "saves";
		LIST.clear();
		for(const auto& entry: std::filesystem::directory_iterator(path)){

			unsigned int time[6];
			get_last_write_time(entry.path(), time);

			//.txt = multiplayer
			//.txtx = single-player
			if(entry.path().extension().string() == ".txt"){ // is multiplayer
				LIST.push_back({
					true,
					entry.path().stem().string(),
					time,
					entry.path()
				});
			}
			else if(entry.path().extension().string() == ".txtx"){ // is NOT multiplayer (is singleplayer)
				LIST.push_back({
					false,
					entry.path().stem().string(),
					time,
					entry.path()
				});
			};

		};

		auto cmp = [](Saves_data a, Saves_data b) -> bool {
			return (a < b);
		};
		std::sort(LIST.begin(), LIST.end(), cmp);

#undef LIST

	};

};

namespace events{
	
	void loadGameClicked(){
		internalStats::stage = 1;
	};
	
	void newGameClicked(){
		internalStats::stage = 2;
	};

	void goBackClicked(){		

		if(internalStats::stage > 0) internalStats::stage = 0;
		else{
			newGameData::clear();
			globalConfig::current_win = 1;
		};

	};

	void goNextListClicked(unsigned int& idx){
		++idx;
	};

	void goBackListClicked(unsigned int& idx){
		--idx;
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
		static bool repeated_events = false;
		if(go_back_button.getGlobalBounds().contains(win.mapPixelToCoords(sf::Mouse::getPosition(win)))){
			if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)
				&& event->is<sf::Event::MouseButtonPressed> ()
			){
				if(repeated_events == false){
					repeated_events = true;
					std::thread processEvent(events::goBackClicked);
					processEvent.detach();
				};			
			};
			if(!event->is<sf::Event::MouseButtonPressed> ()){
				repeated_events = false;
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

	// return true if any save is chosen => proceed to load the
	//  chosen save into game_screen
	// otherwise return false.
	bool gameName(
		sf::RenderWindow& win,
		std::optional<sf::Event>& event,
		unsigned int idx,
		std::string name_string,
		bool classifier_type, // 0 = single play, 1 = multiplayer
		bool& init
	){

		static bool return_val = false;

		static bool event_ignore = true;

		static sf::Text
			idx_text(fonts::minecraft),
			name_text(fonts::minecraft),
			classifier(fonts::minecraft);

		// draw hover effect
		static sf::RectangleShape bounds;
		bounds.setSize({900, 50});
		bounds.setOrigin({
			bounds.getLocalBounds().size.x / 2.f,
			bounds.getLocalBounds().size.y / 2.f
		});
		bounds.setOutlineThickness(4);
		bounds.setPosition({600, static_cast<float> (200 + 60 * (idx % 7))});
		return_val = false;
		if(bounds.getGlobalBounds().contains(win.mapPixelToCoords(sf::Mouse::getPosition(win)))){
			if(globalConfig::dark_mode){
				bounds.setOutlineColor(sf::Color(255, 255, 255, 100));
			} else{
				bounds.setOutlineColor(sf::Color(255, 255, 255, 100));
			};
			if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
				if(globalConfig::dark_mode){
					bounds.setFillColor(sf::Color(54, 47, 162, 100));
				} else{
					bounds.setFillColor(sf::Color(246, 214, 84, 100));
				};
				if(event->is<sf::Event::MouseButtonPressed> ()){
					if(!event_ignore){
						return_val = true;
					};
					event_ignore = true;
				} else{
					event_ignore = false;
				};
			} else{
				bounds.setFillColor(sf::Color::Transparent);
			};
		} else{
			bounds.setOutlineColor(sf::Color::Transparent);
			bounds.setFillColor(sf::Color::Transparent);
		};
		win.draw(bounds);

		unsigned int t_idx = idx + 1;

		std::string
			idx_string = ".";

		// set idx_string and idx_text
		while(t_idx > 0){
			idx_string = (char)((t_idx % 10) + '0') + idx_string;
			t_idx /= 10;
		};

		idx_text.setString(idx_string);

		if(globalConfig::dark_mode){
			idx_text.setFillColor(sf::Color::White);
			idx_text.setOutlineColor(sf::Color::Black);
		} else{
			idx_text.setFillColor(sf::Color::Black);
			idx_text.setOutlineColor(sf::Color::White);
		};
		idx_text.setCharacterSize(20);
		idx_text.setOrigin({
			0,
			idx_text.getLocalBounds().size.y / 2.f
		});
		idx_text.setPosition({100, static_cast<float> (200 + 60 * (idx % 7))});

		win.draw(idx_text);

		// set name_text
		name_text.setString(name_string);
		if(globalConfig::dark_mode){
			name_text.setFillColor(sf::Color::White);
			name_text.setOutlineColor(sf::Color::Black);
		} else{
			name_text.setFillColor(sf::Color::Black);
			name_text.setOutlineColor(sf::Color::White);
		};
		name_text.setCharacterSize(20);
		name_text.setOrigin({
			0,
			name_text.getLocalBounds().size.y / 2.f
		});
		name_text.setPosition({150, static_cast<float> (200 + 60 * (idx % 7))});

		win.draw(name_text);

		// set classifier
		static std::wstring class_string[2][2] = {
			{L"Single player", L"Một người chơi"},
			{L"Multiplayer", L"Hai người chơi"}
		};
		classifier.setString(class_string[classifier_type][globalConfig::language]);
		if(globalConfig::dark_mode){
			classifier.setFillColor(sf::Color::White);
			classifier.setOutlineColor(sf::Color::Black);
		} else{
			classifier.setFillColor(sf::Color::Black);
			classifier.setOutlineColor(sf::Color::White);
		};
		classifier.setCharacterSize(20);
		classifier.setOrigin({
			0,
			classifier.getLocalBounds().size.y / 2.f
		});
		classifier.setPosition({800, static_cast<float> (200 + 60 * (idx % 7))});

		win.draw(classifier);

		return return_val;
	};

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

		// init page index
		static unsigned int page_index = 0;

		// draw list of saves

#define START 7 * page_index
#define LIST internalStats::list_of_saves

		auto min = [](unsigned int a, unsigned int b) -> unsigned int {
			return (a < b) * a + (a >= b) * b;
		};

		//std::cout << "loop check: " << START <<  " " << min(START + 7, LIST.size()) << '\n';

		namespace ngd = newGameData;

		for(unsigned int i = START; i < min(START + 7, LIST.size()); ++i){
			if(gameName(win, event, i, LIST[i].name, LIST[i].is_multiplayer, init)){
				ngd::path = LIST[i].path;
				ngd::is_new_game = false;
				internalStats::status = 1;
			};
		};

#undef START
		
		// draw go_next button
		
		static bool is_event;

		if((7 * page_index + 7) < LIST.size()){

			static sf::Sprite go_next_button(textures::go_next_button[0]);

			is_event = setupIconButton(
				init,
				win,
				go_next_button,
				textures::go_next_button,
				{{625, 615}, {100, 100}},
				event
			);

			if(is_event){
				std::thread processEvent(events::goNextListClicked, std::ref(page_index));
				processEvent.detach();
			};

			win.draw(go_next_button);
		};
		

		// draw go_back button
		if(page_index > 0){

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
				std::thread processEvent(events::goBackListClicked, std::ref(page_index));
				processEvent.detach();
			};

			win.draw(go_back_button);
		};
	};

	// -------------- STAGE 2 -------------------

	void singleplayerButton(sf::RenderWindow& win, std::optional<sf::Event> event, bool& init, char& selected){
		static sf::Text text_obj(fonts::minecraft);
		static sf::Sprite button_obj(textures::text_button[0]);
		static std::wstring strings[2] = {
			L"Single-player",
			L"Một người chơi"
		};
		static sf::Color fill_colors[2] = {
			sf::Color::White,
			sf::Color::Black
		};
		static sf::Color outline_colors[2] = {
			sf::Color::Black,
			sf::Color::White
		};
		bool is_event = setupTextButton(
			init,
			win,
			button_obj,
			text_obj,
			textures::text_button,
			{{300, 50}, {300, 100}},
			fonts::minecraft,
			strings,
			25,
			fill_colors,
			outline_colors,
			4,
			event
		);
		if(is_event){
			selected = 1;
		};
		win.draw(button_obj);
		win.draw(text_obj);
	};

	void multiplayerButton(sf::RenderWindow& win, std::optional<sf::Event> event, bool& init, char& selected){
		static sf::Text text_obj(fonts::minecraft);
		static sf::Sprite button_obj(textures::text_button[0]);
		static std::wstring strings[2] = {
			L"Multi-player",
			L"Hai người chơi"
		};
		static sf::Color fill_colors[2] = {
			sf::Color::White,
			sf::Color::Black
		};
		static sf::Color outline_colors[2] = {
			sf::Color::Black,
			sf::Color::White
		};
		bool is_event = setupTextButton(
			init,
			win,
			button_obj,
			text_obj,
			textures::text_button,
			{{650, 50}, {300, 100}},
			fonts::minecraft,
			strings,
			25,
			fill_colors,
			outline_colors,
			4,
			event
		);
		if(is_event){
			selected = 2;
		};
		win.draw(button_obj);
		win.draw(text_obj);
	};

	void singleplayerOptions(sf::RenderWindow& win, std::optional<sf::Event> event, bool& init){
		static sf::Text text(fonts::minecraft);
		text.setCharacterSize(30);
		text.setString("Feature Under Development...");
		text.setFillColor(sf::Color::White);
		text.setOutlineColor(sf::Color::Black);
		text.setOutlineThickness(5);
		text.setOrigin({
			text.getLocalBounds().size.x / 2.f,
			text.getLocalBounds().size.y / 2.f
		});
		text.setPosition({
			600.f, 400.f
		});
		win.draw(text);
	};
	
	namespace _multiplayerOptions{

		void title(sf::RenderWindow& win, std::optional<sf::Event> event, bool& init){
			// draw title -------------------
			static sf::Text title(fonts::mono_signature);
			title.setCharacterSize(40);
			static std::wstring strings[2] = {
				L"Multi-player",
				L"Hai người chơi"
			};
			title.setString(strings[globalConfig::language]);
			title.setOutlineThickness(5);
			if(globalConfig::dark_mode){
				title.setFillColor(sf::Color::White);
				title.setOutlineColor(sf::Color::Black);
			} else{
				title.setFillColor(sf::Color::Black);
				title.setOutlineColor(sf::Color::White);
			};
			title.setOrigin({
				title.getLocalBounds().size.x / 2.f,
				title.getLocalBounds().size.y / 2.f
				});
			title.setPosition({
				600.f, 200.f
				});
			win.draw(title);
		};
		
		void inputBoxes(sf::RenderWindow& win, std::optional<sf::Event> event, bool& init){
			
			namespace ngd = newGameData;
			
			static bool selected1 = false, selected2 = false, selected3 = false;
			// draw input box for save name -------------------
			static char status;
			static sf::Text
				saveName_inputBox_label(fonts::minecraft),
				saveName_inputBox_text(fonts::minecraft);
			static sf::RectangleShape saveName_inputBox_obj;

			// - draw input box
			setupInputBox(
				init,
				win,
				saveName_inputBox_obj,
				saveName_inputBox_label,
				saveName_inputBox_text,
				{
					{100, 250}, 
				{1000, 50}
				},
				5,
				{sf::Color(250, 222, 132), sf::Color(2, 46, 117)},
				{sf::Color(82, 63, 2), sf::Color(130, 178, 255)},
				{sf::Color(255, 0, 0), sf::Color(255, 0, 0)},
				fonts::minecraft,
				{L"Save name", L"Tên ván chơi"},
				25,
				{sf::Color::Black, sf::Color::White},
				{sf::Color::White, sf::Color::Black},
				5,
				event,
				ngd::save_name,
				38,
				selected1,
				status
			);
			win.draw(saveName_inputBox_obj);
			win.draw(saveName_inputBox_text);
			win.draw(saveName_inputBox_label);


			// draw input box for player 1's name -------------------
			static sf::Text
				player1Name_inputBox_label(fonts::minecraft),
				player1Name_inputBox_text(fonts::minecraft);
			static sf::RectangleShape player1Name_inputBox_obj;

			// - draw input box
			setupInputBox(
				init,
				win,
				player1Name_inputBox_obj,
				player1Name_inputBox_label,
				player1Name_inputBox_text,
				{
					{100, 350}, 
				{485, 50}
				},
				5,
				{sf::Color(250, 222, 132), sf::Color(2, 46, 117)},
				{sf::Color(82, 63, 2), sf::Color(130, 178, 255)},
				{sf::Color(255, 0, 0), sf::Color(255, 0, 0)},
				fonts::minecraft,
				{L"X player", L"Người chơi X"},
				25,
				{sf::Color::Black, sf::Color::White},
				{sf::Color::White, sf::Color::Black},
				5,
				event,
				ngd::playerX,
				20,
				selected2,
				status
			);
			win.draw(player1Name_inputBox_obj);
			win.draw(player1Name_inputBox_text);
			win.draw(player1Name_inputBox_label);


			// draw input box for player 2's name -------------------
			static sf::Text
				player2Name_inputBox_label(fonts::minecraft),
				player2Name_inputBox_text(fonts::minecraft);
			static sf::RectangleShape player2Name_inputBox_obj;

			// - draw input box
			setupInputBox(
				init,
				win,
				player2Name_inputBox_obj,
				player2Name_inputBox_label,
				player2Name_inputBox_text,
				{
					{615, 350}, 
					{485, 50}
				},
				5,
				{sf::Color(250, 222, 132), sf::Color(2, 46, 117)},
				{sf::Color(82, 63, 2), sf::Color(130, 178, 255)},
				{sf::Color(255, 0, 0), sf::Color(255, 0, 0)},
				fonts::minecraft,
				{ L"O player", L"Người chơi O" },
				25,
				{sf::Color::Black, sf::Color::White},
				{sf::Color::White, sf::Color::Black},
				5,
				event,
				ngd::playerO,
				20,
				selected3,
				status
			);
			win.draw(player2Name_inputBox_obj);
			win.draw(player2Name_inputBox_text);
			win.draw(player2Name_inputBox_label);
		};

		void firstTurnOption(sf::RenderWindow& win, std::optional<sf::Event> event, bool& init){

			static sf::Vector2f pos = {100, 475};
			static sf::Vector2f size;
			static sf::FloatRect XO_char_bounds;

			auto drawTitle = [&win](sf::Vector2f& pos, sf::Vector2f& size){
				static sf::Text label_text(fonts::minecraft);
				static std::wstring label_strings[2] = {
					L"First turn:",
					L"Luợt chơi đầu:"
				};
				label_text.setCharacterSize(25);
				static sf::Color
					fill_color[2] = {sf::Color::Black, sf::Color::White},
					outline_color[2] = {sf::Color::White, sf::Color::Black};

				label_text.setString(label_strings[globalConfig::language]);
				label_text.setOutlineThickness(5);
				label_text.setPosition(pos);

				label_text.setFillColor(fill_color[globalConfig::dark_mode]);
				label_text.setOutlineColor(outline_color[globalConfig::dark_mode]);

				size = label_text.getGlobalBounds().size;

				win.draw(label_text);
			};

			auto drawXO = [&win, &event](
				sf::Vector2f& pos,
				sf::Vector2f& size,
				sf::FloatRect& bounds
			){
				static sf::Text XO_char[2] = {
					sf::Text(fonts::mono_signature, "X"),
					sf::Text(fonts::mono_signature, "O")
				};

				namespace ngd = newGameData;

				XO_char[ngd::first_turn].setCharacterSize(50);

				sf::Color fill_colors[2] = {
					sf::Color::Red, sf::Color::Green
				};

				sf::Color outline_colors[2] = {
					sf::Color::Black, sf::Color::White
				};

				XO_char[ngd::first_turn].setOutlineThickness(6);
				XO_char[ngd::first_turn].setFillColor(fill_colors[ngd::first_turn]);
				XO_char[ngd::first_turn].setOutlineColor(outline_colors[globalConfig::dark_mode]);

				XO_char[ngd::first_turn].setOrigin({
					0,
					XO_char[ngd::first_turn].getLocalBounds().size.y / 2.f
				});

				XO_char[ngd::first_turn].setPosition({
					350,
					pos.y + size.y / 2.f
				});

				bounds = XO_char[ngd::first_turn].getGlobalBounds();

				win.draw(XO_char[ngd::first_turn]);

			};
			
			auto drawHoveringEffect = [&win, &event](sf::FloatRect& bounds){
				static sf::RectangleShape rect_shape;
				rect_shape.setPosition(bounds.position);
				rect_shape.setSize(bounds.size);
				sf::Color fill_colors[2] = {
					sf::Color(255, 145, 145, 150),
					sf::Color(1, 1, 82, 150)
				};

				namespace ngd = newGameData;

				static bool ignore_repeated_events = false;
				if(bounds.contains(win.mapPixelToCoords(sf::Mouse::getPosition(win)))){
					rect_shape.setFillColor(fill_colors[globalConfig::dark_mode]);
					if(event->is<sf::Event::MouseButtonPressed> ()){
						if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)
							&& !ignore_repeated_events){
							ngd::first_turn = (ngd::first_turn + 1) % 2;
						};
						ignore_repeated_events = true;
					} else{
						ignore_repeated_events = false;
					};
				} else{
					rect_shape.setFillColor(sf::Color::Transparent);
				};

				win.draw(rect_shape);
			};
			
			drawTitle(pos, size);

			drawHoveringEffect(XO_char_bounds);

			drawXO(pos, size, XO_char_bounds);

		};
		
		void startGameButton(sf::RenderWindow& win, std::optional<sf::Event> event, bool& init){
			
			namespace ngd = newGameData;
			
			sf::Text text_obj(fonts::minecraft);
			sf::Sprite button_obj(textures::text_button[0]);
			std::wstring text_string[2] = {
				L"Play",
				L"Chơi"
			};
			sf::Color text_fill_color[2] = {
				sf::Color::White, sf::Color::Black
			};
			sf::Color text_outline_color[2] = {
				sf::Color::Black, sf::Color::White
			};
			bool is_event = setupTextButton(
				init,
				win,
				button_obj,
				text_obj,
				textures::text_button,
				{{450, 650}, {300, 100}},
				fonts::minecraft,
				text_string,
				30,
				text_fill_color,
				text_outline_color,
				5,
				event
			);
			if(is_event){
				ngd::is_new_game = true;
				internalStats::status = 2;
			};
			win.draw(button_obj);
			win.draw(text_obj);
		};

};

	void multiplayerOptions(sf::RenderWindow& win, std::optional<sf::Event> event, bool& init){

		_multiplayerOptions::title(win, event, init);

		_multiplayerOptions::inputBoxes(win, event, init);

		_multiplayerOptions::firstTurnOption(win, event, init);
		
		_multiplayerOptions::startGameButton(win, event, init);

	};
	
	void stageTwo(sf::RenderWindow& win, std::optional<sf::Event> event, bool& init){
		
		static char selected = 0;
		// 0 = nothing
		// 1 = singleplayer
		// 2 = multiplayer
		
		singleplayerButton(win, event, init, selected);
		multiplayerButton(win, event, init, selected);

		if(selected == 1){
			newGameData::is_multiplayer = false;
			singleplayerOptions(win, event, init);
		} else if(selected == 2){
			newGameData::is_multiplayer = true;
			multiplayerOptions(win, event, init);
		};
	};

};

void drawForEachLoop(sf::RenderWindow& win, std::optional<sf::Event>& event, bool init){

	win.setSize({
		globalConfig::win_width,
		globalConfig::win_height
	});

	win.clear();

	draw::background(win);
	draw::goBackButton(win, event, init);

	if(internalStats::stage == 0){

		draw::stageZero(win, event, init);

	} else if(internalStats::stage == 1){

		draw::stageOne(win, event, init);

	} else if(internalStats::stage == 2){

		draw::stageTwo(win, event, init);

	};

	win.display();

};

// return true if re-init is needed
bool loopPrepareScreen(sf::RenderWindow& win, bool& init){
	
	while(win.isOpen()){

		if(globalConfig::current_win != 2
			|| internalStats::status != 0) return false;
		
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

gameDataPackage drawPrepareScreen(sf::RenderWindow& win){	//generate_random_saves();
	internalStats::status = 0;
	bool start = true;
	while(start){
		start = loopPrepareScreen(win, start);
	};
	if(internalStats::status != 0){
		globalConfig::current_win = 3;
	};
	internalStats::stage = 0;
	auto package = newGameData::convertToPackage();
	newGameData::clear();
	return package;
};

#endif