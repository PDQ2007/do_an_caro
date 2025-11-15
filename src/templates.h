#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "config.h"

struct icon_button {

	sf::Texture textures[6];
	sf::Sprite button_obj{textures[0]};
	sf::FloatRect bounds;
	sf::Vector2f origin;

	//constructor
	icon_button(
		const std::filesystem::path path[6],
		const sf::Vector2f origin,
		const sf::Vector2f position,
		const sf::Vector2f size
	);

	void reInit(		
		const std::filesystem::path path[6],
		sf::Vector2f origin,
		sf::Vector2f position,
		sf::Vector2f size
	);

	void draw(
		sf::RenderWindow& win,
		std::optional<sf::Event> events,
		void (*func)(std::optional<sf::Event> )
	);

};

//----------------------------------------------

struct text_button {

	sf::Font text_font[2];
	sf::Text texts[2] = {
		sf::Text(text_font[0], "English"),
		sf::Text(text_font[0], "Vietnamese")
	};
	sf::Vector2f text_alignment_to_button;
	sf::Color text_color[2];
	sf::Texture textures[6];
	sf::Sprite button_obj{textures[0]};
	sf::FloatRect bounds;
	sf::Vector2f origin;

	//constructor
	text_button(
		const std::filesystem::path (&path)[6],
		const std::wstring (&button_text)[2],
		const unsigned short font_size,
		const sf::Color (&text_fill_color)[2],
		const sf::Color (&text_outline_color)[2],0
		const unsigned short text_outline_thickness,
		const sf::Vector2f& text_alignment_to_button,
		const sf::Font& text_font,
		const sf::Vector2f button_origin,
		const sf::Vector2f button_position,
		const sf::Vector2f button_size
	);

	void reInit(		
		const std::filesystem::path (&path)[6],
		const std::wstring (&button_text)[2],
		const unsigned short font_size,
		const sf::Color (&text_fill_color)[2],
		const sf::Color (&text_outline_color)[2],
		const unsigned short text_outline_thickness,
		const sf::Vector2f& text_alignment_to_button,
		const sf::Font& text_font,
		const sf::Vector2f button_origin,
		const sf::Vector2f button_position,
		const sf::Vector2f button_size
	);

	//2 = there is an event to process
	bool draw(
		sf::RenderWindow& win,
		std::optional<sf::Event> events
	);

};

#endif
