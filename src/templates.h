#ifndef TEMPLATES_H
#define TEMPLATES_H

#include "config.h"

struct gameDataPackage {
	bool is_new_game, is_multiplayer;
	int first_turn; // 0 for X, 1 for O
	std::string
		save_name,
		playerX_name,
		playerO_name;
	std::filesystem::path load_game_from;
};

bool setupIconButton(
	bool init,
	sf::RenderWindow& win,
	sf::Sprite& button_obj,
	sf::Texture textures[6],
	sf::FloatRect bounds,
	std::optional<sf::Event>& event
);

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
);

void setupInputBox(
	bool init,
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
	wchar_t& status //A = max_length warning, B = empty_warning
);

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
);

void trackMousePosition(sf::RenderWindow& win);

#endif
