#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <thread>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include <optional>
#include "templates.h"
#include "menu_window.h"
#include "process.h"
#include "game_window.h"
#include "different_window.h"
#include "settings_window.h"
#include "prepare_window.h"

namespace globalConfig{
	extern unsigned int win_width, win_height;
	extern bool dark_mode, re_init, sound_on;
	extern int current_win, previous_win; 
	// 0 = program terminated
	// 1 = menu
	// 2 = play screen
	// 3 = game screen
	// 4 = settings
	// 5 = in game settings

	extern short language;
	// 0 = english
	// 1 = vietnamese
};

#endif
