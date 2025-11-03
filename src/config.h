#ifndef CONFIG_H
#define CONFIG_H

namespace globalConfig{
	extern unsigned int win_width, win_height;
	extern bool dark_mode;
	extern int current_win; 
	// 0 = program terminated
	// 1 = menu
	// 2 = play screen
	// 3 = game screen
	// 4 = settings
	// 5 = in game settings

	extern short language;
	// 0 = English
	// 1 = Vietnamese
};

#endif
