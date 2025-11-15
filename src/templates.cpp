#ifndef TEMPLATE_CPP
#define TEMPLATE_CPP

// This file create universal data-type of interative elements
// like buttons and input boxes.

#include "config.h"
#include "templates.h"

//----------------------------------------------

namespace fonts{
	sf::Font minecraft("resources/fonts/minecraft-f2d-v1-42.ttf");
};

#define DARK (globalConfig::dark_mode * 3)

icon_button::icon_button(
	const std::filesystem::path path[6],
	const sf::Vector2f origin,
	const sf::Vector2f position,
	const sf::Vector2f size
){
	//init textures
	this->textures[0].loadFromFile(path[0]);
	this->textures[1].loadFromFile(path[1]);
	this->textures[2].loadFromFile(path[2]);
	this->textures[3].loadFromFile(path[3]);
	this->textures[4].loadFromFile(path[4]);
	this->textures[5].loadFromFile(path[5]);

	//init origin, size and position
	this->origin = origin;
	this->button_obj.setTexture(this->textures[DARK + 0]);
	this->button_obj.setScale({
		size.x / this->textures[DARK + 0].getSize().x,
		size.y / this->textures[DARK + 0].getSize().y
		});
	this->button_obj.setOrigin({
		origin.x * this->button_obj.getLocalBounds().size.x,
		origin.y * this->button_obj.getLocalBounds().size.y
	});
	this->button_obj.setPosition(position);

	// update bounds
	this->bounds = this->button_obj.getGlobalBounds();
};

void icon_button::reInit(		
	const std::filesystem::path path[6],
	sf::Vector2f origin,
	sf::Vector2f position,
	sf::Vector2f size
){
	*this = icon_button(path, origin, position, size);
};

void icon_button::draw(
	sf::RenderWindow& win,
	std::optional<sf::Event> events,
	void (*func)(std::optional<sf::Event> )
){
	if(bounds.contains(win.mapPixelToCoords(sf::Mouse::getPosition(win)))){
		if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
			this->button_obj.setTexture(this->textures[DARK + 2]);
			if(events->is<sf::Event::MouseButtonPressed> ()){
				std::thread thread_to_process_event(func, events);
			};
		} else{
			this->button_obj.setTexture(this->textures[DARK + 1]);
		};
	} else{
		this->button_obj.setTexture(this->textures[DARK + 0]);
	};
	win.draw(this->button_obj);
};


//----------------------------------------------

//constructor
text_button::text_button(
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
){
	
	//init textures
	bool in = this->textures[0].loadFromFile(path[0]);
	in = this->textures[1].loadFromFile(path[1]);
	in = this->textures[2].loadFromFile(path[2]);
	in = this->textures[3].loadFromFile(path[3]);
	in = this->textures[4].loadFromFile(path[4]);
	in = this->textures[5].loadFromFile(path[5]);

	//init origin, size and position of the button
	this->origin = button_origin;
	this->button_obj.setTexture(this->textures[DARK + 0]);
	this->button_obj.setScale({
		button_size.x / this->textures[DARK + 0].getSize().x,
		button_size.y / this->textures[DARK + 0].getSize().y
	});
	this->button_obj.setOrigin({
		this->origin.x * this->button_obj.getLocalBounds().size.x,
		this->origin.y * this->button_obj.getLocalBounds().size.y
	});
	this->button_obj.setPosition(button_position);

	// update bounds
	this->bounds = this->button_obj.getGlobalBounds();
	
	//init origin, size and position of the text
	auto init_text = [
		this,
		text_fill_color,
		text_outline_color,
		text_outline_thickness,
		button_text,
		text_alignment_to_button,
		font_size,
		text_font
	](bool idx){
		this->text_font[idx] = text_font;
		this->texts[idx].setFont(text_font);
		this->texts[idx].setCharacterSize(font_size);
		this->texts[idx].setFont(text_font);
		this->texts[idx].setFillColor(text_fill_color[globalConfig::dark_mode]);
		this->texts[idx].setOutlineColor(text_outline_color[globalConfig::dark_mode]);
		this->texts[idx].setOutlineThickness(text_outline_thickness);
		this->texts[idx].setString(button_text[idx]);
		this->texts[idx].setOrigin({
			this->texts[idx].getLocalBounds().size.x / 2.f,
			this->texts[idx].getLocalBounds().size.y / 2.f
			});
		this->text_alignment_to_button = text_alignment_to_button;

		// throw error if text is too big to fit into the button
		/*if(
			this->button_obj.getGlobalBounds().size.x <= text_obj.getGlobalBounds().size.x ||
			this->button_obj.getGlobalBounds().size.y <= text_obj.getGlobalBounds().size.y
		){
			std::cout << "Runtime eror: text is too big to fit into the button\n";
			throw std::runtime_error("text is too big to fit into the button");
		};*/
		
		sf::Vector2f delta = {
			this->button_obj.getGlobalBounds().size.x - this->texts[idx].getGlobalBounds().size.x,
			this->button_obj.getGlobalBounds().size.y - this->texts[idx].getGlobalBounds().size.y
		};
		this->texts[idx].setPosition({
			(this->button_obj.getGlobalBounds().size.x - delta.x) / 2 + text_alignment_to_button.x * delta.x,
			(this->button_obj.getGlobalBounds().size.y - delta.y) / 2 + text_alignment_to_button.y * delta.y
		});
		
	};

	init_text(0);
	
	init_text(1);

};

void text_button::reInit(		
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
){
	*this = text_button(
		path, button_text, font_size, text_fill_color, text_outline_color,
		text_outline_thickness, text_alignment_to_button, text_font,
		button_origin, button_position, button_size
	);
};

bool text_button::draw(
	sf::RenderWindow& win,
	std::optional<sf::Event> events
){
	static bool return_val = 0;
	
	if(bounds.contains(win.mapPixelToCoords(sf::Mouse::getPosition(win)))){
		if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
			this->button_obj.setTexture(this->textures[DARK + 2]);
			if(events && events->is<sf::Event::MouseButtonPressed> ()){
				return_val = 1;
			};
		} else{
			this->button_obj.setTexture(this->textures[DARK + 1]);
			return_val = 0;
			
		};
	} else{
		this->button_obj.setTexture(this->textures[DARK + 0]);
		return_val = 0;
		
	};
	win.draw(this->button_obj);
	//std::cout << "pass?\n";
	//win.draw(this->texts[globalConfig::language]);

	//std::cout << "pass?\n";
	
	return return_val;
};

#undef DARK

#endif