#ifndef TEMPLATES_H
#define TEMPLATES_H

#include "config.h"

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

void trackMousePosition(sf::RenderWindow& win);

#endif
