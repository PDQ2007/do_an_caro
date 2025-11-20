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

	std::filesystem::path load_game_from;
	
	std::vector<Saves_data> list_of_saves;

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
		std::cout << "new game clicked\n";
	};

	void goBackClicked(){		

		if(internalStats::stage > 0) internalStats::stage = 0;
		else{
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

		for(unsigned int i = START; i < min(START + 7, LIST.size()); ++i){
			if(gameName(win, event, i, LIST[i].name, LIST[i].is_multiplayer, init)){
				internalStats::load_game_from = LIST[i].path;
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

void drawPrepareScreen(
	sf::RenderWindow& win, 
	std::filesystem::path& load_game_from, 
	bool& is_new_game
){
	//generate_random_saves();
	internalStats::status = 0;
	bool start = true;
	while(start){
		start = loopPrepareScreen(win, start);
	};
	if(internalStats::status == 1){
		load_game_from = internalStats::load_game_from;
		is_new_game = false;
		globalConfig::current_win = 3;
	} else if(internalStats::status == 2){
		is_new_game = true;
		globalConfig::current_win = 3;
	};
};

#endif