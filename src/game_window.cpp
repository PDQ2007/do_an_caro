#ifndef GAME_WINDOW_CPP
#define GAME_WINDOW_CPP

#include "config.h"

namespace fonts{
	sf::Font minecraft("resources/fonts/minecraft-f2d-v1-42.ttf");
};

namespace textures{

	sf::Texture
		background[2] = {
			sf::Texture("resources/game_screen/light/background.png"),
			sf::Texture("resources/game_screen/dark/background.png")
		},
		undo_button[6] = {
			sf::Texture("resources/game_screen/light/undo_button.png"),
			sf::Texture("resources/game_screen/light/hovered_undo_button.png"),
			sf::Texture("resources/game_screen/light/clicked_undo_button.png"),
			sf::Texture("resources/game_screen/dark/undo_button.png"),
			sf::Texture("resources/game_screen/dark/hovered_undo_button.png"),
			sf::Texture("resources/game_screen/dark/clicked_undo_button.png")
		},
		light_dark_button[6] = {
			sf::Texture("resources/game_screen/light/switch_to_dark.png"),
			sf::Texture("resources/game_screen/light/hovered_switch_to_dark.png"),
			sf::Texture("resources/game_screen/light/clicked_switch_to_dark.png"),
			sf::Texture("resources/game_screen/dark/switch_to_light.png"),
			sf::Texture("resources/game_screen/dark/hovered_switch_to_light.png"),
			sf::Texture("resources/game_screen/dark/clicked_switch_to_light.png")
		},
		settings_button[6] = {
			sf::Texture("resources/game_screen/light/settings_button.png"),
			sf::Texture("resources/game_screen/light/hovered_settings_button.png"),
			sf::Texture("resources/game_screen/light/clicked_settings_button.png"),
			sf::Texture("resources/game_screen/dark/settings_button.png"),
			sf::Texture("resources/game_screen/dark/hovered_settings_button.png"),
			sf::Texture("resources/game_screen/dark/clicked_settings_button.png")
		},
		exit_button[6] = {
			sf::Texture("resources/game_screen/light/exit_button.png"),
			sf::Texture("resources/game_screen/light/hovered_exit_button.png"),
			sf::Texture("resources/game_screen/light/clicked_exit_button.png"),
			sf::Texture("resources/game_screen/dark/exit_button.png"),
			sf::Texture("resources/game_screen/dark/hovered_exit_button.png"),
			sf::Texture("resources/game_screen/dark/clicked_exit_button.png")
		},
		caro_table[2] = {
			sf::Texture("resources/game_screen/light/caro_table.png"),
			sf::Texture("resources/game_screen/dark/caro_table.png"),
		},
		xo_character[4] = {
			sf::Texture("resources/game_screen/light/X.png"),
			sf::Texture("resources/game_screen/light/O.png"),
			sf::Texture("resources/game_screen/dark/X.png"),
			sf::Texture("resources/game_screen/dark/O.png")
		},
		most_recent_move[2] = {
			sf::Texture("resources/game_screen/light/the_most_recent_move.png"),
			sf::Texture("resources/game_screen/dark/the_most_recent_move.png")
		},
		blank_button[6] = {
			sf::Texture("resources/menu_screen/light/normal_button.png"),
			sf::Texture("resources/menu_screen/light/hovered_button.png"),
			sf::Texture("resources/menu_screen/light/clicked_button.png"),
			sf::Texture("resources/menu_screen/dark/normal_button.png"),
			sf::Texture("resources/menu_screen/dark/hovered_button.png"),
			sf::Texture("resources/menu_screen/dark/clicked_button.png")
		};
};

namespace gameStats{
	std::wstring is_load_game_success; //"success" for success, otherwise store the content of the error.
	gameDataPackage saveInfo;
	unsigned int x_score, o_score;
	std::vector<sf::Vector2i> moves;
	std::vector<std::vector<short> > cells;

	enum CURRENT_STATE_ENUM{
		PLAYING = 0,
		SAVING = 1,
		WINNING = 2,
		DRAW = 3
	};
	CURRENT_STATE_ENUM current_stage = PLAYING;
	// 0 = playing
	// 1 = saving (when user prepares to exit, asking to save the game)
	// 2 = winning (when X or O won, the game stop)
	// 3 = draw (when nobody can win)

	CURRENT_STATE_ENUM VERY_previous_stage = PLAYING;

};

namespace events{

	void decrease_one_by_every(int miliseconds, int& value, int& confirm_timer){
		while(value > 0){
			std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
			--value;
		};
		confirm_timer = 8;
		while(confirm_timer > 0){
			std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
			--confirm_timer;
		};
		return;
	};

	void undoMove(){
		if(gameStats::moves.size() == 0) return;
		auto& last_move = gameStats::moves[gameStats::moves.size()-1];
		gameStats::cells[last_move.x][last_move.y] = 0;
		gameStats::moves.pop_back();
	};

	void lightDark(){
		globalConfig::dark_mode = (globalConfig::dark_mode + 1) % 2;
	};

	void settingsButton(){
		globalConfig::current_win = 4;
	};

	void exitButton(){
		static gameStats::CURRENT_STATE_ENUM previous_state = gameStats::current_stage;
		if(gameStats::current_stage == gameStats::SAVING) gameStats::current_stage = previous_state;
		else{
			gameStats::CURRENT_STATE_ENUM previous_state = gameStats::current_stage;
			gameStats::current_stage = gameStats::SAVING;
		};
	};

	void addAMove(const sf::Vector2i& move){
		if(gameStats::cells[move.x][move.y] != 0) return;
		gameStats::moves.push_back(move);
		gameStats::cells[move.x][move.y] = (gameStats::moves.size() + 1 + gameStats::saveInfo.first_turn) % 2 + 1;
	};

	void Continue_next_round(){
		for(auto& i: gameStats::cells){
			i.clear();
			i.resize(16,0);
		};
		gameStats::moves.clear();
		gameStats::current_stage = gameStats::PLAYING;
	};

	bool buttonSaveGameClicked(std::wstring error_content[2]){
		if(gameStats::saveInfo.is_new_game){
			for(const auto& entry: std::filesystem::directory_iterator("saves")){
				if(entry.path().stem().string() == gameStats::saveInfo.save_name){
					std::string t_str = entry.path().stem().string();

#define toWstring(st) std::wstring(st.begin(), st.end())

					error_content[0] =	L"Name \"" + toWstring(t_str) + L"\" existed.";
					error_content[1] =	L"Tên \"" + toWstring(t_str) + L"\" đã tồn tại.";

#undef toWstring

					return false;
				};
				saveGame(
					gameStats::saveInfo,
					gameStats::x_score,
					gameStats::o_score,
					gameStats::moves
				);
			};
		} else{
			saveGame(
				gameStats::saveInfo,
				gameStats::x_score,
				gameStats::o_score,
				gameStats::moves
			);
		};
		globalConfig::current_win = 1;
		return true;
	};

};

namespace draw{

	void background(sf::RenderWindow& win){
		static sf::Sprite background(textures::background[globalConfig::dark_mode]);
		background.setTexture(textures::background[globalConfig::dark_mode]);
		win.draw(background);
	};

	void undoButton(sf::RenderWindow& win, std::optional<sf::Event>& event){
		static sf::Sprite undo_button(textures::undo_button[0]);
		bool is_event = setupIconButton(
			true,
			win,
			undo_button,
			textures::undo_button,
			{{850, 690}, {100, 100}},
			event
		);
		if(is_event && gameStats::current_stage == gameStats::PLAYING){
			events::undoMove();
		};
		win.draw(undo_button);
	};

	void lightDarkButton(sf::RenderWindow& win, std::optional<sf::Event>& event){
		static sf::Sprite light_dark_button(textures::undo_button[0]);
		bool is_event = setupIconButton(
			true,
			win,
			light_dark_button,
			textures::light_dark_button,
			{{960, 690}, {100, 100}},
			event
		);
		if(gameStats::current_stage != 1 && is_event){
			events::lightDark();
		};
		win.draw(light_dark_button);
	};

	void settingsButton(sf::RenderWindow& win, std::optional<sf::Event>& event){
		static sf::Sprite settings_button(textures::undo_button[0]);
		bool is_event = setupIconButton(
			true,
			win,
			settings_button,
			textures::settings_button,
			{{1070, 690}, {100, 100}},
			event
		);
		if(gameStats::current_stage != 1 && is_event){
			events::settingsButton();
		};
		win.draw(settings_button);
	};

	void exitButton(sf::RenderWindow& win, std::optional<sf::Event>& event){
		static sf::Sprite exit_button(textures::undo_button[0]);
		bool is_event = setupIconButton(
			true,
			win,
			exit_button,
			textures::exit_button,
			{{1100, 50}, {50, 50}},
			event
		);
		if(is_event){
			events::exitButton();
		};
		win.draw(exit_button);
	};

	namespace insideCaroTableView{

#define width globalConfig::win_width
#define height globalConfig::win_height

		sf::View view(
			{height / 2.f, height / 2.f},
			{height * 1.f, height * 1.f}
		);

		void setViewProperties(){
			view.setViewport({
				{10.f / width, 10.f / height},
				{780.f / width, 780.f / height}
			});
		};

		void tableBackground(sf::RenderWindow& win){
			static sf::Sprite background(textures::caro_table[0]);
			background.setTexture(textures::caro_table[globalConfig::dark_mode]);
			win.draw(background);
		};

		void moves(sf::RenderWindow& win){
			static sf::Sprite xo_char[2] = {
				sf::Sprite(textures::xo_character[0]),
				sf::Sprite(textures::xo_character[0])
			};
			xo_char[0].setTexture(textures::xo_character[2 * globalConfig::dark_mode], true);
			xo_char[1].setTexture(textures::xo_character[2 * globalConfig::dark_mode + 1], true);
			xo_char[0].setScale({
				40.f / xo_char[0].getLocalBounds().size.x,
				40.f / xo_char[0].getLocalBounds().size.y,
			});
			xo_char[1].setScale({
				40.f / xo_char[1].getLocalBounds().size.x,
				40.f / xo_char[1].getLocalBounds().size.y,
			});
			bool curr_char = gameStats::saveInfo.first_turn;
			for(auto& move: gameStats::moves){
				xo_char[curr_char].setPosition({
					static_cast<float> (50 * move.x + 5), 
					static_cast<float> (50 * move.y + 5)
				});
				win.draw(xo_char[curr_char]);
				curr_char = (curr_char + 1) % 2;
			};
		};

		void lastMoveIndication(sf::RenderWindow& win){
			static sf::Sprite most_recent_move(textures::most_recent_move[0]);
			if(gameStats::moves.size() == 0) return;
			most_recent_move.setTexture(textures::most_recent_move[globalConfig::dark_mode], true);
			most_recent_move.setScale({
				50.f / most_recent_move.getLocalBounds().size.x,
				50.f / most_recent_move.getLocalBounds().size.y
			});
			auto& move_pos = gameStats::moves[gameStats::moves.size()-1];
			most_recent_move.setPosition({
				move_pos.x * 50.f - 1,
				move_pos.y * 50.f - 1
			});
			win.draw(most_recent_move);
		};

		// draw hover effect + handle user's clicks
		void hoverEffect(sf::RenderWindow& win, std::optional<sf::Event>& event){
			static sf::RectangleShape fill_rect({50.f, 50.f});

#define mouse_pos win.mapPixelToCoords(sf::Mouse::getPosition(win))

			// check if the user's mouse cursor is inside the caro table
			auto insideTable = [&win]() -> bool {
				if(!(0 <= std::min(mouse_pos.x, mouse_pos.y)
					&& std::max(mouse_pos.x, mouse_pos.y) < 800))
				{
					return false;
				};
				return true;
			};

			// if the user's mouse cursor is inside the table, then...

			static bool ignore_repeated_events = false;
			if(insideTable()){

				// draw the hovering effect onto the cell that contains the mouse cursor.
				fill_rect.setPosition({
					std::floor(mouse_pos.x / 50) * 50 - 3,
					std::floor(mouse_pos.y / 50) * 50 - 3
				});
				static sf::Color fill_color[2] = {
					sf::Color(0, 0, 0, 150),
					sf::Color(255, 255, 255, 150)
				};
				fill_rect.setFillColor(fill_color[globalConfig::dark_mode]);
				win.draw(fill_rect);

				// record a move when a left click is detected
				
				if(gameStats::current_stage == gameStats::PLAYING
					&& event 
					&& event->is<sf::Event::MouseButtonPressed> () 
					&& sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
				{
					if(!ignore_repeated_events){
						events::addAMove({
							static_cast<short> (mouse_pos.x / 50),
							static_cast<short> (mouse_pos.y / 50)
						});
					};
				};
			};
#undef mouse_pos
		};

#define APPEAR_TIME 1000
		// draw + CHECK AND PROCESS WIN (add score to X's player / O's player)
		void drawWinningIndication(sf::RenderWindow& win){
			static std::vector<sf::Vector2i> result;
			static bool ignore_repeated_counts = false;
			bool is_win = false, is_draw = false;
			static int delay = 0;
			static sf::RectangleShape rect_shape({47.f, 47.f});
			if(gameStats::moves.size() >= 5){
				result = checkForWin(gameStats::moves[gameStats::moves.size()-1], gameStats::cells, is_win);
				if(!is_win){
					is_draw = checkForDraw(gameStats::moves);
				};
			};
			if(is_win){

				if(!ignore_repeated_counts){
					if((gameStats::moves.size() + 1 + gameStats::saveInfo.first_turn) % 2){
						++gameStats::x_score;
					} else{
						++gameStats::o_score;
					};
				};
				ignore_repeated_counts = true;
				gameStats::current_stage = gameStats::WINNING;
				if(delay <= APPEAR_TIME){
					static sf::Color colors[2] = {
						sf::Color(252, 186, 3),
						sf::Color(0, 61, 204)
					};
					rect_shape.setFillColor(sf::Color::Transparent);
					rect_shape.setOutlineColor(colors[globalConfig::dark_mode]);
					rect_shape.setOutlineThickness(4);
					for(auto i: result){
						rect_shape.setPosition({i.x * 50.f, i.y * 50.f});
						win.draw(rect_shape);
					};
				};
				delay = (delay + 1) % (APPEAR_TIME * 2);
			} else if(is_draw){
				gameStats::current_stage = gameStats::DRAW;
				ignore_repeated_counts = false;
			} else{
				ignore_repeated_counts = false;
				if(gameStats::current_stage != gameStats::SAVING) gameStats::current_stage = gameStats::PLAYING;
			};

#undef APPEAR_TIME
		};

		void drawAll(sf::RenderWindow& win, std::optional<sf::Event>& event){

			setViewProperties();

			win.setView(view);

			tableBackground(win);
			hoverEffect(win, event); // hover effect + handle user's click
			moves(win);
			drawWinningIndication(win);
			lastMoveIndication(win);

			win.setView(win.getDefaultView());

		};

#undef width
#undef height

	};

	void caroTable(sf::RenderWindow& win, std::optional<sf::Event>& event){
		insideCaroTableView::drawAll(win, event);
	};

#define APPEAR_TIME 1000
	void scoreboard(sf::RenderWindow& win, std::optional<sf::Event>& event){

		//draw board's background
		auto board_background = [&win](){
			static sf::RectangleShape board_bg({350.f, 250.f});
			static sf::Color fill_colors[2] = {
				sf::Color(255, 255, 255, 150),
				sf::Color(0, 0, 0, 150)
			};
			board_bg.setFillColor(fill_colors[globalConfig::dark_mode]);
			board_bg.setPosition({825, 300});
			win.draw(board_bg);
		};

		//draw board's title (if X or O won, show who won instead)
		auto board_title = [&win](){
			static sf::Text title(fonts::minecraft);


#define color(x, LIGHT,DARK) (x) ? LIGHT : DARK

			static std::wstring strings[8] = {
				L"Scoreboard",
				L"Bảng điểm",
				L"X won!",
				L"X đã thắng!",
				L"O won!",
				L"O đã thắng!",
				L"Match is Draw!",
				L"Ván chơi hòa!"
			};
			title.setCharacterSize(30);
		
			title.setOutlineThickness(5);

			static int count = 0;
		
			if(gameStats::current_stage == gameStats::PLAYING){
				count = 0;
				title.setString(strings[globalConfig::language]);
				title.setFillColor(color(!globalConfig::dark_mode, sf::Color::White, sf::Color::Black));
				title.setOutlineColor(color(!globalConfig::dark_mode, sf::Color::Black, sf::Color::White));
			} else if(gameStats::current_stage == gameStats::WINNING){
				count = (count + 1) % (2 * APPEAR_TIME);
				using namespace gameStats;
				title.setString(strings[2 + ((moves.size() + 1 + saveInfo.first_turn) % 2) * 2 + globalConfig::language]);
				title.setFillColor(color((moves.size() + saveInfo.first_turn) % 2, sf::Color::Red, sf::Color::Green));
				if(count <= APPEAR_TIME){
					title.setOutlineColor(color(!globalConfig::dark_mode, sf::Color::Black, sf::Color::White));
				} else{
					title.setOutlineColor(sf::Color::Transparent);
				};
			} else if(gameStats::current_stage == gameStats::DRAW){
				count = (count + 1) % (2 * APPEAR_TIME);
				title.setString(strings[6 + globalConfig::language]);
				title.setFillColor(color(!globalConfig::dark_mode, sf::Color::White, sf::Color::Black));
				if(count <= APPEAR_TIME){
					title.setOutlineColor(color(!globalConfig::dark_mode, sf::Color::Black, sf::Color::White));
				} else{
					title.setOutlineColor(sf::Color::Transparent);
				};
			};

#undef color

			title.setOrigin({
				title.getLocalBounds().size.x / 2,
				title.getLocalBounds().size.y / 2
			});
			title.setPosition({1000, 350});
			win.draw(title);
		};

		//draw XO's score
		auto XO_score = [&win](const bool& XO, const sf::Vector2f& pos){ // XO = 0 => draw for X, XO = 1 => draw for O

			//convert integer to string
			auto unsignedintToString = [](int n) -> std::string {
				if(n == 0) return "0";
				std::string res = "";
				while(n > 0){
					res = (char)(n % 10 + (int)'0') + res;
					n /= 10;
				};
				return res;
			};

			//draw icon
			static sf::Sprite XO_icon(textures::xo_character[XO]);
			XO_icon.setTexture(textures::xo_character[XO + 2 * globalConfig::dark_mode], true);
			XO_icon.setScale({
				85.f / XO_icon.getLocalBounds().size.x,
				85.f / XO_icon.getLocalBounds().size.y
			});
			XO_icon.setOrigin({
				0.5f * XO_icon.getLocalBounds().size.x,
				0.5f * XO_icon.getLocalBounds().size.y
			});
			XO_icon.setPosition(pos);
			win.draw(XO_icon);

#define color(x, LIGHT, DARK) (x) ? LIGHT : DARK
#define scoreValue ((!XO) ? gameStats::x_score : gameStats::o_score)

			//draw score value
			static sf::Text XO_score(fonts::minecraft);
			XO_score.setCharacterSize(40);
			XO_score.setFillColor(color(!globalConfig::dark_mode, sf::Color::Black, sf::Color::White));
			XO_score.setOutlineThickness(3);
			XO_score.setOutlineColor(color(globalConfig::dark_mode, sf::Color::Black, sf::Color::White));
			XO_score.setString(unsignedintToString(scoreValue));
			XO_score.setOrigin({
				0.5f * XO_score.getLocalBounds().size.x,
				0.5f * XO_score.getLocalBounds().size.y
			});
			XO_score.setPosition({
				pos.x + 100,
				pos.y
			});
			win.draw(XO_score);

#undef color
#undef scoreValue
		};


		board_background();
		board_title();
		XO_score(0, {825 + 350 * (1.f/3), 410}); //draw X
		XO_score(1, {825 + 350 * (1.f/3), 500}); //draw O

#undef APPEAR_TIME
	};

#define BUTTON_FLOATRECT {{830, 560}, {330, 110}}
	void continueButton(sf::RenderWindow& win, std::optional<sf::Event>& event){
		

		static sf::FloatRect bounds = BUTTON_FLOATRECT;

#undef BUTTON_FLOATRECT

		static sf::Sprite button_obj(textures::blank_button[0]);
		static sf::Text text_obj(fonts::minecraft);

		std::wstring text_string[2] = {
			L"Continue",
			L"Tiếp tục"
		};

		sf::Color text_fill_color[2] = {
			sf::Color::Black,
			sf::Color::White
		};
		sf::Color text_outline_color[2] = {
			sf::Color::White,
			sf::Color::Black
		};
		
		bool is_event = setupTextButton(
			true,
			win,
			button_obj,
			text_obj,
			textures::blank_button,
			bounds,
			fonts::minecraft,
			text_string,
			30,
			text_fill_color,
			text_outline_color,
			4,
			event
		);

		if(is_event){
			events::Continue_next_round();
		};

		win.draw(button_obj);
		win.draw(text_obj);

		
	};

	namespace savePopupComponents{

#define BG_SIZE sf::Vector2f(900, 400)
		void backgroundAndTitle(sf::RenderWindow& win, std::optional<sf::Event>& event){
			
			// draw the dark background cover
			static sf::RectangleShape cover_obj({
				static_cast<float> (globalConfig::win_width), 
				static_cast<float> (globalConfig::win_height)
			});
			cover_obj.setFillColor(sf::Color(0, 0, 0, 150));
			win.draw(cover_obj);

			// draw the background box object
			static sf::RectangleShape rect_obj(BG_SIZE);
			static sf::Color fill_rect_colors[2] = {
				sf::Color(0, 0, 0, 150),
				sf::Color(255, 255, 255, 150)
			};
			rect_obj.setFillColor(fill_rect_colors[globalConfig::dark_mode]);
			rect_obj.setOrigin({
				rect_obj.getLocalBounds().size.x / 2.f,
				rect_obj.getLocalBounds().size.y / 2.f
			});
			rect_obj.setPosition({globalConfig::win_width / 2.f, globalConfig::win_height / 2.f});
			win.draw(rect_obj);

			// draw title text
#define colors(x,y) (globalConfig::dark_mode) ? (y) : (x)
#define strings(x,y) (globalConfig::language) ? (y) : (x)
			static sf::Text text_obj(fonts::minecraft);
			text_obj.setCharacterSize(30);
			text_obj.setFillColor(colors(sf::Color::Black, sf::Color::White));
			text_obj.setOutlineColor(colors(sf::Color::White, sf::Color::Black));
			text_obj.setOutlineThickness(4);
			text_obj.setString(strings(L"Save game as...", L"Lưu ván thành..."));
			text_obj.setOrigin({
				text_obj.getLocalBounds().size.x / 2.f,
				text_obj.getLocalBounds().size.y / 2.f
			});
			text_obj.setPosition({rect_obj.getPosition().x, 400 - BG_SIZE.y / 2 + 50});
			win.draw(text_obj);
#undef colors
#undef strings
#undef BG_SIZE

		};

		void input_box(sf::RenderWindow& win, std::optional<sf::Event>& event, int warnings[]){
			
			auto warningProcess = [&warnings](const wchar_t& status){
				if(status == L'A'){
					warnings[0] = -1;
				} else if(status == L'B'){
					warnings[1] = -1;
				} else if(status != L'\0' && status != '0'){
					warnings[2] = -1;
					warnings[3] = static_cast<int>(status);
				};
			};
			
			static sf::RectangleShape box_obj;
			static sf::Text label_text_obj(fonts::minecraft), input_text_obj(fonts::minecraft);
			static bool is_selected;
			static wchar_t status;

			status = L'\0';
			setupInputBox(
				true,
				win,
				box_obj,
				label_text_obj,
				input_text_obj,
				{{175, 370}, {850, 60}},
				5,
				{sf::Color(255, 223, 117), sf::Color(1, 1, 117)},
				{sf::Color(2, 2, 189), sf::Color(209, 161, 0)},
				{sf::Color(209, 0, 0), sf::Color(209, 0, 0)},
				fonts::minecraft,
				{L"Save name", L"Tên ván chơi"},
				30,
				{sf::Color::Black, sf::Color::White},
				{sf::Color::White, sf::Color::Black},
				4,
				event,
				gameStats::saveInfo.save_name,
				38,
				is_selected,
				status
			);
			warningProcess(status);
			win.draw(box_obj);
			win.draw(label_text_obj);
			win.draw(input_text_obj);
		};


#define WARNING_SHOWTIME 10000
#define FLASHING_TIME 2000
		void showInputWarnings(sf::RenderWindow& win, std::optional<sf::Event>& event, int warnings[]){
			

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

			if(warnings[2] == -1){
				warning_strings[2][0][11] = static_cast<wchar_t>(warnings[3]);
				warning_strings[2][1][7] = static_cast<wchar_t>(warnings[3]);
			};

			int idx[3] = {0, 1, 2};

#define DECREASE(x) x = (x > 0) ? (--x) : 0

			for(int i = 0; i < 2; ++i){
				for(int j = 1; j < 3; ++j){
					if(warnings[i] == -1) warnings[i] = WARNING_SHOWTIME + 1;
					if(warnings[j] == -1) warnings[j] = WARNING_SHOWTIME + 1;
					if(warnings[idx[i]] < warnings[idx[j]]){
						std::swap(idx[i], idx[j]);
					};
				};
			};

			DECREASE(warnings[0]);
			DECREASE(warnings[1]);
			DECREASE(warnings[2]);


#undef DECREASE

			static sf::Text warning_text_obj(fonts::minecraft);
			static std::vector<sf::Color> 
				fill_colors = {
					sf::Color(255, 61, 61),
					sf::Color(255, 61, 61)
				},
				outline_colors = {
					sf::Color::White,
					sf::Color::White
				};

			static unsigned int count_flashing = 0;

			for(int i = 0; i < 3; ++i){
				
				if(warnings[idx[0]] == 0){
					count_flashing = 0;
				};

				count_flashing = (count_flashing + 1) % (int)((3.0/2) * (float)FLASHING_TIME);

				int outline_thickness;
				if(count_flashing < FLASHING_TIME){
					outline_thickness = 4;
				} else{
					outline_thickness = 0;
				};
				if(warnings[idx[i]] > 0){
					setUpTextObj(
						true,
						win,
						warning_text_obj,
						fill_colors,
						outline_colors,
						outline_thickness,
						20,
						{0.5, 0.5},
						{600, static_cast<float> (650 + 45 * i)},
						warning_strings[idx[i]]
					);
					win.draw(warning_text_obj);
				} else{
					break;
				};
			};

#undef WARNING_SHOWTIME
#undef FLASHING_TIME

		};

#define WARNING_SHOWTIME 10000
#define FLASHING_TIME 2000
		void saveButton(sf::RenderWindow& win, std::optional<sf::Event>& event){
			
			bool savenameExistedWarning = false;
			static std::wstring error_content[2];

			auto _saveButton = [&win, &event, &savenameExistedWarning](std::wstring error_content[2]){
				static sf::Sprite button_obj(textures::blank_button[0]);
				static sf::Text text_obj(fonts::minecraft);
				static std::wstring text_strings[2] = {
					L"Save",
					L"Lưu"
				};
				static sf::Color
					text_fill_colors[2] = {
						sf::Color::Black,
						sf::Color::White
					},
					text_outline_colors[2] = {
						sf::Color::White,
						sf::Color::Black
					};
				bool isEvent = setupTextButton(
					true,
					win,
					button_obj,
					text_obj,
					textures::blank_button,
					{{625, 450}, {300, 100}},
					fonts::minecraft,
					text_strings,
					30,
					text_fill_colors,
					text_outline_colors,
					4,
					event
				);
				win.draw(button_obj);
				win.draw(text_obj);


				static bool ignore_repeated_events = false;
				if(isEvent && !gameStats::saveInfo.save_name.empty()){
					if(!ignore_repeated_events){
						savenameExistedWarning = !events::buttonSaveGameClicked(error_content);
						ignore_repeated_events = true;
					};
				} else{
					ignore_repeated_events = false;
				};
			};

			auto _showWarnings = [&win, &savenameExistedWarning](std::wstring error_content[2]){
				static int
					warning_showTime = 0,
					flashing_time = 0;
				flashing_time = (flashing_time + 1) % (int)((3.0/2) * (float)FLASHING_TIME);
				if(savenameExistedWarning){
					warning_showTime = WARNING_SHOWTIME;
					flashing_time = 0;
				} else {
					warning_showTime = ((warning_showTime - 1) >= 0) ? (warning_showTime - 1) : (0);
				};
				if(warning_showTime > 0){
					static sf::Text warning_text_obj(fonts::minecraft);
					std::vector<sf::Color> 
						fill_colors = {
							sf::Color(255, 61, 61),
							sf::Color(255, 61, 61)
						},
						outline_colors = {
							sf::Color::White,
							sf::Color::White
						};
					int outline_thickness = 4;
					if(flashing_time < FLASHING_TIME) outline_thickness = 4;
					else outline_thickness = 0;
					setUpTextObj(
						true,
						win,
						warning_text_obj,
						fill_colors,
						outline_colors,
						outline_thickness,
						20,
						{0.5, 0.5},
						{600, 150},
						error_content
					);
					win.draw(warning_text_obj);
				};
#undef WARNING_SHOWTIME
			};

			_saveButton(error_content);
			_showWarnings(error_content);
		};

#define WARNING_SHOWTIME 10000
#define FLASHING_TIME 2000
		void dontSaveButton(sf::RenderWindow& win, std::optional<sf::Event>& event){
			static sf::Sprite button_obj(textures::blank_button[0]);
			static sf::Text text_obj(fonts::minecraft);
			static char click_times = 0;
			static int warning_showtime = 0, flashing_time = 0, seconds_timer = 0, confirm_timer = -1;

			if((gameStats::VERY_previous_stage != gameStats::current_stage) || (globalConfig::current_win != 3)){
				warning_showtime = 0;
				flashing_time = 0;
				seconds_timer = 0;
				confirm_timer = -1;
			};

			std::wstring text_strings[2] = {
				L"Don't save",
				L"Không lưu"
			};
			sf::Color
				text_fill_colors[2] = {sf::Color::White, sf::Color::Black},
				text_outline_colors[2] = {sf::Color::Black, sf::Color::White};
			if(click_times > 0){
				text_fill_colors[0] = sf::Color::Red;
				text_fill_colors[1] = sf::Color::Red;

#define cast(x,y) static_cast<x> (y)
				if(confirm_timer != -1){
					text_strings[0] += (std::wstring(L" (") + cast(wchar_t, confirm_timer + cast(int, L'0')) + std::wstring(L")"));
					text_strings[1] += (std::wstring(L" (") + cast(wchar_t, confirm_timer + cast(int, L'0')) + std::wstring(L")"));
				};
#undef cast
			};
			bool is_event = setupTextButton(
				true,
				win,
				button_obj,
				text_obj,
				textures::blank_button,
				{{275, 450}, {300, 100}},
				fonts::minecraft,
				text_strings,
				30,
				text_fill_colors,
				text_outline_colors,
				4,
				event
			);
			win.draw(button_obj);
			win.draw(text_obj);

			// process warning text
			if(confirm_timer == 0 || globalConfig::current_win != 3){
				click_times = 0;
				warning_showtime = 0;
				flashing_time = 0;
				confirm_timer = -1;
			};
			if(is_event && seconds_timer == 0){
				seconds_timer = 3;
				std::thread decrease_by_seconds(
					events::decrease_one_by_every, 
					1000, std::ref(seconds_timer), std::ref(confirm_timer)
				);
				decrease_by_seconds.detach();
				click_times = (click_times + 1 <= 2) ? (click_times + 1) : 0;
				warning_showtime = WARNING_SHOWTIME;
				flashing_time = 0;
			};
			if(click_times == 1){
				int outline_thickness;
				flashing_time = (flashing_time + 1) % (int)((3.0/2) * (float)(FLASHING_TIME));
				if(flashing_time < FLASHING_TIME) outline_thickness = 4;
				else outline_thickness = 0;
				static sf::Text warning_text_obj(fonts::minecraft);
				std::vector<sf::Color>
					text_fill_colors = {sf::Color::Red, sf::Color::Red},
					text_outline_colors = {sf::Color::White, sf::Color::White};
				std::wstring prompt_text_strings[2] = {
					L"Confirm that you are not saving the match. (A)",
					L"Xác nhận rằng bạn không lưu ván chơi. (A)"
				};
#define cast(x,y) static_cast<x> (y)
				prompt_text_strings[0][46] = cast(wchar_t, (seconds_timer + cast(int, L'0')));
				prompt_text_strings[1][39] = cast(wchar_t, (seconds_timer + cast(int, L'0')));
#undef cast
				setUpTextObj(
					true,
					win,
					warning_text_obj,
					text_fill_colors,
					text_outline_colors,
					outline_thickness,
					20,
					{0.5, 0.5},
					{600, 90},
					prompt_text_strings
				);
				win.draw(warning_text_obj);
			} else if(click_times > 1){
				click_times = 0;
				globalConfig::current_win = 1;
			};

#undef WARNING_SHOWTIME
#undef FLASHING_TIME
		};

		void drawAll(sf::RenderWindow& win, std::optional<sf::Event>& event){
			backgroundAndTitle(win, event);

			static int warnings[] = {0, 0, 0, 0};
			// '0' for no warning
			// otherwise store counting value (newest warnings have higher counting values)
			// respectively: [0] max-length warning; [1] empty input warning; [2] invalid character warning
			// the last element store the specific invalid character that user typed in (converting to int)

			input_box(win, event, warnings);

			showInputWarnings(win, event, warnings);

			saveButton(win, event);

			dontSaveButton(win, event);

		};

	};

	void savePopup(sf::RenderWindow& win, std::optional<sf::Event>& event){
		savePopupComponents::drawAll(win, event);
	};

};


void drawForEachLoop(sf::RenderWindow& win, std::optional<sf::Event>& event){

	static bool stage_changed = false;

	win.clear();

	draw::background(win);

	draw::undoButton(win, event);

	draw::lightDarkButton(win, event);

	draw::settingsButton(win, event);

	draw::caroTable(win, event);

	draw::scoreboard(win, event);

	if((gameStats::current_stage != gameStats::PLAYING && gameStats::current_stage != gameStats::SAVING)){
		draw::continueButton(win, event);
	};

	if(gameStats::current_stage == gameStats::SAVING){
		draw::savePopup(win, event);
	};

	draw::exitButton(win, event);

	win.display();

	if(gameStats::current_stage != gameStats::VERY_previous_stage && stage_changed == false){
		stage_changed = true;
	} else{
		stage_changed = false;
		gameStats::VERY_previous_stage = gameStats::current_stage;
	};

};

void loopGameScreen(sf::RenderWindow& win){

	while(win.isOpen()){

		if(globalConfig::current_win != 3) return;

		win.setSize({globalConfig::win_width, globalConfig::win_height});

		std::optional<sf::Event> event;

		while(event = win.pollEvent()){

			if(event->is<sf::Event::Closed> ()){
				globalConfig::current_win = 0;
			};

			drawForEachLoop(win, event);

		};

		drawForEachLoop(win, event);

	};

};

void drawGameScreen(
	sf::RenderWindow& win,
	bool start, // if user return from ingame settings screen, set this to false so all the moves remain without being cleared.
	gameDataPackage& package
){
	gameStats::saveInfo = package;

	//if the user opens the game for the first time, the game will be loaded accordingly.
	if(start){
		if(gameStats::saveInfo.is_new_game){
			gameStats::cells.resize(16, std::vector<short> (16, 0));
			gameStats::moves.clear();
			gameStats::x_score = 0;
			gameStats::o_score = 0;
			gameStats::current_stage = gameStats::PLAYING;
		} else{
			loadGame(
				gameStats::saveInfo,
				gameStats::x_score,
				gameStats::o_score,
				gameStats::moves,
				gameStats::cells,
				gameStats::is_load_game_success // if successful, return "success", else return the error's content
			);
		};
	};

	//check if user save the game in WINNING stage or DRAWING stage
	bool is_win = false;
	if(gameStats::moves.size() >= 5) checkForWin(gameStats::moves[gameStats::moves.size()-1], gameStats::cells, is_win);
	if(is_win){
		gameStats::current_stage = gameStats::WINNING;
	} else if(checkForDraw(gameStats::moves)){
		gameStats::current_stage = gameStats::DRAW;
	} else{
		gameStats::current_stage = gameStats::PLAYING;
	};

	//after loading the game, the gameloop start its job
	loopGameScreen(win);
};

#endif