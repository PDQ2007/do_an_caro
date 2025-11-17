#ifndef TEMPLATES_CPP
#define TEMPLATES_CPP

#include "config.h"

#define DARK (globalConfig::dark_mode * 3)

bool setupIconButton(
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
	if(change){
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
	return_val = setupIconButton(win, button_obj, textures, bounds, event);
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

#endif