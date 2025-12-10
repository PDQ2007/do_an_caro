#ifndef TEMPLATES_CPP
#define TEMPLATES_CPP

#include "config.h"

#define DARK (globalConfig::dark_mode * 3)

bool setupIconButton(
	bool init,
	sf::RenderWindow& win,
	sf::Sprite& button_obj,
	sf::Texture textures[6],
	sf::FloatRect bounds,
	std::optional<sf::Event>& event
){
	static bool return_val = false, change = false;
	static int status = -1;
	if(bounds.contains(win.mapPixelToCoords(sf::Mouse::getPosition(win)))){
		if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
			button_obj.setTexture(textures[DARK + 2]);
			if(event && event->is<sf::Event::MouseButtonPressed> ()){
				return_val = true;
			} else{
				return_val = false;
			};
			if(status != 2) change = true;
			else change = false;
			status = 2;
		} else{
			button_obj.setTexture(textures[DARK + 1]);
			return_val = false;
			if(status != 1) change = true;
			else change = false;
			status = 1;
		};
	} else{
		button_obj.setTexture(textures[DARK + 0]);
		return_val = false;
		if(status != 0) change = true;
		else change = false;
		status = 0;
	};
	if(change || init){
		button_obj.setScale({
			bounds.size.x / button_obj.getLocalBounds().size.x,
			bounds.size.y / button_obj.getLocalBounds().size.y
		});
		button_obj.setPosition(bounds.position);
	};
	return return_val;
};

#undef DARK

#define LANG globalConfig::language
#define DARK globalConfig::dark_mode
#define FL_RCT_POS button_obj.getGlobalBounds().position
#define FL_RCT_SZE button_obj.getGlobalBounds().size

bool setupTextButton(
	bool init,
	sf::RenderWindow& win,
	sf::Sprite& button_obj,
	sf::Text& text_obj,
	sf::Texture textures[6],
	sf::FloatRect bounds,
	sf::Font& text_font,
	std::wstring text_string[2],
	unsigned short text_size,
	sf::Color text_fill_color[2],
	sf::Color text_outline_color[2],
	unsigned short outline_thickness,
	std::optional<sf::Event>& event
){
	static bool return_val;
	return_val = setupIconButton(init, win, button_obj, textures, bounds, event);
	if(init){
		text_obj.setFont(text_font);
		text_obj.setString(text_string[LANG]);
		text_obj.setCharacterSize(text_size);
		text_obj.setFillColor(text_fill_color[DARK]);
		text_obj.setOutlineColor(text_outline_color[DARK]);
		text_obj.setOutlineThickness(outline_thickness);
		text_obj.setOrigin({
			text_obj.getLocalBounds().size.x / 2.f,
			text_obj.getLocalBounds().size.y / 2.f
		});
		text_obj.setPosition({
			FL_RCT_POS.x + FL_RCT_SZE.x / 2.f,
			FL_RCT_POS.y + FL_RCT_SZE.y / 2.f
		});
	};
	return return_val;
};

#undef DARK
#undef FL_RCT_POS
#undef FL_RCT_SZE

void handleInputBox(std::string& input_str, std::optional<sf::Event>& _, unsigned char max_length, wchar_t& status){
	if(!_){
		if(input_str.size() == 0){
			if(input_str.size() == 0) status = 'B'; //empty string warning
		};
		return;
	};
	if(input_str.size() == 0){
		if(input_str.size() == 0) status = 'B'; //empty string warning
	};
	auto event = _->getIf<sf::Event::TextEntered> ();
	if(event){
		if(('a' <= event->unicode && event->unicode <= 'z') ||
			('A' <= event->unicode && event->unicode <= 'Z') ||
			('0' <= event->unicode && event->unicode <= '9') ||
			(event->unicode == '_')
			){
			if(input_str.size() < max_length){
				input_str += (char)(event->unicode);
				status = '0'; //valid input
			} else{
				status = 'A'; // max length warning
			};
		} else if(event->unicode == 8){
			input_str = input_str.substr(0, input_str.size() - 1);
		} else{
			// return warning when user type invalid input characters
			status = (wchar_t)event->unicode;
			//std::wcout << "HANDLE-INPUT-BOX: " << (wchar_t)event->unicode << ' ' << event->unicode << "======\n";
		};
	};
	
};

#define DARK globalConfig::dark_mode

void setupInputBox(bool init,
	sf::RenderWindow& win,
	sf::RectangleShape& box_obj,
	sf::Text& label_text_obj,
	sf::Text& input_text_obj,
	const sf::FloatRect& bounds,
	unsigned short box_border_thickness,
	std::vector<sf::Color> box_fill_color,
	std::vector<sf::Color> box_outline_color,
	std::vector<sf::Color> box_selected_outline_color,
	sf::Font& text_font,
	std::vector<std::wstring> label_text_string,
	unsigned short text_size,
	std::vector<sf::Color> text_fill_color,
	std::vector<sf::Color> text_outline_color,
	unsigned short outline_thickness,
	std::optional<sf::Event>& event,
	std::string& input_string,
	unsigned char max_string_length,
	bool& isBoxSelected,
	wchar_t& status
){

	//static bool isBoxSelected = false;

	// set up box
	box_obj.setSize(bounds.size);
	box_obj.setOutlineThickness(5);
	if(isBoxSelected){
		box_obj.setOutlineColor(box_selected_outline_color[globalConfig::dark_mode]);
		handleInputBox(input_string, event, max_string_length, status);
	} else{
		box_obj.setOutlineColor(box_outline_color[globalConfig::dark_mode]);
	};
	box_obj.setFillColor(box_fill_color[globalConfig::dark_mode]);
	box_obj.setPosition(bounds.position);
	if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
		if(bounds.contains(win.mapPixelToCoords(sf::Mouse::getPosition(win)))){
			isBoxSelected = true;
		} else{
			isBoxSelected = false;
		};
	};

	// set up label
	label_text_obj.setString(label_text_string[DARK]);
	label_text_obj.setCharacterSize(25);
	label_text_obj.setFillColor(text_fill_color[DARK]);
	label_text_obj.setOutlineColor(text_outline_color[DARK]);
	label_text_obj.setOutlineThickness(3);
	label_text_obj.setOrigin({
		0,
		label_text_obj.getLocalBounds().size.y
	});
	label_text_obj.setPosition(bounds.position);

	// process event

	// set up input text
	input_text_obj.setCharacterSize(25);
	input_text_obj.setString(input_string);
	input_text_obj.setFont(text_font);
	input_text_obj.setOutlineThickness(3);
	input_text_obj.setFillColor(
		(DARK == 1) ? (sf::Color::White) : (sf::Color::Black)
	);
	input_text_obj.setOutlineColor(
		(DARK == 1) ? (sf::Color::Black) : (sf::Color::White)
	);
	input_text_obj.setOrigin({
		0,
		input_text_obj.getLocalBounds().size.y / 2.f
		});
	input_text_obj.setPosition({
		box_obj.getPosition().x + 15,
		box_obj.getPosition().y + box_obj.getSize().y / 2.f
	});
	//std::wcout << status << ' ';
};

void setUpTextObj(
	const bool& init,
	sf::RenderWindow& win,
	sf::Text& text_obj,
	const std::vector<sf::Color>& fill_colors,
	const std::vector<sf::Color>& outline_colors,
	unsigned short outline_thickness,
	unsigned short character_size,
	const sf::Vector2f& origin,
	const sf::Vector2f& pos,
	std::wstring text_strings[2]
){
	if(init){
		text_obj.setCharacterSize(character_size);
		text_obj.setString(text_strings[globalConfig::language]);
		text_obj.setFillColor(fill_colors[globalConfig::dark_mode]);
		text_obj.setOutlineColor(outline_colors[globalConfig::dark_mode]);
		text_obj.setOutlineThickness(outline_thickness);
		text_obj.setOrigin({
			origin.x * text_obj.getLocalBounds().size.x,
			origin.y * text_obj.getLocalBounds().size.y
		});
		text_obj.setPosition(pos);
	};
};

void trackMousePosition(sf::RenderWindow& win){
	auto m = sf::Mouse::getPosition(win);
	while(win.isOpen()){
		sf::sleep(sf::milliseconds(500));
		m = sf::Mouse::getPosition(win);
		std::cout << "Mouse pos: " << m.x << " " << m.y << '\n';
	};
};

#endif